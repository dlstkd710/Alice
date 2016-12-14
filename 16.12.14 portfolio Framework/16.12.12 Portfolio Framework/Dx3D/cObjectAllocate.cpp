#include "StdAfx.h"
#include "cObjectAllocate.h"
#include "cMtlTex.h"

cObjectAllocate::cObjectAllocate(void)
	:cMin(FLT_MAX, FLT_MAX, FLT_MAX),
	cMax(FLT_MIN, FLT_MIN, FLT_MIN)
{
}

cObjectAllocate::~cObjectAllocate(void)
{
}

STDMETHODIMP cObjectAllocate::CreateFrame(
	THIS_ LPCSTR Name,
	LPD3DXFRAME *ppNewFrame)
{
	ST_BONE* pBone = new ST_BONE;
	// TODO : 이름을 복사해주세요.
	pBone->Name = NULL;
	if (Name)
	{
		pBone->Name = new char[strlen(Name) + 1];
		strcpy(pBone->Name, Name);
	}

	D3DXMatrixIdentity(&pBone->CombinedTransformationMatrix);
	D3DXMatrixIdentity(&pBone->TransformationMatrix);
	pBone->pFrameFirstChild = NULL;
	pBone->pFrameSibling = NULL;
	pBone->pMeshContainer = NULL;

	*ppNewFrame = pBone;

	return S_OK;
}

STDMETHODIMP cObjectAllocate::CreateMeshContainer(
	THIS_ LPCSTR Name,
	CONST D3DXMESHDATA *pMeshData,
	CONST D3DXMATERIAL *pMaterials,
	CONST D3DXEFFECTINSTANCE *pEffectInstances,
	DWORD NumMaterials,
	CONST DWORD *pAdjacency,
	LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
	ST_BONE_MESH* pBoneMesh = new ST_BONE_MESH;

	assert(pMeshData->Type == D3DXMESHTYPE_MESH);

	pBoneMesh->MeshData.pMesh = NULL;

	SAFE_ADD_REF(pMeshData->pMesh);

	// step 1. pSkinInfo 저장
	pBoneMesh->pSkinInfo = NULL;
	if (pSkinInfo)
	{
		SAFE_ADD_REF(pSkinInfo);
		pBoneMesh->pSkinInfo = pSkinInfo;
	}
	pBoneMesh->pOrigMesh = pMeshData->pMesh;

	// step 2. 원본 메쉬 복사
	pBoneMesh->pOrigMesh->CloneMeshFVF(
		pBoneMesh->pOrigMesh->GetOptions(),
		pBoneMesh->pOrigMesh->GetFVF(),
		g_pD3DDevice,
		&pBoneMesh->pWorkMesh);

	pBoneMesh->Name = NULL;
	pBoneMesh->NumMaterials = NumMaterials;
	pBoneMesh->pAdjacency = NULL;
	pBoneMesh->pEffects = NULL;

	D3DXVECTOR3 vMin(FLT_MAX, FLT_MAX, FLT_MAX);
	D3DXVECTOR3 vMax(FLT_MIN, FLT_MIN, FLT_MIN);

	LPVOID pV = NULL;
	pMeshData->pMesh->LockVertexBuffer(0, &pV);
	D3DXComputeBoundingBox((D3DXVECTOR3*)pV,
		pMeshData->pMesh->GetNumVertices(),
		D3DXGetFVFVertexSize(ST_PNT_VERTEX::FVF),
		&vMin,
		&vMax);
	D3DXVec3Minimize(&cMin, &cMin, &vMin);
	D3DXVec3Maximize(&cMax, &cMax, &vMax);
	pMeshData->pMesh->UnlockVertexBuffer();

	for (DWORD i = 0; i < NumMaterials; ++i)
	{
		cMtlTex* pMtlTex = new cMtlTex;
		D3DMATERIAL9 tempMeterial;
		ZeroMemory(&tempMeterial, sizeof(D3DMATERIAL9));

		if (pMaterials[i].pTextureFilename) {
			tempMeterial = pMaterials[i].MatD3D;
			pMtlTex->SetMtl(tempMeterial);
			std::string sFullPath = m_sFolder + "/" + std::string(pMaterials[i].pTextureFilename);
			pMtlTex->SetTexture(g_pTextureManager->GetTexture(sFullPath));
			pBoneMesh->vecMtlTex.push_back(pMtlTex);
		}
	}
	pBoneMesh->pNextMeshContainer = NULL;

	// step 3. pSkinInfo->GetNumBones()를 통해
	// 영향력을 미치는 모든 본에 대한 매트릭스 들을 세팅
	// ppBoneMatrixPtrs, pBoneOffsetMatrices, pCurrentBoneMatrices를 동적할당
	if (pSkinInfo)
	{
		DWORD dwNumBones = pSkinInfo->GetNumBones();
		pBoneMesh->pBoneOffsetMatrices = new D3DXMATRIXA16[dwNumBones];
		pBoneMesh->pCurrentBoneMatrices = new D3DXMATRIXA16[dwNumBones];
		pBoneMesh->ppBoneMatrixPtrs = new D3DXMATRIXA16*[dwNumBones];

		// step 4. 동적 할당된 pBoneOffsetMatrices 매트릭스에 값 저장.
		// pSkinInfo->GetBoneOffsetMatrix(i)
		for (DWORD i = 0; i < dwNumBones; ++i)
		{
			LPD3DXMATRIX pmat = pSkinInfo->GetBoneOffsetMatrix(i);
			pBoneMesh->pBoneOffsetMatrices[i] = *pmat;
		}
	}

	*ppNewMeshContainer = pBoneMesh;
	return S_OK;
}

STDMETHODIMP cObjectAllocate::DestroyFrame(
	THIS_ LPD3DXFRAME pFrameToFree)
{
	// TODO : 해제 잘 합시다.
	SAFE_DELETE_ARR(pFrameToFree->Name);
	SAFE_DELETE(pFrameToFree);

	return S_OK;
}

STDMETHODIMP cObjectAllocate::DestroyMeshContainer(
	THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	// TODO : 해제 잘 합시다.
	ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pMeshContainerToFree;

	SAFE_RELEASE(pBoneMesh->pOrigMesh);
	SAFE_RELEASE(pBoneMesh->pWorkMesh);

	for each(auto p in pBoneMesh->vecMtlTex)
	{
		SAFE_RELEASE(p);
	}
	pBoneMesh->vecMtlTex.clear();

	SAFE_RELEASE(pBoneMesh->pSkinInfo);

	SAFE_DELETE_ARR(pBoneMesh->pBoneOffsetMatrices);
	SAFE_DELETE_ARR(pBoneMesh->pCurrentBoneMatrices);
	SAFE_DELETE_ARR(pBoneMesh->ppBoneMatrixPtrs);

	SAFE_DELETE(pMeshContainerToFree);

	return S_OK;
}
