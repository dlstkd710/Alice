#include "stdafx.h"
#include "cCatmullRom.h"

#include <stack>
#include <queue>

cCatmullRom::cCatmullRom()
	: m_UpdateCnt(0)
{
}

cCatmullRom::~cCatmullRom()
{
}

void cCatmullRom::Setup(IN const D3DXVECTOR3 UpPos, IN const D3DXVECTOR3 DownPos)
{
	/* Vertex Pos */
	m_vPos.resize(8);

	for (int i = 0; i < 4; ++i)
	{
		m_vPos[i * 2 + 0] = UpPos;
		m_vPos[i * 2 + 1] = DownPos;
	}

	//m_pTexture = g_pTextureManager->GetTexture("FilePath.tga");
}

void cCatmullRom::Release()
{
	if (m_pTexture)
		m_pTexture->Release();
}

void cCatmullRom::Update(IN const D3DXVECTOR3 UpPos, const IN D3DXVECTOR3 DownPos)
{
	/*m_UpdateCnt++;

	if (m_UpdateCnt == 5 || m_UpdateCnt == 10 || m_UpdateCnt == 15)
	{
	PosPull();
	m_vPos[6] = UpPos;
	m_vPos[7] = DownPos;

	int a = 0;
	}
	else if (m_UpdateCnt == 20)
	{
	PosPull();
	m_vPos[6] = UpPos;
	m_vPos[7] = DownPos;

	m_UpdateCnt = 0;

	CalcCatmullRom();

	int a = 0;
	}*/

	PosPull();
	m_vPos[6] = UpPos;
	m_vPos[7] = DownPos;

	CalcCatmullRom();
}

void cCatmullRom::Render()
{
	if (VertexBuffer.size() > 3)
	{
		g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, FALSE);
		g_pD3DDevice->SetTexture(0, NULL);

		D3DXMATRIXA16 matWorld;
		D3DXMatrixIdentity(&matWorld);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
		g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);
		g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
			VertexBuffer.size() / 3,
			&VertexBuffer[0],
			sizeof(ST_PC_VERTEX));
		g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, TRUE);
	}

}

void cCatmullRom::CalcCatmullRom()
{
	vector<D3DXVECTOR3> m_vPosVertex;

	for (int i = 0; i < (LINENUM / 3); ++i)
	{
		float S = (float)i / (float)(LINENUM / 3);

		D3DXVECTOR3 temp, sour;

		D3DXVec3CatmullRom(&temp, &m_vPos[1], &m_vPos[3], &m_vPos[5], &m_vPos[7], S);
		D3DXVec3CatmullRom(&sour, &m_vPos[0], &m_vPos[2], &m_vPos[4], &m_vPos[6], S);

		m_vPosVertex.push_back(temp);
		m_vPosVertex.push_back(sour);
	}

	//vector<ST_PC_VERTEX> VertexBuffer;
	//VertexBuffer.resize(588);
	//
	//for (int j = 0; j < 98; ++j)
	//{
	//	VertexBuffer[j * 6 + 0] = ST_PC_VERTEX(m_vPosVertex[j * 2 + 1], D3DCOLOR_XRGB(255, 255, 255));
	//	VertexBuffer[j * 6 + 1] = ST_PC_VERTEX(m_vPosVertex[j * 2 + 2], D3DCOLOR_XRGB(255, 255, 255));;
	//	VertexBuffer[j * 6 + 2] = ST_PC_VERTEX(m_vPosVertex[j * 2 + 0], D3DCOLOR_XRGB(255, 255, 255));;
	//	VertexBuffer[j * 6 + 3] = ST_PC_VERTEX(m_vPosVertex[j * 2 + 1], D3DCOLOR_XRGB(255, 255, 255));;
	//	VertexBuffer[j * 6 + 4] = ST_PC_VERTEX(m_vPosVertex[j * 2 + 3], D3DCOLOR_XRGB(255, 255, 255));;
	//	VertexBuffer[j * 6 + 5] = ST_PC_VERTEX(m_vPosVertex[j * 2 + 2], D3DCOLOR_XRGB(255, 255, 255));;
	//}

	if (VertexBuffer.size() > 5880)
	{
		VertexPull();
		VertexBuffer.resize(5880);
	}

	for (int j = 0; j < 98; ++j)
	{
		VertexBuffer.push_back(ST_PC_VERTEX(m_vPosVertex[j * 2 + 1], D3DCOLOR_XRGB(255, 255, 255)));
		VertexBuffer.push_back(ST_PC_VERTEX(m_vPosVertex[j * 2 + 2], D3DCOLOR_XRGB(255, 255, 255)));;
		VertexBuffer.push_back(ST_PC_VERTEX(m_vPosVertex[j * 2 + 0], D3DCOLOR_XRGB(255, 255, 255)));;
		VertexBuffer.push_back(ST_PC_VERTEX(m_vPosVertex[j * 2 + 1], D3DCOLOR_XRGB(255, 255, 255)));;
		VertexBuffer.push_back(ST_PC_VERTEX(m_vPosVertex[j * 2 + 3], D3DCOLOR_XRGB(255, 255, 255)));;
		VertexBuffer.push_back(ST_PC_VERTEX(m_vPosVertex[j * 2 + 2], D3DCOLOR_XRGB(255, 255, 255)));;
	}

	int a = 0;
}

void cCatmullRom::Reset(IN const D3DXVECTOR3 UpPos, IN const D3DXVECTOR3 DownPos)
{
	for (int i = 0; i < 8; ++i)
	{
		m_vPos[i + 0] = UpPos;
		m_vPos[i + 1] = DownPos;
	}
}

void cCatmullRom::PosPull()
{
	// 2개의 벡터를 앞쪽으로 당긴다
	for (int i = 0; i < 3; ++i)
	{
		m_vPos[i * 2] = m_vPos[i * 2 + 2];
		m_vPos[i * 2 + 1] = m_vPos[i * 2 + 1 + 2];
	}
}

void cCatmullRom::VertexPull()
{
	for (int i = 0; i < 588; ++i)
	{
		if (i == 587)
		{
			int a = 0;
		}
		VertexBuffer[i + (588 * 0)] = VertexBuffer[i + (588 * 1)];
		VertexBuffer[i + (588 * 1)] = VertexBuffer[i + (588 * 2)];
		VertexBuffer[i + (588 * 2)] = VertexBuffer[i + (588 * 3)];
		VertexBuffer[i + (588 * 3)] = VertexBuffer[i + (588 * 4)];
		VertexBuffer[i + (588 * 4)] = VertexBuffer[i + (588 * 5)];
		VertexBuffer[i + (588 * 5)] = VertexBuffer[i + (588 * 6)];
		VertexBuffer[i + (588 * 6)] = VertexBuffer[i + (588 * 7)];
		VertexBuffer[i + (588 * 7)] = VertexBuffer[i + (588 * 8)];
		VertexBuffer[i + (588 * 8)] = VertexBuffer[i + (588 * 9)];
		VertexBuffer[i + (588 * 9)] = VertexBuffer[i + (588 * 10)];
	}
}

/*

1 3 5 7 9 11 13 15 17 19 21 23 25 27 29 31 33 35 37 39 41 43 45 47 49 51

0 2 4 6 8 10 12 14 16 18 20 22 24 26 28 30 32 34 36 38 40 42 44 46 48 50


0 1 2 2 1 3

2 3 4 4 3 5

*/