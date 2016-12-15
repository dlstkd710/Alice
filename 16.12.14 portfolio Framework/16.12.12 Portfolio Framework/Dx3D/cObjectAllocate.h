#pragma once

class cMtlTex;
//
//struct ST_BONE : public D3DXFRAME
//{
//	D3DXMATRIX	CombinedTransformationMatrix;
//};
//
//struct ST_BONE_MESH : public D3DXMESHCONTAINER
//{
//	LPD3DXMESH				pOrigMesh;
//	LPD3DXMESH				pWorkMesh;
//	std::vector<cMtlTex*>	vecMtlTex;
//
//	// 이 메쉬에 영향을 주는 프레임'들'의 월드매트릭스 포인터 배열
//	D3DXMATRIX**			ppBoneMatrixPtrs;
//	// 원본 메쉬를 로컬스페이스로 보내는 매트릭스들.
//	D3DXMATRIX*				pBoneOffsetMatrices;
//	// 각 본의 계산된 월드매트릭스
//	D3DXMATRIX*				pCurrentBoneMatrices;
//};

class cObjectAllocate : public ID3DXAllocateHierarchy
{
private:
	SYNTHESIZE(std::string, m_sFolder, Folder);
	SYNTHESIZE(D3DXVECTOR3, cMax, Max);
	SYNTHESIZE(D3DXVECTOR3, cMin, Min);

public:
	cObjectAllocate(void);
	~cObjectAllocate(void);

	// ID3DXAllocateHierarchy

	//------------------------------------------------------------------------
	// CreateFrame:
	// ------------
	// Requests allocation of a frame object.
	//
	// Parameters:
	//  Name
	//		Name of the frame to be created
	//	ppNewFrame
	//		Returns the created frame object
	//
	//------------------------------------------------------------------------
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name,
		LPD3DXFRAME *ppNewFrame) override;

	//------------------------------------------------------------------------
	// CreateMeshContainer:
	// --------------------
	// Requests allocation of a mesh container object.
	//
	// Parameters:
	//  Name
	//		Name of the mesh
	//	pMesh
	//		Pointer to the mesh object if basic polygon data found
	//	pPMesh
	//		Pointer to the progressive mesh object if progressive mesh data found
	//	pPatchMesh
	//		Pointer to the patch mesh object if patch data found
	//	pMaterials
	//		Array of materials used in the mesh
	//	pEffectInstances
	//		Array of effect instances used in the mesh
	//	NumMaterials
	//		Num elements in the pMaterials array
	//	pAdjacency
	//		Adjacency array for the mesh
	//	pSkinInfo
	//		Pointer to the skininfo object if the mesh is skinned
	//	pBoneNames
	//		Array of names, one for each bone in the skinned mesh. 
	//		The numberof bones can be found from the pSkinMesh object
	//	pBoneOffsetMatrices
	//		Array of matrices, one for each bone in the skinned mesh.
	//
	//------------------------------------------------------------------------
	STDMETHOD(CreateMeshContainer)(THIS_
		LPCSTR Name,
		CONST D3DXMESHDATA *pMeshData,
		CONST D3DXMATERIAL *pMaterials,
		CONST D3DXEFFECTINSTANCE *pEffectInstances,
		DWORD NumMaterials,
		CONST DWORD *pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER *ppNewMeshContainer) override;

	//------------------------------------------------------------------------
	// DestroyFrame:
	// -------------
	// Requests de-allocation of a frame object.
	//
	// Parameters:
	//  pFrameToFree
	//		Pointer to the frame to be de-allocated
	//
	//------------------------------------------------------------------------
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree) override;

	//------------------------------------------------------------------------
	// DestroyMeshContainer:
	// ---------------------
	// Requests de-allocation of a mesh container object.
	//
	// Parameters:
	//  pMeshContainerToFree
	//		Pointer to the mesh container object to be de-allocated
	//
	//------------------------------------------------------------------------
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree) override;
};

