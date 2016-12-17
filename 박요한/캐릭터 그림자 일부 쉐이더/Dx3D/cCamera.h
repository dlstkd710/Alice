#pragma once

class cCamera
{
private:
	D3DXVECTOR3		m_vEye;
	D3DXVECTOR3		m_vLookAt;
	D3DXVECTOR3		m_vUp;
	bool			m_isLButtonDown;
	POINT			m_ptPrevMouse;
	float			m_fAngleX;
	float			m_fAngleY;
	float			m_fDistance;

	D3DXMATRIXA16 matView;
	D3DXMATRIXA16 matProj, m_matViewProj;


public:
	cCamera(void);
	~cCamera(void);

	D3DXVECTOR3*   GetEyesVecter();
	D3DXMATRIXA16* GetViewMatrix();
	D3DXMATRIXA16* GetProjMatrix();
	D3DXMATRIXA16* GetViewProjMatrix();

	void Setup();
	void Update(D3DXVECTOR3* pTarget = NULL);
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

