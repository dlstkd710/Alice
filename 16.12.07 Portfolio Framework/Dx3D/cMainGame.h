#pragma once

#include "All_Include.h"
#include "cSoundManager.h"
class cMainGame
{
private:
	cCamera*					m_pCamera;
	cCube*						m_pRoot;
	cGrid*						m_pGrid;
	cCrtController*				m_pController;
	iMap*						m_pMap;
	cTerrain*					m_Terrrain;
	cSoundManager*				m_pSound;
	/* ------------ */
	CharacterClass*				m_pCharacter;
	/* ------------ */

	/* ------------ */
	cSky* m_pSky;
	/* ------------ */

	/* ------------ */
	UiMain* m_UiMain;

	cObjectXfile* m_Object;
	/* ------------ */

	/* ------------ */
	cMonsterManager*			m_pMonster;
	/* ------------ */
	LPDIRECTSOUNDBUFFER g_lpDSBG[2];
public:
	cMainGame(void);
	~cMainGame(void);

	void Setup();
	void Update();
	void Render();
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void SetLight();



};

