#include "StdAfx.h"
#include "cCamera.h"


cCamera::cCamera(void)
	: m_vEye(0, 0, -10)
	, m_vLookAt(0, 0, 0)
	, m_vUp(0, 1, 0)
	, m_isLButtonDown(false)
	, m_fAngleX(0.0f)
	, m_fAngleY(0.0f)
	, m_fDistance(60.0f)
	, upDown(0.0f)
{
	D3DXMatrixIdentity(&m_matViewProj);
}

cCamera::~cCamera(void)
{
}

void cCamera::Setup()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vLookAt, &m_vUp);
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &m_matView);

	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI / 4.0f, rc.right / (float)rc.bottom, 1, 1000);
	g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

void cCamera::Update(D3DXVECTOR3* pTarget)
{
	m_vEye = D3DXVECTOR3(0, pTarget->y, -m_fDistance);
	D3DXMATRIXA16 matRX, matRY, mat;
	D3DXMatrixRotationX(&matRX, m_fAngleX);
	D3DXMatrixRotationY(&matRY, m_fAngleY);

	mat = matRX * matRY;

	D3DXVec3TransformCoord(&m_vEye, &m_vEye, &mat);

	if (pTarget)
	{
		m_vLookAt = *pTarget;
		m_vEye = m_vEye + m_vLookAt;
	}
	if (g_pkeyManager->isStayKeyDown(VK_UP))
	{
		upDown += 0.3f;
	}
	if (g_pkeyManager->isStayKeyDown(VK_DOWN))
	{
		upDown -= 0.3f;
	}
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vLookAt, &m_vUp);
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &m_matView);
}

D3DXMATRIXA16* cCamera::GetViewMatrix()
{
	return &m_matView;
}

D3DXMATRIXA16* cCamera::GetProjMatrix()
{
	return &m_matProj;
}
D3DXMATRIXA16* cCamera::GetViewProjMatrix()
{
	m_matViewProj = m_matView * m_matProj;
	return &m_matViewProj;
}

void cCamera::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		m_isLButtonDown = true;

		m_ptPrevMouse.x = LOWORD(lParam);
		m_ptPrevMouse.y = HIWORD(lParam);
	}
	break;
	case WM_LBUTTONUP:
	{
		m_isLButtonDown = false;
	}
	break;
	case WM_MOUSEMOVE:
	{
		if (m_isLButtonDown)
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);

			int nDeltaX = pt.x - m_ptPrevMouse.x;
			int nDeltaY = pt.y - m_ptPrevMouse.y;

			m_fAngleX += nDeltaY * 0.01f;
			if (m_fAngleX > D3DX_PI / 2.0f - EPSILON)
				m_fAngleX = D3DX_PI / 2.0f - EPSILON;

			if (m_fAngleX < -D3DX_PI / 2.0f + EPSILON)
				m_fAngleX = -D3DX_PI / 2.0f + EPSILON;

			m_fAngleY += nDeltaX * 0.01f;

			m_ptPrevMouse = pt;
		}

	}
	break;
	case WM_MOUSEWHEEL:
	{
		int n = GET_WHEEL_DELTA_WPARAM(wParam);
		m_fDistance -= n / 100.f;
	}
	break;
	}



	//void ProcessMouse(void)
	//{
	//	POINT pt;
	//	float fDelta = 0.001f; // 마우스의 민감도, 이 값이 커질수록 많이 움직인다.

	//	GetCursorPos(&pt);
	//	int dx = pt.x - m_ptPrevMouse; // 마우스의 변화값
	//	int dy = pt.y - m_ptPrevMouse; // 마우스의 변화값

	//	g_pCamera->RotateLocalX(dy * fDelta); // 마우스의 Y축 회전값은 3D world의  X축 회전값
	//	g_pCamera->RotateLocalY(dx * fDelta); // 마우스의 X축 회전값은 3D world의  Y축 회전값
	//	D3DXMATRIXA16* pmatView = g_pCamera->GetViewMatrix(); // 카메라 행렬을 얻는다.
	//	g_pd3dDevice->SetTransform(D3DTS_VIEW, pmatView); // 카메라 행렬 셋팅


	//													  // 마우스를 윈도우의 중앙으로 초기화
	//	SetCursor(NULL); // 마우스를 나타나지 않게 않다.
	//	RECT rc;
	//	GetClientRect(g_hwnd, &rc);
	//	pt.x = (rc.right - rc.left) / 2;
	//	pt.y = (rc.bottom - rc.top) / 2;
	//	ClientToScreen(g_hwnd, &pt);
	//	SetCursorPos(pt.x, pt.y);
	//	g_dwMouseX = pt.x;
	//	g_dwMouseY = pt.y;
	//}
}


