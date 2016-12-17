#include "StdAfx.h"
#include "cCube.h"


cCube::cCube(void)
	: m_vPosition(0, 0, 0)
	, m_vDirection(0, 0, 1)
	, m_fAngle(0.0f)
	, m_fSpeed(0.1f)
{
}

cCube::~cCube(void)
{
}

void cCube::Setup()
{
	std::vector<D3DXVECTOR3>	vecVertex;

	vecVertex.reserve(8);
	vecVertex.push_back(D3DXVECTOR3(-1.0f, -1.0f, -1.0));
	vecVertex.push_back(D3DXVECTOR3(-1.0f,  1.0f, -1.0));
	vecVertex.push_back(D3DXVECTOR3( 1.0f,  1.0f, -1.0));
	vecVertex.push_back(D3DXVECTOR3( 1.0f, -1.0f, -1.0));
	vecVertex.push_back(D3DXVECTOR3(-1.0f, -1.0f,  1.0));
	vecVertex.push_back(D3DXVECTOR3(-1.0f,  1.0f,  1.0));
	vecVertex.push_back(D3DXVECTOR3( 1.0f,  1.0f,  1.0));
	vecVertex.push_back(D3DXVECTOR3( 1.0f, -1.0f,  1.0));

	std::vector<int>		vecIndex;
	vecIndex.reserve(36);
	vecIndex.push_back(0); 
	vecIndex.push_back(1);
	vecIndex.push_back(2);

	vecIndex.push_back(0);
	vecIndex.push_back(2);
	vecIndex.push_back(3);

	vecIndex.push_back(4);
	vecIndex.push_back(6);
	vecIndex.push_back(5);

	vecIndex.push_back(4);
	vecIndex.push_back(7);
	vecIndex.push_back(6);

	vecIndex.push_back(4);
	vecIndex.push_back(5);
	vecIndex.push_back(1);

	vecIndex.push_back(4);
	vecIndex.push_back(1);
	vecIndex.push_back(0);

	vecIndex.push_back(3);
	vecIndex.push_back(2);
	vecIndex.push_back(6);

	vecIndex.push_back(3);
	vecIndex.push_back(6);
	vecIndex.push_back(7);

	vecIndex.push_back(1);
	vecIndex.push_back(5);
	vecIndex.push_back(6);

	vecIndex.push_back(1);
	vecIndex.push_back(6);
	vecIndex.push_back(2);

	vecIndex.push_back(4);
	vecIndex.push_back(0);
	vecIndex.push_back(3);

	vecIndex.push_back(4);
	vecIndex.push_back(3);
	vecIndex.push_back(7);

	m_vecVertex.reserve(36);
	D3DCOLOR c;
	for (size_t i = 0; i < vecIndex.size(); ++i)
	{
		ST_PC_VERTEX v;
		v.p = vecVertex[vecIndex[i]];
		if(i % 6 == 0)
			c = D3DCOLOR_XRGB(rand() % 256, rand() % 256, rand() % 256);
		v.c = c;
		m_vecVertex.push_back(v);
	}

	D3DXMatrixIdentity(&m_matWorld);
}

void cCube::Update()
{
	if (GetKeyState('A') & 0x8000)
	{
		m_fAngle -= 0.1f;
	}
	if (GetKeyState('D') & 0x8000)
	{
		m_fAngle += 0.1f;
	}
	
	m_vDirection = D3DXVECTOR3(0, 0, 1);
	D3DXMATRIXA16 matRotY;
	D3DXMatrixRotationY(&matRotY, m_fAngle);
	D3DXVec3TransformNormal(&m_vDirection, &m_vDirection, &matRotY);

	if (GetKeyState('W') & 0x8000)
	{
		m_vPosition = m_vPosition + m_vDirection * m_fSpeed;
	}
	if (GetKeyState('S') & 0x8000)
	{
		m_vPosition = m_vPosition - m_vDirection * m_fSpeed;
	}
	
	D3DXMATRIXA16 matT;
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	m_matWorld = matRotY * matT;
}

void cCube::Render()
{
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
		m_vecVertex.size() / 3,
		&m_vecVertex[0],
		sizeof(ST_PC_VERTEX));
}

D3DXVECTOR3* cCube::GetPosition()
{
	return &m_vPosition;
}
