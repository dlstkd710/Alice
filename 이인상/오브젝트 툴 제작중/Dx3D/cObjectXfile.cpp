#include "StdAfx.h"
#include "cObjectXfile.h"
#include "cMtlTex.h"
#include "cOBB.h"

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

void cObjectXfile::Load(std::string sFolder, std::string sFile, int Num)
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

	o_Min =	alloc.GetMin();
	o_Max = alloc.GetMax();

	o_OBB = new cOBB;
	o_OBB2 = new cOBB;
	o_OBB3 = new cOBB;
	o_OBB4 = new cOBB;
	o_OBB5 = new cOBB;
	/*o_OBB->Setup(o_Min, o_Max);*/
	LoadObb(Num);

	if (m_pRootBone)
		SetupBoneMatrixPtrs(m_pRootBone);
}

void cObjectXfile::LoadObb(int PNum)
{
	if (PNum == 0) o_OBB->Setup(D3DXVECTOR3(o_Min.x, o_Min.y, o_Min.z + 300), D3DXVECTOR3(o_Max.x - 200, o_Max.y, o_Max.z));
	else if (PNum == 1) o_OBB->Setup(D3DXVECTOR3(o_Min.x, o_Min.y, o_Min.z + 800), D3DXVECTOR3(o_Max.x, o_Max.y, o_Max.z - 800));
	else if (PNum == 7) o_OBB->Setup(D3DXVECTOR3(o_Min.x, o_Min.y, o_Min.z + 300), D3DXVECTOR3(o_Max.x - 200, o_Max.y, o_Max.z));
	else if (PNum == 11) o_OBB->Setup(D3DXVECTOR3(o_Min.x + 600, o_Min.y, o_Min.z + 500), D3DXVECTOR3(o_Max.x - 700, o_Max.y, o_Max.z - 300));
	else if (PNum == 12) o_OBB->Setup(D3DXVECTOR3(o_Min.x + 1000, o_Min.y, o_Min.z + 1100), D3DXVECTOR3(o_Max.x - 1100, o_Max.y, o_Max.z - 1100));
	else if (PNum == 13) o_OBB->Setup(D3DXVECTOR3(o_Min.x + 300, o_Min.y, o_Min.z), D3DXVECTOR3(o_Max.x - 300, o_Max.y, o_Max.z));
	else if (PNum == 14) o_OBB->Setup(D3DXVECTOR3(o_Min.x + 1000, o_Min.y, o_Min.z + 1300), D3DXVECTOR3(o_Max.x - 1000, o_Max.y, o_Max.z - 1800));
	else if (PNum == 15) o_OBB->Setup(D3DXVECTOR3(o_Min.x + 200, o_Min.y, o_Min.z + 200), D3DXVECTOR3(o_Max.x -200, o_Max.y, o_Max.z - 200));
	else if (PNum == 52) o_OBB->Setup(D3DXVECTOR3(o_Min.x, o_Min.y, o_Min.z + 30), D3DXVECTOR3(o_Max.x, o_Max.y, o_Max.z - 30));
	else if (PNum == 53) o_OBB->Setup(D3DXVECTOR3(o_Min.x, o_Min.y, o_Min.z + 30), D3DXVECTOR3(o_Max.x, o_Max.y, o_Max.z - 30));
	else if (PNum == 54) o_OBB->Setup(D3DXVECTOR3(o_Min.x, o_Min.y, o_Min.z + 1100), D3DXVECTOR3(o_Max.x - 450, o_Max.y - 50, o_Max.z - 30));
	else if (PNum == 55)
	{
		o_OBB->Setup(D3DXVECTOR3(30, -30, -20), D3DXVECTOR3(200, 200, 180));
		o_OBB2->Setup(D3DXVECTOR3(-200, -30, -20), D3DXVECTOR3(-30, 200, 180));
		o_OBB3->Setup(D3DXVECTOR3(-200, 150, -20), D3DXVECTOR3(200, 300, 180));
	}
	else if (PNum == 57)
	{
		o_OBB2->Setup(D3DXVECTOR3(180, 0, -320), D3DXVECTOR3(250, 200, 0));
		o_OBB3->Setup(D3DXVECTOR3(-50, 0, -320), D3DXVECTOR3(0, 200, 0));
		o_OBB4->Setup(D3DXVECTOR3(-280, 0, 100), D3DXVECTOR3(200, 200, 200));
		o_OBB5->Setup(D3DXVECTOR3(-280, 0, -100), D3DXVECTOR3(0, 200, -30));
	}
	else if (PNum == 26 || PNum == 27 || PNum == 28 || PNum == 29 || PNum == 30 || PNum == 31 || PNum == 32 || PNum == 33 || PNum == 34 || PNum == 35 || PNum == 36);
	else o_OBB->Setup(o_Min, o_Max);
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
		o_OBB->Update(&GetWorldTM());
		o_OBB2->Update(&GetWorldTM());
		o_OBB3->Update(&GetWorldTM());
		o_OBB4->Update(&obb4mat);
		o_OBB5->Update(&obb4mat);

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
			D3DXMATRIXA16 matS, matRx, matRy, matRz, matT, obb4rot;
			D3DXVECTOR3 thisPos = this->GetPosition();
			D3DXMatrixScaling(&matS, m_vScale.x, m_vScale.y, m_vScale.z);
			D3DXMatrixRotationX(&matRx, m_vRotateX);
			D3DXMatrixRotationY(&matRy, m_vRotateY);
			D3DXMatrixRotationZ(&matRz, m_vRotateZ);
			D3DXMatrixTranslation(&matT, thisPos.x, thisPos.y, thisPos.z);

			m_matWorld = matS * matRx * matRy * matRz * matT;

			D3DXMatrixRotationY(&obb4rot, D3DX_PI / 3);
			obb4mat = matS * matRx * matRy * matRz * obb4rot * matT;

			g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
			g_pD3DDevice->SetMaterial(&pBoneMesh->vecMtlTex[i]->GetMtl());
			g_pD3DDevice->SetTexture(0, pBoneMesh->vecMtlTex[i]->GetTexture());
			pBoneMesh->pOrigMesh->DrawSubset(i);
			g_pD3DDevice->SetTexture(0, NULL);
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

	o_OBB->DebugRender(D3DCOLOR_XRGB(255, 255, 0));
	o_OBB2->DebugRender(D3DCOLOR_XRGB(255, 255, 0));
	o_OBB3->DebugRender(D3DCOLOR_XRGB(255, 255, 0));
	o_OBB4->DebugRender(D3DCOLOR_XRGB(255, 255, 0));
	o_OBB5->DebugRender(D3DCOLOR_XRGB(255, 255, 0));
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
