#include "StdAfx.h"
#include "cAnimation.h"
#include "cAllocateHierarchy.h"
#include "cCamera.h"

static LPD3DXMESH			pBoundingSphereMesh;

cAnimation::cAnimation(char* szFolder, char* szFilename)
	: m_pRootFrame(NULL)
	, m_pAnimController(NULL)
	, m_dwWorkingPaletteSize(0)
	, m_pmWorkingPalette(NULL)
	, m_pEffect(NULL)
	, m_fBlendTime(0.3f)
	, m_fPassedBlendTime(0.0f)
	, m_isBlending(false)
	, m_OnceRoop(false)
{

	cAnimation* pSkinnedMesh = Animation_M->GetSkinnedMesh(szFolder, szFilename);

	m_pRootFrame = pSkinnedMesh->m_pRootFrame;
	m_dwWorkingPaletteSize = pSkinnedMesh->m_dwWorkingPaletteSize;
	m_pmWorkingPalette = pSkinnedMesh->m_pmWorkingPalette;
	m_pEffect = pSkinnedMesh->m_pEffect;
	m_stBoundingSphere = pSkinnedMesh->m_stBoundingSphere;

	if (pSkinnedMesh->m_pAnimController) {
		pSkinnedMesh->m_pAnimController->CloneAnimationController(
			pSkinnedMesh->m_pAnimController->GetMaxNumAnimationOutputs(),
			pSkinnedMesh->m_pAnimController->GetMaxNumAnimationSets(),
			pSkinnedMesh->m_pAnimController->GetMaxNumTracks(),
			pSkinnedMesh->m_pAnimController->GetMaxNumEvents(),
			&m_pAnimController);
	}
}


cAnimation::cAnimation()
	: m_pRootFrame(NULL)
	, m_pAnimController(NULL)
	, m_dwWorkingPaletteSize(0)
	, m_pmWorkingPalette(NULL)
	, m_pEffect(NULL)
{
}

cAnimation::~cAnimation(void)
{
	SAFE_RELEASE(m_pAnimController);
	SAFE_RELEASE(pBoundingSphereMesh);

}

void cAnimation::Load(char* szDirectory, char* szFilename)
{
	m_pEffect = LoadEffect("MultiAnimation.hpp");

	int nPaletteSize = 0;
	m_pEffect->GetInt("MATRIX_PALETTE_SIZE", &nPaletteSize);

	cAllocateHierarchy ah;
	ah.SetDirectory(szDirectory);
	ah.SetDefaultPaletteSize(nPaletteSize);

	std::string sFullPath(szDirectory);
	sFullPath += std::string(szFilename);

	D3DXLoadMeshHierarchyFromX(sFullPath.c_str(),
		D3DXMESH_MANAGED,
		g_pD3DDevice,
		&ah,
		NULL,
		(LPD3DXFRAME*)&m_pRootFrame,
		&m_pAnimController);

	m_stBoundingSphere = ah.GetBoundingSphere();

	if (pBoundingSphereMesh == NULL)
	{
		pBoundingSphereMesh = cUt::SphereMake(m_stBoundingSphere.fRadius, 20, 20);
	}

	if (m_pmWorkingPalette)
		delete[] m_pmWorkingPalette;

	m_dwWorkingPaletteSize = ah.GetMaxPaletteSize();
	m_pmWorkingPalette = new D3DXMATRIX[m_dwWorkingPaletteSize];
	if (m_pmWorkingPalette == NULL)
	{
		m_dwWorkingPaletteSize = 0;
	}

	if (m_pRootFrame)
		SetupBoneMatrixPtrs(m_pRootFrame);



}


void cAnimation::UpdateAndRender(D3DXMATRIXA16* tran)
{
	cGameObject::Update();

	if (m_pRootFrame == NULL)
		return;

	if (m_isBlending)
	{
		m_fPassedBlendTime += g_pTimeManager->GetDeltaTime();

		if (m_fPassedBlendTime >= m_fBlendTime)
		{
			m_isBlending = false;
			m_pAnimController->SetTrackWeight(0, 1.0f);
			m_pAnimController->SetTrackWeight(1, 0.0f);
			m_pAnimController->SetTrackEnable(1, false);
		}
		else
		{
			float fWeight = m_fPassedBlendTime / m_fBlendTime;
			m_pAnimController->SetTrackWeight(0, fWeight);
			m_pAnimController->SetTrackWeight(1, 1.0f - fWeight);
		}
	}
	if (m_pAnimController)
	{
		m_pAnimController->AdvanceTime(g_pTimeManager->GetDeltaTime(), NULL);
	}
	Update(m_pRootFrame, tran);
	Render(m_pRootFrame);
}

void cAnimation::Render(D3DXFRAME_DERIVED* pBone /*= NULL*/) {
	assert(pBone);

	// �� �������� �޽� �����̳ʿ� �ִ� pSkinInfo�� �̿��Ͽ� ����޴� ��� 
	// �������� ��Ʈ������ ppBoneMatrixPtrs�� �����Ѵ�.
	if (pBone->pMeshContainer)
	{
		D3DXMESHCONTAINER_DERIVED* pBoneMesh = (D3DXMESHCONTAINER_DERIVED*)pBone->pMeshContainer;

		// get bone combinations
		LPD3DXBONECOMBINATION pBoneCombos =
			(LPD3DXBONECOMBINATION)(pBoneMesh->pBufBoneCombos->GetBufferPointer());

		D3DXMATRIXA16 matViewProj, matView, matProj;
		g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
		g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);
		matViewProj = matView * matProj;

		D3DXMATRIXA16 mView, mInvView;
		g_pD3DDevice->GetTransform(D3DTS_VIEW, &mView);
		D3DXMatrixInverse(&mInvView, 0, &mView);
		D3DXVECTOR3 vEye = D3DXVECTOR3(0, 0, 0);
		D3DXVec3TransformCoord(&vEye, &vEye, &mInvView);

		DWORD dwAttrSize = 0;
		pBoneMesh->pOrigMesh->GetAttributeTable(0, &dwAttrSize);
		D3DXATTRIBUTERANGE* pRange = new D3DXATTRIBUTERANGE[dwAttrSize];
		pBoneMesh->pOrigMesh->GetAttributeTable(pRange, &dwAttrSize);


		// for each palette
		for (DWORD dwAttrib = 0; dwAttrib < pBoneMesh->dwNumAttrGroups; ++dwAttrib)
		{
			// set each transform into the palette
			for (DWORD dwPalEntry = 0; dwPalEntry < pBoneMesh->dwNumPaletteEntries; ++dwPalEntry)
			{
				DWORD dwMatrixIndex = pBoneCombos[dwAttrib].BoneId[dwPalEntry];
				if (dwMatrixIndex != UINT_MAX)
				{
					m_pmWorkingPalette[dwPalEntry] =
						pBoneMesh->pBoneOffsetMatrices[dwMatrixIndex] *
						(*pBoneMesh->ppBoneMatrixPtrs[dwMatrixIndex]);
				}
			}

			// set the matrix palette into the effect
			m_pEffect->SetMatrixArray("amPalette",
				m_pmWorkingPalette,
				pBoneMesh->dwNumPaletteEntries);

			m_pEffect->SetMatrix("g_mViewProj", &matViewProj);
			m_pEffect->SetVector("vLightDiffuse", &D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
			m_pEffect->SetVector("vWorldLightPos", &D3DXVECTOR4(1000.0f, 1000.0f, 1000.0f, 1.0f));
			m_pEffect->SetVector("vWorldCameraPos", &D3DXVECTOR4(vEye, 1.0f));
			m_pEffect->SetVector("vMaterialAmbient", &D3DXVECTOR4(0.53f, 0.53f, 0.53f, 0.53f));
			m_pEffect->SetVector("vMaterialDiffuse", &D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));

			// we're pretty much ignoring the materials we got from the x-file; just set
			// the texture here
			m_pEffect->SetTexture("g_txScene", pBoneMesh->vecTexture[pBoneCombos[dwAttrib].AttribId - pRange[0].AttribId]);

			// set the current number of bones; this tells the effect which shader to use
			m_pEffect->SetInt("CurNumBones", pBoneMesh->dwMaxNumFaceInfls - 1);

			// set the technique we use to draw
			m_pEffect->SetTechnique("Skinning20");

			UINT uiPasses, uiPass;

			// run through each pass and draw
			m_pEffect->Begin(&uiPasses, 0);
			for (uiPass = 0; uiPass < uiPasses; ++uiPass)
			{
				m_pEffect->BeginPass(uiPass);
				pBoneMesh->pWorkingMesh->DrawSubset(dwAttrib);
				m_pEffect->EndPass();
			}
			m_pEffect->End();
		}
	}

	//��������� ��� �����ӿ� ���ؼ� ����.
	if (pBone->pFrameSibling)
	{
		Render((D3DXFRAME_DERIVED*)pBone->pFrameSibling);
	}

	if (pBone->pFrameFirstChild)
	{
		Render((D3DXFRAME_DERIVED*)pBone->pFrameFirstChild);
	}
}

LPD3DXEFFECT cAnimation::LoadEffect(char* szFilename)
{
	LPD3DXEFFECT pEffect = NULL;

	D3DXMACRO mac[2] =
	{
		{ "MATRIX_PALETTE_SIZE_DEFAULT", "35" },
		{ NULL,                          NULL }
	};

	D3DCAPS9 caps;
	D3DXMACRO *pmac = NULL;
	g_pD3DDevice->GetDeviceCaps(&caps);
	if (caps.VertexShaderVersion > D3DVS_VERSION(1, 1))
		pmac = mac;

	DWORD dwShaderFlags = 0;

#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DXSHADER_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

#ifdef DEBUG_VS
	dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
#endif
#ifdef DEBUG_PS
	dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
#endif

	ID3DXBuffer* pBuffer = NULL;
	if (FAILED(D3DXCreateEffectFromFile(g_pD3DDevice,
		szFilename,
		pmac,
		NULL,
		dwShaderFlags,
		NULL,
		&pEffect,
		&pBuffer)))
	{
		// if creation fails, and debug information has been returned, output debug info
		if (pBuffer)
		{
			OutputDebugStringA((char*)pBuffer->GetBufferPointer());
			SAFE_RELEASE(pBuffer);
		}

		return NULL;
	}

	return pEffect;
}

void cAnimation::Update(D3DXFRAME_DERIVED* pCurrent, D3DXMATRIXA16* pmatParent)
{
	pCurrent->CombinedTransformationMatrix = pCurrent->TransformationMatrix;
	if (pmatParent)
	{
		pCurrent->CombinedTransformationMatrix =
			pCurrent->CombinedTransformationMatrix * (*pmatParent);
	}

	if (pCurrent->pFrameSibling)
	{
		Update((D3DXFRAME_DERIVED*)pCurrent->pFrameSibling, pmatParent);
	}

	if (pCurrent->pFrameFirstChild)
	{
		Update((D3DXFRAME_DERIVED*)pCurrent->pFrameFirstChild, &(pCurrent->CombinedTransformationMatrix));
	}
}

void cAnimation::SetupBoneMatrixPtrs(D3DXFRAME_DERIVED* pBone)
{
	assert(pBone);

	// �� �������� �޽� �����̳ʿ� �ִ� pSkinInfo�� �̿��Ͽ� ����޴� ��� 
	// �������� ��Ʈ������ ppBoneMatrixPtrs�� �����Ѵ�.
	if (pBone && pBone->pMeshContainer)
	{
		D3DXMESHCONTAINER_DERIVED* pBoneMesh = (D3DXMESHCONTAINER_DERIVED*)pBone->pMeshContainer;

		if (pBoneMesh->pSkinInfo)
		{
			LPD3DXSKININFO pSkinInfo = pBoneMesh->pSkinInfo;

			// pSkinInfo->GetNumBones() ���� ����޴� ���� ������ ã��.
			// pSkinInfo->GetBoneName(i) �� i�� �������� �̸��� ã��
			// D3DXFrameFind(��Ʈ ������, ������ �̸�) �� �������� ã��.
			// ã�Ƽ� �����Ʈ������ �ɾ����.
			DWORD dwNumBones = pSkinInfo->GetNumBones();
			for (DWORD i = 0; i < dwNumBones; ++i)
			{
				LPCSTR szBoneName = pSkinInfo->GetBoneName(i);
				if (szBoneName == NULL || strlen(szBoneName) == 0)
					continue;
				D3DXFRAME_DERIVED* pInfluence = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, szBoneName);
				pBoneMesh->ppBoneMatrixPtrs[i] = &(pInfluence->CombinedTransformationMatrix);
			}
		}
	}

	//��������� ��� �����ӿ� ���ؼ� ����.
	if (pBone->pFrameSibling)
	{
		SetupBoneMatrixPtrs((D3DXFRAME_DERIVED*)pBone->pFrameSibling);
	}

	if (pBone->pFrameFirstChild)
	{
		SetupBoneMatrixPtrs((D3DXFRAME_DERIVED*)pBone->pFrameFirstChild);
	}
}

void cAnimation::SetAnimationIndex(int nIndex)
{
	if (!m_pAnimController)
		return;
	LPD3DXANIMATIONSET pAnimSet = NULL;
	m_pAnimController->GetAnimationSet(nIndex, &pAnimSet);
	m_pAnimController->SetTrackAnimationSet(0, pAnimSet);
	SAFE_RELEASE(pAnimSet);

}

float cAnimation::AnimationFrame(int num)
{
	LPD3DXANIMATIONSET pAnimSet = NULL;
	m_pAnimController->GetAnimationSet(num, &pAnimSet);
	double dPeriod = pAnimSet->GetPeriod();

	return dPeriod;

	//D3DXTRACK_DESC tc;
	//m_pAnimController->GetTrackDesc(num, &tc);
	//double dPosition = tc.Position;

	//if (dPeriod > dPosition)
	//{
	//	//SetAnimationIndex(0, 4);
	//}
	//else
	//{
	//	m_bEnd = true;
	//}

	SAFE_RELEASE(pAnimSet);
}

void cAnimation::SetAnimationIndex1(int First, int Last)
{
	//LPD3DXANIMATIONSET pAnimSet = NULL;
	//m_pAnimController->GetAnimationSet(n, &pAnimSet);
	//m_pAnimController->SetTrackAnimationSet(track, pAnimSet);
	////m_pAnimController->SetTrackSpeed(0, -1);
	//SAFE_RELEASE(pAnimSet);
	LPD3DXANIMATIONSET pPrevAnimationSet = NULL;
	LPD3DXANIMATIONSET pNextAnimationSet = NULL;

	m_pAnimController->GetAnimationSet(Last, &pPrevAnimationSet);
	m_pAnimController->SetTrackAnimationSet(1, pPrevAnimationSet);

	m_pAnimController->GetAnimationSet(First, &pNextAnimationSet);
	m_pAnimController->SetTrackAnimationSet(0, pNextAnimationSet);

	m_pAnimController->SetTrackEnable(1, true);
	m_pAnimController->SetTrackPosition(0, 0);

	SAFE_RELEASE(pPrevAnimationSet);
	SAFE_RELEASE(pNextAnimationSet);
}

void cAnimation::Destroy()
{
	cAllocateHierarchy ah;
	D3DXFrameDestroy((LPD3DXFRAME)m_pRootFrame, &ah);
	SAFE_DELETE_ARR(m_pmWorkingPalette);
	SAFE_RELEASE(m_pEffect);
}

void cAnimation::GetAnimController(LPD3DXANIMATIONCONTROLLER * ppAC)
{
	assert(ppAC != NULL);
	m_pAnimController->AddRef();
	*ppAC = m_pAnimController;
}

void cAnimation::SetRandomTrackPosition()
{
	m_pAnimController->SetTrackPosition(0, (rand() % 100) / 10.0f);
}

D3DXFRAME_DERIVED* cAnimation::GetFineBONE(std::string boneName)
{
	return (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, boneName.c_str());
}