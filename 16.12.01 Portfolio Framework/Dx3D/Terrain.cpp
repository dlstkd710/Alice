#include "stdafx.h"
#include "Terrain.h"

#define Number 10
#define DELTA_Y 0.1f

Terrain::Terrain()
	: m_pVB(NULL)
	, m_pIB(NULL)
	, m_nNumTri(0)
	, m_nNumVertex(0)
	, m_pTexture(NULL)
{
	ZeroMemory(&m_stMtl, sizeof(D3DMATERIAL9));

}


Terrain::~Terrain()
{
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);
	SAFE_RELEASE(m_pTexture);

}



void Terrain::Load(char * szFilename, char * szTexture, int nBytesPerPixel)
{
	if (m_pTexture == NULL)
	{
		m_pTexture = g_pTextureManager->GetTexture(szTexture);
		SAFE_ADD_REF(m_pTexture);
	}

	FILE* fp;

	fopen_s(&fp, szFilename, "rb");

	fseek(fp, 0, SEEK_END);
	int MaplSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	int nPixel = MaplSize / nBytesPerPixel;

	nRow = (int)sqrt(nPixel + 0.0001f);

	nCol = nRow;


	for (int i = 0; i < nPixel; ++i)
	{
		vecData.push_back(fgetc(fp));
		if (nBytesPerPixel > 1)
			fseek(fp, nBytesPerPixel - 1, SEEK_CUR);
	}

	fclose(fp);

	m_nNumVertex = nPixel;

	int nNumTile = nRow - 1;

	m_nNumTile = nNumTile;

	std::vector<ST_PNT_VERTEX> vecVertex;

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

	m_nNumTri = nNumTile * nNumTile * 2;

	for (int z = 1; z < nNumTile; z++)
	{
		for (int x = 1; x <nNumTile; x++)
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

bool Terrain::GetHeight(IN float x, OUT float& y, IN float z)
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

	if (fDeltaX + fDeltaZ < 1)
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


float Terrain::GetHeight2(IN float x,IN float z)
{
	x = ((float)nCol / 2.0f) + x;
	z = ((float)nRow / 2.0f) - z;


	float col = ::floorf(x);
	float row = ::floorf(z);

	float A = getHeightmapEntry(row,	col);
	float B = getHeightmapEntry(row,	col + 1);
	float C = getHeightmapEntry(row + 1,col);
	float D = getHeightmapEntry(row + 1,col + 1);
	float dx = x - col;
	float dz = z - row;

	float height = 0.0f;
	if (dz < 1.0f - dx)  // upper triangle ABC
	{
		float uy = B - A; // A->B
		float vy = C - A; // A->C

		height = A + Lerp(0.0f, uy, dx) + Lerp(0.0f, vy, dz);
	}
	else // lower triangle DCB
	{
		float uy = C - D; // D->C
		float vy = B - D; // D->B
	
		height = D + Lerp(0.0f, uy,1.0f -  dx) + Lerp(0.0f, vy,1.0f -  dz);


	}



	return height;
}


void Terrain::Render()
{

	D3DXMATRIXA16 matI, matT;

	D3DXMatrixIdentity(&matI);
	D3DXMatrixIdentity(&matT);
	//D3DXMatrixRotationY(matT, °¢µµ(45.0f));
	//D3DXMatrixTranslation(&matT, 0, -10, 0);

	//matI = matT;
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
}

float Terrain::Lerp(float a, float b, float t)
{
	return a - (a*t) + (b*t);
}
int Terrain::getHeightmapEntry(int row, int col)
{
	return vecData[row * (nCol -1) + col];
}