#include "stdafx.h"
#include "cRay.h"
#include "cBoundingSphere.h"

cRay::cRay()
	: m_vOri(0, 0, 0)
	, m_vDir(0, 0, 0)
{
}


cRay::~cRay()
{
}


cRay cRay::CalcViewSpace(int _x, int _y)
{
	cRay _ray;
	D3DVIEWPORT9 vp;
	g_pD3DDevice->GetViewport(&vp);

	D3DXMATRIXA16 proj;
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &proj);

	_ray.m_vDir.x = (((2.0f * _x) / vp.Width) - 1.0f) / proj(0, 0);
	_ray.m_vDir.y = (((-2.0f * _y) / vp.Height) + 1.0f) / proj(1, 1);
	_ray.m_vDir.z = 1.0f;

	D3DXVec3Normalize(&_ray.m_vDir, &_ray.m_vDir);

	//	_ray.m_vOri = D3DXVECTOR3(0, 0, 0);

	return _ray;
}


cRay cRay::CalcWorldSpace(int _x, int _y)
{
	cRay _ray = cRay::CalcViewSpace(_x, _y);

	D3DXMATRIXA16 view, inverse;
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &view);
	D3DXMatrixInverse(&inverse, 0, &view);

	D3DXVec3TransformCoord(&_ray.m_vOri, &_ray.m_vOri, &inverse);
	D3DXVec3TransformNormal(&_ray.m_vDir, &_ray.m_vDir, &inverse);

	D3DXVec3Normalize(&_ray.m_vDir, &_ray.m_vDir);

	return _ray;
}


cRay cRay::CalcViewSpace(POINT _pos)
{
	cRay _ray;
	D3DVIEWPORT9 vp;
	g_pD3DDevice->GetViewport(&vp);

	D3DXMATRIXA16 proj;
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &proj);

	_ray.m_vDir.x = (((2.0f * _pos.x) / vp.Width) - 1.0f) / proj(0, 0);
	_ray.m_vDir.y = (((-2.0f * _pos.y) / vp.Height) + 1.0f) / proj(1, 1);
	_ray.m_vDir.z = 1.0f;

	D3DXVec3Normalize(&_ray.m_vDir, &_ray.m_vDir);

	//	_ray.m_vOri = D3DXVECTOR3(0, 0, 0);

	return _ray;
}


cRay cRay::CalcWorldSpace(POINT _pos)
{
	cRay _ray = cRay::CalcViewSpace(_pos.x, _pos.y);

	D3DXMATRIXA16 view, inverse;
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &view);
	D3DXMatrixInverse(&inverse, 0, &view);

	D3DXVec3TransformCoord(&_ray.m_vOri, &_ray.m_vOri, &inverse);
	D3DXVec3TransformNormal(&_ray.m_vDir, &_ray.m_vDir, &inverse);

	D3DXVec3Normalize(&_ray.m_vDir, &_ray.m_vDir);

	return _ray;
}


cRay cRay::WorldToLocal(D3DXVECTOR3 v) //벡터는 구의 중심점
{
	cRay r = (*this);
	r.m_vOri -= v;
	return r;
}


bool cRay::IsPick(cBoundingSphere* sphere)
{
	float qq = D3DXVec3Dot(&m_vOri, &m_vOri);
	float qv = D3DXVec3Dot(&m_vOri, &m_vDir);
	float vv = D3DXVec3Dot(&m_vDir, &m_vDir);
	float rr = sphere->GetRadius() * sphere->GetRadius();

	return qv * qv - vv * (qq - rr) >= 0;
}