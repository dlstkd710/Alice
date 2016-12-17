#include "StdAfx.h"
#include "cPyramid.h"


cPyramid::cPyramid(void)
{
}


cPyramid::~cPyramid(void)
{
}

void cPyramid::Setup(D3DCOLOR c, D3DXMATRIXA16* pmat /*= NULL*/)
{
	ST_PC_VERTEX v;
	
	v.c = c;

	v.p = D3DXVECTOR3( 0,  0,  0); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3( 1, -1, -1); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-1, -1, -1); m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3( 0,  0,  0); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3( 1, -1,  1); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3( 1, -1, -1); m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3( 0,  0,  0); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-1, -1,  1); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3( 1, -1,  1); m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3( 0,  0,  0); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-1, -1, -1); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-1, -1,  1); m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3( 1, -1, -1); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3( 1, -1,  1); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-1, -1, -1); m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3(-1, -1,  1); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-1, -1, -1); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3( 1, -1,  1); m_vecVertex.push_back(v);

	if(pmat)
	{	
		for(size_t i = 0; i < m_vecVertex.size(); ++i)
		{
			D3DXVec3TransformCoord(&m_vecVertex[i].p,
				&m_vecVertex[i].p,
				pmat); 
		}
	}
}

void cPyramid::Render()
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
		m_vecVertex.size() / 3,
		&m_vecVertex[0],
		sizeof(ST_PC_VERTEX));
}
