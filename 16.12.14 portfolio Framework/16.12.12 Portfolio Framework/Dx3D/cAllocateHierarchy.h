#pragma once

//typedef struct _D3DXFRAME {
//	LPTSTR Name;                            // 프레임 이름 
//	D3DXMATRIX TransformationMatrix;        // 변환 행렬 정보 
//	LPD3DXMESHCONTAINER pMeshContainer;     // 메시 컨테이너 포인터 
//	struct _D3DXFRAME *pFrameSibling;       // 형제 프레임의 포인터 
//	struct _D3DXFRAME *pFrameFirstChild;    // 자식 프레임의 포인터 
//} D3DXFRAME, *LPD3DXFRAME;

struct D3DXFRAME_DERIVED : public D3DXFRAME
{
	D3DXMATRIXA16 CombinedTransformationMatrix;
};
//typedef struct _D3DXMESHCONTAINER {
//	LPTSTR Name;                      // 메시의 이름 
//	D3DXMESHDATA MeshData;            // 메시 데이터 구조체 
//	LPD3DXMATERIAL pMaterials;        // 메시의 메터리얼 정보 구조체의 포인터 
//	LPD3DXEFFECTINSTANCE pEffects;    // EffectInstance 구조체의 포인터 
//	DWORD NumMaterials;               // 메터리얼 갯수 
//	DWORD *pAdjacency;                // 인접정보를 갖고 있는 DWORD형의 삼각형 하나당 3개의 배열의 포인터.(이게 뭐지?-_-) 
//	LPD3DXSKININFO pSkinInfo;         // 스킨 정보를 가지고 있는 ID3DXSKININFO 인터페이스의 포인터 
//	struct _D3DXMESHCONTAINER *pNextMeshContainer;  // 링크드 리스트.. 
//} D3DXMESHCONTAINER, *LPD3DXMESHCONTAINER;
struct D3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER
{
	vector<LPDIRECT3DTEXTURE9> vecTexture;
	vector<D3DMATERIAL9>		vecMaterial;

	LPD3DXMESH				pWorkingMesh;			// 작업메쉬
	LPD3DXMESH				pOrigMesh;				// 원본 메쉬 CloneMeshFVF로 복사

	D3DXMATRIXA16**			ppBoneMatrixPtrs;		// 이 메쉬에 영향을 주는 프레임'들'의 월드매트릭스 포인터 배열
	D3DXMATRIXA16*			pBoneOffsetMatrices;	// 원본 메쉬를 로컬스페이스로 보내는 매트릭스들.

	DWORD					dwNumPaletteEntries;
	DWORD					dwMaxNumFaceInfls;
	DWORD					dwNumAttrGroups;
	LPD3DXBUFFER			pBufBoneCombos;
};

class cAllocateHierarchy : public ID3DXAllocateHierarchy
{
public:
	cAllocateHierarchy();
	~cAllocateHierarchy();

	SYNTHESIZE(std::string, m_sDirectory, Directory);
	SYNTHESIZE(ST_SPHERE, m_stSphere, BoundingSphere);
	SYNTHESIZE(DWORD, m_dwDefaultPaletteSize, DefaultPaletteSize);
	SYNTHESIZE(DWORD, m_dwMaxPaletteSize, MaxPaletteSize);

	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);
	STDMETHOD(CreateMeshContainer)(THIS_
		LPCSTR Name,
		CONST D3DXMESHDATA *pMeshData,
		CONST D3DXMATERIAL *pMaterials,
		CONST D3DXEFFECTINSTANCE *pEffectInstances,
		DWORD NumMaterials,
		CONST DWORD *pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER *ppNewMeshContainer);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree);
};

