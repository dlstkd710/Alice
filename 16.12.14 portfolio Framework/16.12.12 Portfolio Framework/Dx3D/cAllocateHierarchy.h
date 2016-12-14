#pragma once

//typedef struct _D3DXFRAME {
//	LPTSTR Name;                            // ������ �̸� 
//	D3DXMATRIX TransformationMatrix;        // ��ȯ ��� ���� 
//	LPD3DXMESHCONTAINER pMeshContainer;     // �޽� �����̳� ������ 
//	struct _D3DXFRAME *pFrameSibling;       // ���� �������� ������ 
//	struct _D3DXFRAME *pFrameFirstChild;    // �ڽ� �������� ������ 
//} D3DXFRAME, *LPD3DXFRAME;

struct D3DXFRAME_DERIVED : public D3DXFRAME
{
	D3DXMATRIXA16 CombinedTransformationMatrix;
};
//typedef struct _D3DXMESHCONTAINER {
//	LPTSTR Name;                      // �޽��� �̸� 
//	D3DXMESHDATA MeshData;            // �޽� ������ ����ü 
//	LPD3DXMATERIAL pMaterials;        // �޽��� ���͸��� ���� ����ü�� ������ 
//	LPD3DXEFFECTINSTANCE pEffects;    // EffectInstance ����ü�� ������ 
//	DWORD NumMaterials;               // ���͸��� ���� 
//	DWORD *pAdjacency;                // ���������� ���� �ִ� DWORD���� �ﰢ�� �ϳ��� 3���� �迭�� ������.(�̰� ����?-_-) 
//	LPD3DXSKININFO pSkinInfo;         // ��Ų ������ ������ �ִ� ID3DXSKININFO �������̽��� ������ 
//	struct _D3DXMESHCONTAINER *pNextMeshContainer;  // ��ũ�� ����Ʈ.. 
//} D3DXMESHCONTAINER, *LPD3DXMESHCONTAINER;
struct D3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER
{
	vector<LPDIRECT3DTEXTURE9> vecTexture;
	vector<D3DMATERIAL9>		vecMaterial;

	LPD3DXMESH				pWorkingMesh;			// �۾��޽�
	LPD3DXMESH				pOrigMesh;				// ���� �޽� CloneMeshFVF�� ����

	D3DXMATRIXA16**			ppBoneMatrixPtrs;		// �� �޽��� ������ �ִ� ������'��'�� �����Ʈ���� ������ �迭
	D3DXMATRIXA16*			pBoneOffsetMatrices;	// ���� �޽��� ���ý����̽��� ������ ��Ʈ������.

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

