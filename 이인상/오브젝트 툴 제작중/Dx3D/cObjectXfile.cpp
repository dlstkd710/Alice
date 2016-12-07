#include "StdAfx.h"
#include "cObjectXfile.h"
#include "cMtlTex.h"

cObjectXfile::cObjectXfile(void)
	: m_pRootBone(NULL)
	, m_pAnimController(NULL)
{
}

cObjectXfile::~cObjectXfile(void)
{
	SAFE_RELEASE(m_pAnimController);

	cObjectAllocate alloc;
	D3DXFrameDestroy(m_pRootBone, &alloc);
}

void cObjectXfile::Load(std::string sFolder, std::string sFile)
{
	std::string sFullPath = sFolder + "/" + sFile;
	cObjectAllocate alloc;
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

void cObjectXfile::Update(ST_BONE* pBone, D3DXMATRIX* pParent)
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

void cObjectXfile::Update()
{
	if (m_pRootBone == NULL)
		return;

	//m_pAnimController->AdvanceTime(g_pTimeManager->GetDeltaTime(), NULL);

	//Update(m_pRootBone, NULL);
	//UpdateSkinnedMesh(m_pRootBone);
}

void cObjectXfile::Render(ST_BONE* pBone)
{
	ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;
	while (pBoneMesh)
	{
		for (size_t i = 0; i < pBoneMesh->vecMtlTex.size(); ++i)
		{
			D3DXMATRIXA16 matS, matT;
			D3DXVECTOR3 thisPos = this->GetPosition();
			D3DXMatrixScaling(&matS, m_vScale.x, m_vScale.y, m_vScale.z);
			D3DXMatrixTranslation(&matT, thisPos.x, thisPos.y, thisPos.z);

			m_matWorld = matS * matT;

			g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
			g_pD3DDevice->SetMaterial(&pBoneMesh->vecMtlTex[i]->GetMtl());
			g_pD3DDevice->SetTexture(0, pBoneMesh->vecMtlTex[i]->GetTexture());
			pBoneMesh->pOrigMesh->DrawSubset(i);
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

void cObjectXfile::Render()
{
	if (m_pRootBone == NULL) return;
	Render(m_pRootBone);
}

void cObjectXfile::SetupBoneMatrixPtrs(ST_BONE* pBone)
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

void cObjectXfile::UpdateSkinnedMesh(ST_BONE* pBone)
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

void cObjectXfile::SetAnimationIndex(int n)
{
	LPD3DXANIMATIONSET pAnimSet = NULL;
	m_pAnimController->GetAnimationSet(n, &pAnimSet);
	//m_pAnimController->SetTrackSpeed(0, -1);
	m_pAnimController->SetTrackAnimationSet(0, pAnimSet);
	SAFE_RELEASE(pAnimSet);
}
