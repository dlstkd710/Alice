#pragma once
class cCubeP;
class cCamera;
class cGrid;
class Shadow;

class cMainGame
{
private:
	cCubeP*				m_pCube;
	cCamera*			m_pCamera;
	cGrid*				m_pGrid;
	Shadow*				m_pShadow;
public:
	cMainGame(void);
	~cMainGame(void);

	void Setup();
	void Update();
	void Render();
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

