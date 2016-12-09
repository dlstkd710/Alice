#include "stdafx.h"
#include "FrustumClling.h"


FrustumClling::FrustumClling()
{
}


FrustumClling::~FrustumClling()
{
}

BOOL FrustumClling::Make(D3DXMATRIXA16 * pmatViewProj)
{
	m_VecDot.clear();
	D3DXMATRIXA16  matInv;

	m_VecDot.push_back(D3DXVECTOR3(-1.0f, -1.0f, 0.0f));
	m_VecDot.push_back(D3DXVECTOR3(1.0f, -1.0f, 0.0f));
	m_VecDot.push_back(D3DXVECTOR3(1.0f, -1.0f, 1.0f));
	m_VecDot.push_back(D3DXVECTOR3(-1.0f, -1.0f, 1.0f));
	m_VecDot.push_back(D3DXVECTOR3(-1.0f, 1.0f, 0.0f));
	m_VecDot.push_back(D3DXVECTOR3(1.0f, 1.0f, 0.0f));
	m_VecDot.push_back(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	m_VecDot.push_back(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));

	D3DXMatrixInverse(&matInv, NULL, pmatViewProj);

	for (int i = 0; i < 8; i++) {
		D3DXVec3TransformCoord(&m_VecDot[i], &m_VecDot[i], &matInv);
	}

	D3DXPlaneFromPoints(&m_plane[0], &m_VecDot[4], &m_VecDot[7], &m_VecDot[6]);	// 상 평면(top)
	D3DXPlaneFromPoints(&m_plane[1], &m_VecDot[0], &m_VecDot[1], &m_VecDot[2]);	// 하 평면(bottom)
	D3DXPlaneFromPoints(&m_plane[2], &m_VecDot[0], &m_VecDot[4], &m_VecDot[5]);	// 근 평면(near)
	D3DXPlaneFromPoints(&m_plane[3], &m_VecDot[2], &m_VecDot[6], &m_VecDot[7]);	// 원 평면(far)
	D3DXPlaneFromPoints(&m_plane[4], &m_VecDot[0], &m_VecDot[3], &m_VecDot[7]);	// 좌 평면(left)
	D3DXPlaneFromPoints(&m_plane[5], &m_VecDot[1], &m_VecDot[5], &m_VecDot[6]);	// 우 평면(right)

	return true;
}


BOOL FrustumClling::IsInSphere(D3DXVECTOR3 * pv, float radius)
{
	float fDist;
	for each(auto m_plane in m_plane) {
		fDist = D3DXPlaneDotCoord(&m_plane, pv);
		if (fDist > (radius + ang)) return FALSE;
	}
	return TRUE;
}
