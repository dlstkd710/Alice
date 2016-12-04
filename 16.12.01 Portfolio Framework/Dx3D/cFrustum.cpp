#include "stdafx.h"
#include "cFrustum.h"
#include "cBoundingSphere.h"


cFrustum::cFrustum()
{
}


cFrustum::~cFrustum()
{
}


void cFrustum::Setup()
{
	m_vecV.resize(8);
	m_vecPlane.resize(6);

	m_vecV[0] = D3DXVECTOR3(-1, -1, 0);
	m_vecV[1] = D3DXVECTOR3(-1, 1, 0);
	m_vecV[2] = D3DXVECTOR3(1, 1, 0);
	m_vecV[3] = D3DXVECTOR3(1, -1, 0);

	m_vecV[4] = D3DXVECTOR3(-1, -1, 1);
	m_vecV[5] = D3DXVECTOR3(-1, 1, 1);
	m_vecV[6] = D3DXVECTOR3(1, 1, 1);
	m_vecV[7] = D3DXVECTOR3(1, -1, 1);
}


void cFrustum::Update()
{
	D3DXMATRIXA16 matProj, matView;
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);

	vector<D3DXVECTOR3>		vecV;
	vecV.resize(8);
	for (int i = 0; i < m_vecV.size(); i++)
	{
		D3DXVec3Unproject(
			&vecV[i],
			&m_vecV[i],
			NULL,
			&matProj,
			&matView,
			NULL);
	}

	D3DXPlaneFromPoints(&m_vecPlane[0], &vecV[0], &vecV[1], &vecV[2]);		//앞
	D3DXPlaneFromPoints(&m_vecPlane[1], &vecV[7], &vecV[6], &vecV[5]);		//뒤
	D3DXPlaneFromPoints(&m_vecPlane[2], &vecV[1], &vecV[5], &vecV[6]);		//상
	D3DXPlaneFromPoints(&m_vecPlane[3], &vecV[0], &vecV[7], &vecV[4]);		//하
	D3DXPlaneFromPoints(&m_vecPlane[4], &vecV[4], &vecV[5], &vecV[1]);		//좌
	D3DXPlaneFromPoints(&m_vecPlane[5], &vecV[3], &vecV[2], &vecV[6]);		//우
}


bool cFrustum::IsinFrustum(cBoundingSphere* pSphere)
{
	for (int i = 0; i < m_vecPlane.size(); i++)
		if (D3DXPlaneDotCoord(&m_vecPlane[i], &pSphere->GetCenter()) > pSphere->GetRadius())
			return false;

	return true;
}