#include "stdafx.h"
#include "cText.h"


cText::cText()
	: Font(NULL)
	, FrameCnt(0)
	, TimeElapsed(0.0f)
	, FPS(0.0f)
	, m_pSprite(NULL)

{
	D3DXMatrixIdentity(&mat);
	D3DXMatrixIdentity(&matWorld);
}

cText::~cText()
{
}

void cText::Setup()
{
	HDC hdc = CreateCompatibleDC(0); 																																				   // 글팔을 만들고 장치 컨텍스트로 글꼴을 선택한다. HFONT hFont; HFONT hFontOld; hFont = CreateFontlndirect(&lf); hFontOld = (HFONT) SelectObject (hdc, hFont); 
																																				   // 텍스트의 3D 메쉬를 만들어낸다. ID3DXMesh* Text = 0; D3DXCreateText( device, hdc, "Direct3D", O.OOlf, 0.4f, &Text, 0, 0); 
	LOGFONT lf;
	ZeroMemory(&lf, sizeof(LOGFONT)); 		
	
	lf.lfHeight = 25;
	lf.lfWidth = 12;
	lf.lfWeight = 500;
	// 논리 단위 // 논리 단위 // 굵기 정도I 범위 o (앓음) - 1000 (두꺼움) 
	lf.lfItalic = false;
	lf.lfUnderline = false;
	lf.lfStrikeOut = false; 
	lf.lfCharSet = DEFAULT_CHARSET; 
	strcpy(lf.lfFaceName, "Times New Roman"); // 글꼴 스타일 // 다시 이전의 글꼴율 선택하고 자원을 해제한다. 
	
	Text = 0;

	HFONT hFont;
	HFONT hFontOld;
	hFont = CreateFontIndirect(&lf);
	hFontOld = (HFONT)SelectObject(hdc, hFont);

	D3DXCreateText(g_pD3DDevice, hdc, "18.18", 0.001f, 0.4f, &Text, 0, 0);

	SelectObject(hdc, hFontOld);
	DeleteObject( hFont );
	DeleteDC( hdc ) ; 
}

void cText::reset()
{
	D3DXMatrixIdentity(&matWorld);
}

void cText::update()
{

	D3DXMatrixTranslation(&mat, 0.0f, 0.1f, 0.0f);

	matWorld = matWorld*  mat;
}


void cText::render()
{


	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

	Text->DrawSubset( 0 ) ; 
}
//
void cText::fpsSetup()
{
	//
	// Initialize a LOGFONT structure that describes the font
	// we want to create.
	//

	LOGFONT lf;
	ZeroMemory(&lf, sizeof(LOGFONT));

	lf.lfHeight = 25;    // in logical units
	lf.lfWidth = 12;    // in logical units
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfWeight = 500;   // boldness, range 0(light) - 1000(bold)
	lf.lfItalic = false;
	lf.lfUnderline = false;
	lf.lfStrikeOut = false;
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = 0;
	lf.lfClipPrecision = 0;
	lf.lfQuality = 0;
	lf.lfPitchAndFamily = 0;
	strcpy(lf.lfFaceName, "Times New Roman"); // font style

	D3DXCreateFont(g_pD3DDevice, 20, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		TEXT("돋움체"), &Font);			  //
											  // Create an ID3DXFont based on 'lf'.
											  //
	return;
}
void cText::fpsUpdate(float timeDelta , float FPS1)
{
	FrameCnt++;

	TimeElapsed += timeDelta;

	if (TimeElapsed >= 1.0f)
	{
		FPS = (float)FrameCnt / TimeElapsed;

		sprintf(FPSString, " FPS : %f , Y : %f", FPS , FPS1);
		FPSString[19] = '\0'; // mark end of string

		TimeElapsed = 0.0f;
		FrameCnt = 0;
	}
}

void cText::fpsRender()
{

	RECT rect = { 0, 0, 450, 450 };

	Font->DrawText(m_pSprite,
		FPSString,
		-1, // size of string or -1 indicates null terminating string
		&rect,            // rectangle text is to be formatted to in windows coords
		DT_TOP | DT_LEFT, // draw in the top left corner of the viewport
		D3DCOLOR_XRGB(0, 255, 255));      // black text


}
