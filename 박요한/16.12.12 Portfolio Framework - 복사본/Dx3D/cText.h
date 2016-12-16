#pragma once
class cText
{
protected:
	ID3DXMesh* Text;
	D3DXMATRIXA16 mat, matWorld;

	ID3DXFont* Font;
	LPD3DXSPRITE m_pSprite;
	DWORD FrameCnt;
	float TimeElapsed;
	float FPS;
	char FPSString[20];


public:
	cText();
	~cText();

	void Setup();
	void reset();
	void update();
	void render();
	void fpsSetup();
	void fpsUpdate(float timeDelta, float FPS1);
	void fpsRender();
};

