#pragma once

#include "All_Include.h"

class cMainGame
{
private:
	cCamera*					m_pCamera;
	cCube*						m_pRoot;
	cGrid*						m_pGrid;
	cCrtController*				m_pController;

	/* ------------ */
	cSky* m_pSky;
	/* ------------ */

	/* ------------ */
	UiMain* m_UiMain;

	cObjtool* m_Objtool;
	/* ------------ */

public:
	cMainGame(void);
	~cMainGame(void);

	void Setup();
	void Update();
	void Render();
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void SetLight();



};

