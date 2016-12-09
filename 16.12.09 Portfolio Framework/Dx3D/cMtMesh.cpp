#include "StdAfx.h"
#include "cMtMesh.h"
#include "cMtlTex.h"

cMtMesh::cMtMesh(void)
	: m_pRootBone(NULL)
	, m_pAnimController(NULL)
	, m_fBlendTime(0.3f)
	, m_fPassedBlendTime(0.0f)
	, m_isBlending(false)
{
}

cMtMesh::~cMtMesh(void)
{
	SAFE_RELEASE(m_pAnimController);

	cMtAllocate alloc;
	D3DXFrameDestroy(m_pRootBone, &alloc);
}

void cMtMesh::Load(std::string sFolder, std::string sFile)
{
	std::string sFullPath = sFolder + "/" + sFile;
	cMtAllocate alloc;
	alloc.SetFolder(sFolder);

	D3DXLoadMeshHierarchyFromX(
		sFullPath.c_str(),
		D3DXMESH_MANAGED,
		g_pD3DDevice,
		&alloc,
		NULL,
		(LPD3DXFRAME*)&m_pRootBone,
		&m_pAnimController);

	if (m_pRootBone)
		SetupBoneMatrixPtrs(m_pRootBone);
}

void cMtMesh::Update(ST_BONE* pBone, D3DXMATRIX* pParent)
{
	if (pParent)
	{
		pBone->CombinedTransformationMatrix = pBone->TransformationMatrix * *pParent;
	}
	else
	{
		pBone->CombinedTransformationMatrix = pBone->TransformationMatrix;
	}

	if (pBone->pFrameFirstChild)
	{
		Update((ST_BONE*)pBone->pFrameFirstChild, &pBone->CombinedTransformationMatrix);
	}

	if (pBone->pFrameSibling)
	{
		Update((ST_BONE*)pBone->pFrameSibling, pParent);
	}
}

void cMtMesh::Update()
{
	cGameObject::Update();

	if (m_pRootBone == NULL)
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

	m_pAnimController->AdvanceTime(g_pTimeManager->GetDeltaTime(), NULL);

	Update(m_pRootBone, NULL);
	UpdateSkinnedMesh(m_pRootBone);
}

void cMtMesh::Render(ST_BONE* pBone)
{

	ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;
	while (pBoneMesh)
	{
		for (size_t i = 0; i < pBoneMesh->vecMtlTex.size(); ++i)
		{
			D3DXMATRIXA16 matS, matT, matR;
			D3DXVECTOR3 thisPos = this->GetPosition();
			D3DXVECTOR3 thisDir = this->GetDirection();
			D3DXMatrixScaling(&matS, m_vScale.x, m_vScale.y, m_vScale.z);
			D3DXMatrixTranslation(&matT, thisPos.x, thisPos.y, thisPos.z);
			D3DXMatrixTranslation(&matR, thisDir.x, thisDir.y, thisDir.z);
			m_matWorld = matS *matR* matT;

			g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
			g_pD3DDevice->SetMaterial(&pBoneMesh->vecMtlTex[i]->GetMtl());
			g_pD3DDevice->SetTexture(0, pBoneMesh->vecMtlTex[i]->GetTexture());
			pBoneMesh->pWorkMesh->DrawSubset(i);
		}
		pBoneMesh = (ST_BONE_MESH*)pBoneMesh->pNextMeshContainer;
	}

	if (pBone->pFrameFirstChild)
	{
		Render((ST_BONE*)pBone->pFrameFirstChild);
	}

	if (pBone->pFrameSibling)
	{
		Render((ST_BONE*)pBone->pFrameSibling);
	}
}

void cMtMesh::Render()
{
	if (m_pRootBone == NULL) return;
	Render(m_pRootBone);
}

void cMtMesh::SetupBoneMatrixPtrs(ST_BONE* pBone)
{
	// 각 프레임의 메시 컨테이너에 있는 pSkinInfo를 이용하여 영향받는 모든 
	// 프레임의 매트릭스를 ppBoneMatrixPtrs에 연결한다.
	// pSkinInfo->GetNumBones() 으로 영향받는 본의 개수를 찾음.
	// pSkinInfo->GetBoneName(i) 로 i번 프레임의 이름을 찾음
	// D3DXFrameFind(루트 프레임, 프레임 이름) 로 프레임을 찾음.
	// 찾아서 월드매트릭스를 걸어줘라.
	if (pBone && pBone->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;
		LPD3DXSKININFO pSkinInfo = pBoneMesh->pSkinInfo;
		if (pSkinInfo)
		{
			DWORD dwNumBones = pSkinInfo->GetNumBones();
			for (DWORD i = 0; i < dwNumBones; ++i)
			{
				LPCSTR szName = pSkinInfo->GetBoneName(i);
				ST_BONE* p = (ST_BONE*)D3DXFrameFind(m_pRootBone, szName);
				pBoneMesh->ppBoneMatrixPtrs[i] = &p->CombinedTransformationMatrix;
			}
		}
	}

	//재귀적으로 모든 프레임에 대해서 실행.
	if (pBone->pFrameFirstChild)
	{
		SetupBoneMatrixPtrs((ST_BONE*)pBone->pFrameFirstChild);
	}

	if (pBone->pFrameSibling)
	{
		SetupBoneMatrixPtrs((ST_BONE*)pBone->pFrameSibling);
	}
}

void cMtMesh::UpdateSkinnedMesh(ST_BONE* pBone)
{
	// pCurrentBoneMatrices 를 계산하시오
	// pCurrentBoneMatrices = pBoneOffsetMatrices * ppBoneMatrixPtrs 

	if (pBone && pBone->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;
		LPD3DXSKININFO pSkinInfo = pBoneMesh->pSkinInfo;
		if (pSkinInfo)
		{
			DWORD dwNumBones = pSkinInfo->GetNumBones();
			for (DWORD i = 0; i < dwNumBones; ++i)
			{
				pBoneMesh->pCurrentBoneMatrices[i] =
					pBoneMesh->pBoneOffsetMatrices[i] *
					*(pBoneMesh->ppBoneMatrixPtrs[i]);
			}

			BYTE* src = NULL;
			BYTE* dest = NULL;

			pBoneMesh->pOrigMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&src);
			pBoneMesh->pWorkMesh->LockVertexBuffer(0, (void**)&dest);

			//pWorkMesh을 업데이트 시켜준다.
			pSkinInfo->UpdateSkinnedMesh(
				pBoneMesh->pCurrentBoneMatrices, NULL, src, dest);

			pBoneMesh->pWorkMesh->UnlockVertexBuffer();
			pBoneMesh->pOrigMesh->UnlockVertexBuffer();
		}
	}

	//재귀적으로 모든 프레임에 대해서 실행.
	if (pBone->pFrameFirstChild)
	{
		UpdateSkinnedMesh((ST_BONE*)pBone->pFrameFirstChild);
	}

	if (pBone->pFrameSibling)
	{
		UpdateSkinnedMesh((ST_BONE*)pBone->pFrameSibling);
	}
}

void cMtMesh::SetAnimationIndex(int n)
{
	if (m_isBlending) return;

	LPD3DXANIMATIONSET pPrevAnimSet = NULL;
	LPD3DXANIMATIONSET pCurrAnimSet = NULL;
	// 기존 애니매이션을 1번 트랙에 세팅
	m_pAnimController->GetTrackAnimationSet(0, &pPrevAnimSet);
	m_pAnimController->SetTrackAnimationSet(1, pPrevAnimSet);

	D3DXTRACK_DESC stTrackDesc;
	m_pAnimController->GetTrackDesc(0, &stTrackDesc);
	m_pAnimController->SetTrackDesc(1, &stTrackDesc);

	// 새로운 애니매이션을 0번 트랙에 세팅
	m_pAnimController->GetAnimationSet(n, &pCurrAnimSet);
	m_pAnimController->SetTrackAnimationSet(0, pCurrAnimSet);

	// 트랙 가중치 설정
	m_pAnimController->SetTrackWeight(0, 0.0f);
	m_pAnimController->SetTrackWeight(1, 1.0f);

	// 트랙 포지션 설정
	m_pAnimController->SetTrackPosition(0, 0.0f);

	m_fPassedBlendTime = 0.0f;
	m_isBlending = true;

	SAFE_RELEASE(pPrevAnimSet);
	SAFE_RELEASE(pCurrAnimSet);
}
