#include "stdafx.h"
#include "cTerrain.h"


cTerrain::cTerrain() :fCellScale(0)
, m_pkTexBmp(NULL)
{
}


cTerrain::~cTerrain()
{
}

//초기화


HRESULT cTerrain::Heightmap(char *szTexFile, char *szTexFile1, float cellSize)
{
	if (FAILED(D3DXCreateTextureFromFileEx(g_pD3DDevice,
		szTexFile, D3DX_DEFAULT, D3DX_DEFAULT,
		D3DX_DEFAULT, 0,
		D3DFMT_X8R8G8B8, D3DPOOL_MANAGED,
		D3DX_DEFAULT, D3DX_DEFAULT, 0,
		NULL, NULL, &m_pkTexture
	))) {
		MessageBox(NULL, "Can't not open the texture file.", "Error", MB_OK);
		return E_FAIL;
	}
	if (m_pkTexBmp == NULL)
	{
		m_pkTexBmp = g_pTextureManager->GetTexture(szTexFile1);
		SAFE_ADD_REF(m_pkTexBmp);
	}

	D3DSURFACE_DESC ddsd;
	// 텍스처 정보를 읽어온다.
	m_pkTexture->GetLevelDesc(0, &ddsd);

	// 텍스처의 가로, 세로 크기를 저장한다.
	this->nVerNumX = ddsd.Width;
	this->nVerNumZ = ddsd.Height;

	this->nTotalVerNum = this->nVerNumX * this->nVerNumZ;

	//셀수
	this->nCellNumX = this->nVerNumX - 1;
	this->nCellNumZ = this->nVerNumZ - 1;
	this->nTotalCellNum = this->nCellNumX * this->nCellNumZ;

	//셀사이즈
	this->fCellScale = cellSize;

	//폴리곤 갯수
	this->nTotalTri = this->nTotalCellNum * 2;

	//터레인 사이즈
	this->fTerrainSizeX = this->nCellNumX * this->fCellScale;
	this->fTerrainSizeZ = this->nCellNumZ * this->fCellScale;

	D3DLOCKED_RECT d3drc;

	m_pkTexture->LockRect(0, &d3drc, NULL, D3DLOCK_READONLY);
	vertexPoses = new D3DXVECTOR3[nTotalVerNum];

	//시작 지점은 일단 0. 0. 0  위치로
	D3DXVECTOR3 startPos(0, 0, 0);

	DWORD dwRowPixelSize = d3drc.Pitch / 4;

	for (int z = 0; z < this->nVerNumZ; z++)
	{
		for (int x = 0; x < this->nVerNumX; x++)
		{
			//정점인덱스
			int idx = z * this->nVerNumZ + x;

			vertexPoses[idx].x = startPos.x + (x * fCellScale);
			vertexPoses[idx].z = startPos.z + (z * fCellScale);
			DWORD dwPixel = *((LPDWORD)d3drc.pBits + z + x * dwRowPixelSize);
			y = (float)(dwPixel & 0x000000ff) / 3.f;
			vertexPoses[idx].y = y;

			vVertex.push_back(vertexPoses);
		}
	}
	m_pkTexture->UnlockRect(0);
	m_pkTexture->Release();

	vector<DWORD> vecIndex;
	//
	// 정점인덱스
	//
	pIndices = new TERRAININDEX[this->nTotalTri];

	for (int z = 0; z < this->nCellNumZ; z++)
	{
		//수평라인
		int nowZ = z;
		int nextZ = z + 1;

		for (int x = 0; x < this->nCellNumX; x++)
		{
			//현제 수직라인
			int nowX = x;
			int nextX = x + 1;

			//  1-----2
			//  |    /|
			//  |   / |
			//  |  /  |
			//  | /   |
			//  |/    |
			//  0-----3

			//		1--3
			//		|\ |
			//		| \|
			//		0--2
			//모서리 정점 인덱스
			DWORD lt = nextZ * this->nVerNumX + nowX;
			DWORD rt = nextZ * this->nVerNumX + nextX;
			DWORD lb = nowZ * this->nVerNumX + nowX;
			DWORD rb = nowZ * this->nVerNumX + nextX;



			vecIndex.push_back(lb); vecIndex.push_back(lt); vecIndex.push_back(rb);
			vecIndex.push_back(rt); vecIndex.push_back(rb); vecIndex.push_back(lt);


			//Tri 인덱스 배열 인덱스
			//int idx = (z * this->nCellNumX + x) * 2;
			//
			//
			////Cell 폴리곤 1
			////lb, lt, rt, 
			//pIndices[idx]._0 = lb;
			//pIndices[idx]._1 = lt;
			//pIndices[idx]._2 = rt;
			//vIndex.push_back(pIndices);
			////Cell 폴리곤 1
			////lb, rt, rb, 
			//pIndices[idx + 1]._0 = lb;
			//pIndices[idx + 1]._1 = rt;
			//pIndices[idx + 1]._2 = rb;
			//vIndex.push_back(pIndices);
		}
	}
	D3DVERTEXELEMENT9 vertElement[] = {
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		D3DDECL_END()
	};

	g_pD3DDevice->CreateVertexDeclaration(
		vertElement,         //앞에서 만든 D3DVERTEXELEMENT9 배열 포인터
		&this->terrainDecl         //얻어올 LPDIRECT3DVERTEXDECLARATION9 포인터
	);

	//
	// 정점 버퍼를 만든다.
	//
	if (FAILED(g_pD3DDevice->CreateVertexBuffer(
		sizeof(TERRAINVERTEX) *	this->nTotalVerNum,
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_DEFAULT,
		&this->terrainVb,
		0))) return E_FAIL;

	LPTERRAINVERTTEX pVertices = NULL;

	if (SUCCEEDED(this->terrainVb->Lock(0, 0, (void**)&pVertices, 0)))
	{
		for (int i = 0; i < this->nTotalVerNum; i++)
		{
			//정점 위치 대입
			pVertices[i].pos = this->vertexPoses[i];
		}
		this->terrainVb->Unlock();
	}

	//
	// 인덱스 버퍼
	//
	if (FAILED(g_pD3DDevice->CreateIndexBuffer(
		sizeof(TERRAININDEX) * this->nTotalTri,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX32,
		D3DPOOL_DEFAULT,
		&this->terrainIb,
		NULL))) return E_FAIL;

	void* pI = NULL;

	if (SUCCEEDED(this->terrainIb->Lock(0, 0, &pI, 0)))
	{
		memcpy(pI, &vecIndex[0], this->nTotalTri * sizeof(TERRAININDEX));

		this->terrainIb->Unlock();
	}
	m_stMtl.Ambient = m_stMtl.Diffuse = m_stMtl.Specular = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);

	return S_OK;
}
//해제
void cTerrain::Release()
{
	SAFE_DELETE_ARR(this->vertexPoses);
	SAFE_DELETE_ARR(this->pIndices);

	SAFE_RELEASE(this->terrainIb);
	SAFE_RELEASE(this->terrainVb);
}

bool cTerrain::GetHeight(IN float x, OUT float& y, IN float z)
{
	if (x < 0 || z < 0 || x > this->nCellNumZ + 1 || z > this->nCellNumZ + 1)
	{
		return false;
	}

	int nX = x;
	int nZ = z;

	float fDeltaX = x - nX;
	float fDeltaZ = z - nZ;

	int _0 = (nZ + 0) * (nCellNumZ + 1) + nX;
	int _1 = (nZ + 1) * (nCellNumZ + 1) + nX;
	int _2 = (nZ + 0) * (nCellNumZ + 1) + nX + 1;
	int _3 = (nZ + 1) * (nCellNumZ + 1) + nX + 1;

	if (fDeltaX + fDeltaZ < 1)
	{
		D3DXVECTOR3 v01 = this->vertexPoses[_1] - this->vertexPoses[_0];
		D3DXVECTOR3 v02 = this->vertexPoses[_2] - this->vertexPoses[_0];
		v01 *= fDeltaZ;
		v02 *= fDeltaX;
		y = (v01 + v02).y + this->vertexPoses[_0].y;
	}
	else
	{
		fDeltaX = 1 - fDeltaX;
		fDeltaZ = 1 - fDeltaZ;
		D3DXVECTOR3 v31 = this->vertexPoses[_1] - this->vertexPoses[_3];
		D3DXVECTOR3 v32 = this->vertexPoses[_2] - this->vertexPoses[_3];
		v31 *= fDeltaX;
		v32 *= fDeltaZ;
		y = (v31 + v32).y + this->vertexPoses[_3].y;
	}

	return true;
}

float cTerrain::GetHeight2(IN float x, IN float z)
{
	return 0.0f;
}

void cTerrain::Render()
{
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//월드 행렬 일단 초기화
	D3DXMATRIXA16 matWorld, matY;


	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matY);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pD3DDevice->SetMaterial(&m_stMtl);
	g_pD3DDevice->SetTexture(0, m_pkTexBmp);
	g_pD3DDevice->SetFVF(TERRAINVERTEX::FVF);
	g_pD3DDevice->SetStreamSource(0, this->terrainVb, 0, sizeof(TERRAINVERTEX));
	g_pD3DDevice->SetIndices(this->terrainIb);
	g_pD3DDevice->SetVertexDeclaration(this->terrainDecl);
	g_pD3DDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,
		0,
		0,
		this->nTotalVerNum,
		0,
		this->nTotalTri);

	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}
