#pragma once

class cCamera;
class cGrid;

class cMainGame
{
private:
	cCamera*					m_pCamera;
	cGrid*						m_pGrid;
	LPD3DXSPRITE				m_pSprite;
	D3DXIMAGE_INFO				m_stImageInfo;
	LPDIRECT3DTEXTURE9			m_pTexture;
	LPD3DXFONT					m_pFont;

public:
	cMainGame(void);
	~cMainGame(void);

	void Setup();
	void Update();
	void Render();
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void SetLight();
};

