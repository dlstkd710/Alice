#include "StdAfx.h"
#include "cHeightMap.h"


cHeightMap::cHeightMap(void)
	: m_pVB(NULL)
	, m_pIB(NULL)
	, m_nNumTri(0)
	, m_nNumVertex(0)
	, m_pTexture(NULL)
{
	ZeroMemory(&m_stMtl, sizeof(D3DMATERIAL9));
}


cHeightMap::~cHeightMap(void)
{
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);
	SAFE_RELEASE(m_pTexture);
}

void cHeightMap::Load( char* szFilename, char* szTexture, int nBytesPerPixel )
{
	if (m_pTexture == NULL)
	{
		m_pTexture = g_pTextureManager->GetTexture(szTexture);
		SAFE_ADD_REF(m_pTexture);
	}

	FILE* fp;
	fopen_s(&fp, szFilename, "rb");

	fseek(fp, 0, SEEK_END);
	int nFileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	int nPixel = nFileSize / nBytesPerPixel;
	
	int nRow = (int)sqrt(nPixel + 0.0001f);
	assert( nPixel == nRow * nRow && "가로 세로 사이즈가 동일한 파일만 로딩이 가능합니다.");

	int nCol = nRow;
	
	std::vector<unsigned char> vecData;
	vecData.reserve(nPixel);

	for (int i = 0; i < nPixel; ++i)
	{
		vecData.push_back(fgetc(fp));
		if(nBytesPerPixel > 1)
			fseek(fp, nBytesPerPixel - 1, SEEK_CUR);
	}

	fclose(fp);

	m_nNumVertex = nPixel;

	int nNumTile = nRow - 1;
	m_nNumTile = nNumTile;

	std::vector<ST_PNT_VERTEX> vecVertex;
	vecVertex.reserve(nPixel);
	m_vecVertex.reserve(nPixel);
	for (int z = 0; z < nRow; ++z)
	{
		for (int x = 0; x < nCol; ++x)
		{
			int nIndex = z * nRow + x;
			ST_PNT_VERTEX v;
			v.p = D3DXVECTOR3(x, vecData[nIndex] / 10.f, z);
			v.n = D3DXVECTOR3(0, 1, 0);
			v.t = D3DXVECTOR2(x / (float)nNumTile, z / (float)nNumTile);
			
			vecVertex.push_back(v);
			m_vecVertex.push_back(v.p);
		}
	}

	for (int z = 1; z < nNumTile; ++z)
	{
		for (int x = 1; x < nNumTile; ++x)
		{
			int nIndex = z * nRow + x;
			D3DXVECTOR3 l = vecVertex[nIndex - 1].p;
			D3DXVECTOR3 r = vecVertex[nIndex + 1].p;
			D3DXVECTOR3 b = vecVertex[nIndex - nRow].p;
			D3DXVECTOR3 u = vecVertex[nIndex + nRow].p;
			D3DXVECTOR3 lr = r - l;
			D3DXVECTOR3 bu = u - b;
			D3DXVECTOR3 n;
			D3DXVec3Cross(&n, &bu, &lr);
			D3DXVec3Normalize(&n, &n);

			vecVertex[nIndex].n = n;
		}
	}

	//		1--3
	//		|\ |
	//		| \|
	//		0--2
	std::vector<DWORD> vecIndex;
	vecIndex.reserve(nNumTile * nNumTile * 3 * 2);

	m_nNumTri = nNumTile * nNumTile * 2;

	for (int z = 0; z < nNumTile; ++z)
	{
		for (int x = 0; x < nNumTile; ++x)
		{
			int _0 = (z + 0) * nRow + x;
			int _1 = (z + 1) * nRow + x;
			int _2 = (z + 0) * nRow + x + 1;
			int _3 = (z + 1) * nRow + x + 1;
			vecIndex.push_back(_0); vecIndex.push_back(_1); vecIndex.push_back(_2);
			vecIndex.push_back(_3); vecIndex.push_back(_2); vecIndex.push_back(_1);
		}
	}

	g_pD3DDevice->CreateVertexBuffer(
		vecVertex.size() * sizeof(ST_PNT_VERTEX),
		0,
		ST_PNT_VERTEX::FVF,
		D3DPOOL_MANAGED,
		&m_pVB,
		NULL);

	LPVOID pV;
	m_pVB->Lock(0, 0, &pV, 0);
	memcpy(pV, &vecVertex[0], vecVertex.size() * sizeof(ST_PNT_VERTEX));
	m_pVB->Unlock();
	
	g_pD3DDevice->CreateIndexBuffer(
		vecIndex.size() * sizeof(DWORD),
		0,
		D3DFMT_INDEX32,
		D3DPOOL_MANAGED,
		&m_pIB,
		NULL);

	LPVOID pI;
	m_pIB->Lock(0, 0, &pI, 0);
	memcpy(pI, &vecIndex[0], vecIndex.size() * sizeof(DWORD));
	m_pIB->Unlock();

	m_stMtl.Ambient = m_stMtl.Diffuse = m_stMtl.Specular = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
}

void cHeightMap::Render()
{
	
	D3DXMATRIXA16 matI;
	D3DXMatrixIdentity(&matI);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matI);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	g_pD3DDevice->SetMaterial(&m_stMtl);
	g_pD3DDevice->SetTexture(0, m_pTexture);
	g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);
	g_pD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(ST_PNT_VERTEX));
	g_pD3DDevice->SetIndices(m_pIB);
	g_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
		0,
		0,
		m_nNumVertex,
		0,
		m_nNumTri);

	//g_pD3DDevice->DrawPrimitiveUP(D3DPT_POINTLIST, m_nNumVertex, &m_vecVertex[0], )
	//g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_nNumTri);
}

bool cHeightMap::GetHeight( IN float x, OUT float& y, IN float z )
{
	if (x < 0 || z < 0 || x > m_nNumTile + 1 || z > m_nNumTile + 1)
	{
		return false;
	}
	
	int nX = x;
	int nZ = z;

	float fDeltaX = x - nX;
	float fDeltaZ = z - nZ;

	//		1--3
	//		|\ |
	//		| \|
	//		0--2
	int _0 = (nZ + 0) * (m_nNumTile + 1) + nX;
	int _1 = (nZ + 1) * (m_nNumTile + 1) + nX;
	int _2 = (nZ + 0) * (m_nNumTile + 1) + nX + 1;
	int _3 = (nZ + 1) * (m_nNumTile + 1) + nX + 1;

	if(fDeltaX + fDeltaZ < 1)
	{
		D3DXVECTOR3 v01 = m_vecVertex[_1] - m_vecVertex[_0];
		D3DXVECTOR3 v02 = m_vecVertex[_2] - m_vecVertex[_0];
		v01 *= fDeltaZ;
		v02 *= fDeltaX;
		y = (v01 + v02).y + m_vecVertex[_0].y;
	}
	else
	{
		fDeltaX = 1 - fDeltaX;
		fDeltaZ = 1 - fDeltaZ;
		D3DXVECTOR3 v31 = m_vecVertex[_1] - m_vecVertex[_3];
		D3DXVECTOR3 v32 = m_vecVertex[_2] - m_vecVertex[_3];
		v31 *= fDeltaX;
		v32 *= fDeltaZ;
		y = (v31 + v32).y + m_vecVertex[_3].y;
	}
	return true;
}


