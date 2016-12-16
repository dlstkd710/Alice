#pragma once
struct ST_PC_VERTEX
{
	D3DXVECTOR3 p;
	D3DCOLOR	c;

	ST_PC_VERTEX() : p(0, 0, 0), c(D3DCOLOR_XRGB(0, 0, 0)) {}
	ST_PC_VERTEX(D3DXVECTOR3 _p, D3DCOLOR _c) : p(_p), c(_c) {}

	enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE, };
};

struct ST_PT_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR2	t;

	ST_PT_VERTEX() : p(0, 0, 0), t(0, 0) {}
	ST_PT_VERTEX(D3DXVECTOR3 _p, D3DXVECTOR2 _t) : p(_p), t(_t) {}

	enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1, };
};

struct ST_PNT_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
	D3DXVECTOR2	t;

	ST_PNT_VERTEX() : p(0, 0, 0), n(0, 0, 0), t(0, 0) {}
	ST_PNT_VERTEX(D3DXVECTOR3 _p, D3DXVECTOR3 _n, D3DXVECTOR2 _t) : p(_p), n(_n), t(_t) {}

	enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1, };
};

struct ST_POS_SAMPLE
{
	int				n;
	D3DXVECTOR3		p;
};

struct ST_ROT_SAMPLE
{
	int				n;
	D3DXQUATERNION	q;
};

struct MYINDEX
{
	DWORD _0, _1, _2;
};

struct ST_SPHERE
{
	D3DXVECTOR3 vCenter;
	float		fRadius;
	bool		isPicked;

	ST_SPHERE() : vCenter(0, 0, 0), fRadius(0.0f), isPicked(false) {}
	ST_SPHERE(D3DXVECTOR3 _vCenter, float _fRadius, bool _isPicked)
		: vCenter(_vCenter), fRadius(_fRadius), isPicked(_isPicked) {}
};

struct ST_SIZE
{
	float fWidth;
	float fHeight;

	ST_SIZE() : fWidth(0), fHeight(0) {}
	ST_SIZE(float W, float H) : fWidth(W), fHeight(H) {}
};

enum MonsterNum
{
	NIGHTMARE,
	FIREFLY,
	BOSS
};
struct ST_PCT_VERTEX
{
	D3DXVECTOR3 p;
	D3DCOLOR	c;
	D3DXVECTOR2	t;

	ST_PCT_VERTEX() : p(0, 0, 0), c(D3DCOLOR_XRGB(255, 255, 255)), t(0, 0) {}
	ST_PCT_VERTEX(D3DXVECTOR3 _p, D3DCOLOR _c, D3DXVECTOR2 _t) : p(_p), c(_c), t(_t) {}

	enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1, };
};
enum eState
{
	E_STATE_NORMAL,
	E_STATE_JUMPUP,
	E_STATE_JUMPDOUBLE,
	E_STATE_TELPO,
	E_STATE_JUMPDOWN,
	E_STATE_1COMBO,
	E_STATE_2COMBO,
	E_STATE_3COMBO,
	E_STATE_IDEL,
	E_STATE_GIDEL,
	E_STATE_HAMMOR,
	E_STATE_H1COMBO,
	E_STATE_H2COMBO,
	E_STATE_H3COMBO,
	E_STATE_GRUN
};

class cMtlTex;

struct ST_BONE : public D3DXFRAME
{
	D3DXMATRIXA16	CombinedTransformationMatrix;
};

struct ST_BONE_MESH : public D3DXMESHCONTAINER
{

	std::vector<LPDIRECT3DTEXTURE9> vecTexture;
	std::vector<D3DMATERIAL9>		vecMaterial;

	LPD3DXMESH				pWorkingMesh;			// �۾��޽�
	LPD3DXMESH				pOrigMesh;				// ���� �޽� CloneMeshFVF�� ����
	D3DXMATRIXA16**			ppBoneMatrixPtrs;		// �� �޽��� ������ �ִ� ������'��'�� �����Ʈ���� ������ �迭
	D3DXMATRIXA16*			pBoneOffsetMatrices;	// ���� �޽��� ���ý����̽��� ������ ��Ʈ������.

	DWORD					dwNumPaletteEntries;
	DWORD					dwMaxNumFaceInfls;
	DWORD					dwNumAttrGroups;
	LPD3DXBUFFER			pBufBoneCombos;

	//LPD3DXMESH				pOrigMesh;
	LPD3DXMESH				pWorkMesh;
	std::vector<cMtlTex*>	vecMtlTex;

	// �� �޽��� ������ �ִ� ������'��'�� �����Ʈ���� ������ �迭
	//D3DXMATRIX**			ppBoneMatrixPtrs;
	// ���� �޽��� ���ý����̽��� ������ ��Ʈ������.
	//D3DXMATRIX*				pBoneOffsetMatrices;
	// �� ���� ���� �����Ʈ����
	D3DXMATRIXA16*				pCurrentBoneMatrices;
};
