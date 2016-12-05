#include "StdAfx.h"
#include "cAllocateHierarchy.h"


cAllocateHierarchy::cAllocateHierarchy()
	: m_sDirectory("")
	, m_dwDefaultPaletteSize(0)
	, m_dwMaxPaletteSize(0)
{
}


cAllocateHierarchy::~cAllocateHierarchy()
{
}

STDMETHODIMP cAllocateHierarchy::CreateFrame(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	D3DXFRAME_DERIVED* pBone = new D3DXFRAME_DERIVED;

	pBone->Name = NULL;
	//이름값이 있어야 이름을 찾을수있음
	if (Name) {
		int nLen = strlen(Name) + 1;
		pBone->Name = new char[nLen];
		strcpy(pBone->Name, Name);
	}

	pBone->pFrameFirstChild = NULL;
	pBone->pFrameSibling = NULL;
	pBone->pMeshContainer = NULL;

	D3DXMatrixIdentity(&pBone->TransformationMatrix);
	D3DXMatrixIdentity(&pBone->CombinedTransformationMatrix);

	*ppNewFrame = pBone;

	return S_OK;
}

STDMETHODIMP cAllocateHierarchy::CreateMeshContainer(THIS_ LPCSTR Name,
	CONST D3DXMESHDATA *pMeshData,
	CONST D3DXMATERIAL *pMaterials,
	CONST D3DXEFFECTINSTANCE *pEffectInstances,
	DWORD NumMaterials,
	CONST DWORD *pAdjacency,
	LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
	D3DXMESHCONTAINER_DERIVED* pBoneMesh = new D3DXMESHCONTAINER_DERIVED;

	// 이름 무시
	pBoneMesh->Name = NULL;

	// 재질, 텍스쳐 정보 복사
	for (DWORD i = 0; i < NumMaterials; ++i)
	{
		// 재질정보 저장
		pBoneMesh->vecMaterial.push_back(pMaterials[i].MatD3D);
		
		if (pMaterials[i].pTextureFilename)
		{
			std::string sFilename(pMaterials[i].pTextureFilename);
			std::string sFullPath = m_sDirectory + sFilename;
			LPDIRECT3DTEXTURE9 pTex = g_pTextureManager->GetTexture(sFullPath);
			pBoneMesh->vecTexture.push_back(pTex);
		}
	}

	// 이펙트 무시
	pBoneMesh->pEffects = NULL;

	// 인접정보 복사
	if (pAdjacency)
	{
		DWORD dwNumFaces = pMeshData->pMesh->GetNumFaces();
		pBoneMesh->pAdjacency = new DWORD[3 * dwNumFaces];
		memcpy(pBoneMesh->pAdjacency, pAdjacency, 3 * sizeof(DWORD) * dwNumFaces);
	}

	// pSkinInfo 저장
	// 
	//pSkinInfo->AddRef();
	SAFE_ADD_REF(pSkinInfo);
	pBoneMesh->pSkinInfo = pSkinInfo;

	// pMeshData->pMesh를 원본 메쉬에 복사
	if (pMeshData && pMeshData->pMesh)
	{
		LPVOID pV = NULL;
		pMeshData->pMesh->LockVertexBuffer(0, &pV);
		D3DXComputeBoundingSphere((D3DXVECTOR3*)pV,
			pMeshData->pMesh->GetNumVertices(),
			D3DXGetFVFVertexSize(pMeshData->pMesh->GetFVF()),
			&(m_stSphere.vCenter),
			&(m_stSphere.fRadius));
		pMeshData->pMesh->UnlockVertexBuffer();

		pMeshData->pMesh->CloneMeshFVF(
			pMeshData->pMesh->GetOptions(),
			pMeshData->pMesh->GetFVF(),
			g_pD3DDevice,
			&pBoneMesh->pOrigMesh);
	}

	if (pSkinInfo)
	{
		// pSkinInfo->GetNumBones()를 통해 영향력을 미치는 모든 본에 대한 매트릭스 들을 세팅
		// ppBoneMatrixPtrs, pBoneOffsetMatrices를 동적할당
		DWORD dwNumBones = pSkinInfo->GetNumBones();
		pBoneMesh->ppBoneMatrixPtrs = new D3DXMATRIXA16*[dwNumBones];
		pBoneMesh->pBoneOffsetMatrices = new D3DXMATRIXA16[dwNumBones];

		// 동적 할당된 pBoneOffsetMatrices 매트릭스에 값 저장.
		// pSkinInfo->GetBoneOffsetMatrix(i)
		for (DWORD i = 0; i < dwNumBones; ++i)
		{
			pBoneMesh->ppBoneMatrixPtrs[i] = NULL;
			pBoneMesh->pBoneOffsetMatrices[i] = *(pSkinInfo->GetBoneOffsetMatrix(i));
		}

		// ConvertToIndexedBlendedMesh 함수에 팔렛트 엔트리 개수를 넘겨야하기 때문에 
		// 멤버변수로 세팅하고 외부에서 적용해주도록 설정.
		// 메쉬에 영향을 주는 팔레트의 개수는 
		// m_dwDefaultPaletteSize와 pSkinInfo->GetNumBones()보다 클 수 없다.
		// m_dwDefaultPaletteSize는 쉐이더에 정의되어있다.
		pBoneMesh->dwNumPaletteEntries = min(m_dwDefaultPaletteSize, pSkinInfo->GetNumBones());

		// 최대 팔레트 사이즈 업데이트
		if (m_dwMaxPaletteSize < pBoneMesh->dwNumPaletteEntries)
		{
			m_dwMaxPaletteSize = pBoneMesh->dwNumPaletteEntries;
		}

		// blend weights와 인덱스를 이용해 pWorkingMesh 생성.
		pBoneMesh->pSkinInfo->ConvertToIndexedBlendedMesh(
			pBoneMesh->pOrigMesh,
			D3DXMESH_MANAGED | D3DXMESHOPT_VERTEXCACHE,
			pBoneMesh->dwNumPaletteEntries,
			pBoneMesh->pAdjacency,
			NULL,
			NULL,
			NULL,
			&pBoneMesh->dwMaxNumFaceInfls,
			&pBoneMesh->dwNumAttrGroups,
			&pBoneMesh->pBufBoneCombos,
			&pBoneMesh->pWorkingMesh);

		// 기존 메쉬의 FVF가 버텍스 블랜딩 정보를 전달 할 수 없는 경우 FVF를 변경한다
		DWORD dwOldFVF = pBoneMesh->pWorkingMesh->GetFVF();
		DWORD dwNewFVF =
			(dwOldFVF & D3DFVF_POSITION_MASK) |
			D3DFVF_NORMAL |
			D3DFVF_TEX1 |
			D3DFVF_LASTBETA_UBYTE4;

		if (dwNewFVF != dwOldFVF)
		{
			// 메쉬를 복사하면서 FVF를 변경한다.
			LPD3DXMESH pMesh = NULL;

			pBoneMesh->pWorkingMesh->CloneMeshFVF(
				pBoneMesh->pWorkingMesh->GetOptions(),
				dwNewFVF,
				g_pD3DDevice,
				&pMesh);

			// 기존 메쉬를 대체
			SAFE_RELEASE(pBoneMesh->pWorkingMesh);
			pBoneMesh->pWorkingMesh = pMesh;

			// 새로 생성한 메쉬에는 노멀 정보가 있으므로
			// 기존 메쉬 FVF에 노멀 정보가 없으면 새로 계산해줘야함.
			if (!(dwOldFVF & D3DFVF_NORMAL))
			{
				D3DXComputeNormals(pBoneMesh->pWorkingMesh, NULL);
			}
		}

		// Interpret the UBYTE4 as a D3DCOLOR.
		// The GeForce3 doesn't support the UBYTE4 decl type.  So, we convert any
		// blend indices to a D3DCOLOR semantic, and later in the shader convert
		// it back using the D3DCOLORtoUBYTE4() intrinsic.  Note that we don't
		// convert any data, just the declaration.
		D3DVERTEXELEMENT9 pDecl[MAX_FVF_DECL_SIZE];
		D3DVERTEXELEMENT9 * pDeclCur;
		pBoneMesh->pWorkingMesh->GetDeclaration(pDecl);

		pDeclCur = pDecl;
		while (pDeclCur->Stream != 0xff)
		{
			if ((pDeclCur->Usage == D3DDECLUSAGE_BLENDINDICES) &&
				(pDeclCur->UsageIndex == 0))
				pDeclCur->Type = D3DDECLTYPE_D3DCOLOR;
			pDeclCur++;
		}

		pBoneMesh->pWorkingMesh->UpdateSemantics(pDecl);
	}

	*ppNewMeshContainer = pBoneMesh;

	return S_OK;
}
STDMETHODIMP cAllocateHierarchy::DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree)
{
	D3DXFRAME_DERIVED* pBone = (D3DXFRAME_DERIVED*)pFrameToFree;
	SAFE_DELETE_ARR(pBone->Name);
	delete pFrameToFree;

	return S_OK;
}
STDMETHODIMP cAllocateHierarchy::DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	D3DXMESHCONTAINER_DERIVED *pBoneMesh = (D3DXMESHCONTAINER_DERIVED *)pMeshContainerToFree;
	SAFE_RELEASE(pBoneMesh->pSkinInfo);
	SAFE_RELEASE(pBoneMesh->pWorkingMesh);
	SAFE_RELEASE(pBoneMesh->pOrigMesh);
	SAFE_RELEASE(pBoneMesh->pBufBoneCombos);

	SAFE_DELETE_ARR(pBoneMesh->Name);
	SAFE_DELETE_ARR(pBoneMesh->pAdjacency);
	SAFE_DELETE_ARR(pBoneMesh->ppBoneMatrixPtrs);
	SAFE_DELETE_ARR(pBoneMesh->pBoneOffsetMatrices);

	SAFE_DELETE(pMeshContainerToFree);
	return S_OK;
}
