#pragma once

#include "All_Include.h"

enum motion{ 없음, 뛰기, 약공격, 큰공격, 내려찍기 };
class cMainGame
{
private:
	cPyramid*					m_pPyramid;
	cCamera*					m_pCamera;
	cCube*						m_pRoot;
	cGrid*						m_pGrid;
	cCrtController*				m_pController;
	iMap*						m_pMap;

	LPD3DXMESH					m_pMesh;
	D3DMATERIAL9				m_stMtlNormal;
	D3DMATERIAL9				m_stMtlPicked;
	std::vector<cMtlTex*>		m_vecMtlTex;
	LPD3DXMESH					m_pMapMesh;
	cAnimation*					_Animation;
	cText*						m_cText;
	int							chargessin;
	motion						zmotion;
	Terrain*					m_Terrrain;

	FrustumClling*				m_FrustumClling;
	cHeightMap*					m_HeightMap;
	
	/* ------------ */
	cSky* m_pSky;
	/* ------------ */

	/* ------------ */
	UiMain* m_UiMain;

	cObjectXfile* m_Object;
	/* ------------ */

	HWND _btnUp;
	HWND _btnDown;
	HWND _btnsetup;
	HWND _btnsetup1;


	bool a;
	D3DXMATRIXA16* binMax;
	std::vector<cAnimation*>	m_vecSkinnedMesh;

	D3DXMATRIXA16 mat;

	D3DXMATRIXA16 temp;

	DWORD						m_dwCurrentTrack;    
	DWORD						m_dwAnimIdxLoiter;
	DWORD						m_dwAnimIdxWalk;
	DWORD						m_dwAnimIdxJog;

	double m_dTimeCurrent;    
	float m_fSpeed;           
	float m_fSpeedWalk;
	int n, m;

	D3DXVECTOR3					m_PickPos;
	D3DXVECTOR3					taget;

	bool ok;
	HDC m_Hdc;
	HDC	m_Hdcmodal;
	POINT imagePt;

private:
	LPD3DXSPRITE				m_pSprite;
	D3DXIMAGE_INFO				m_stImageInfo;
	LPDIRECT3DTEXTURE9			m_pTexture;
	Image   *TestImage;
public:
	cMainGame(void);
	~cMainGame(void);

	void Setup();
	void Update();
	void Render();
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void SetLight();
	void SetMoveKey(int intdex);



};

