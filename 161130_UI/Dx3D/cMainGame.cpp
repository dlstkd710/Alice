#include "StdAfx.h"
#include "cMainGame.h"
#include "cCamera.h"
#include "cGrid.h"

cMainGame::cMainGame(void)
	: m_pCamera(NULL)
	, m_pGrid(NULL)
	, m_pSprite(NULL)
	, m_pTexture(NULL)
	, m_pFont(NULL)
{
}

cMainGame::~cMainGame(void)
{
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);
	
	SAFE_RELEASE(m_pSprite);
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pFont);

	g_pSkinnedMeshManager->Destroy();
	g_pObjectManager->Destroy();
	g_pTextureManager->Destroy();
	g_pDeviceManager->Destroy();
}

void cMainGame::Setup()
{
	m_pCamera = new cCamera;
	m_pCamera->Setup();

	m_pGrid = new cGrid;
	m_pGrid->Setup(30);

	D3DXFONT_DESC fd;
	ZeroMemory(&fd,sizeof(D3DXFONT_DESC));
	fd.Height			= 25;
	fd.Width			= 12;
	fd.Weight			= FW_NORMAL;
	fd.Italic			= false;
	fd.CharSet			= DEFAULT_CHARSET;
	fd.OutputPrecision  = OUT_DEFAULT_PRECIS;
	fd.PitchAndFamily   = FF_DONTCARE;
	AddFontResource("umberto.ttf");
	strcpy_s(fd.FaceName, "umberto");	//글꼴 스타일
	D3DXCreateFontIndirect(g_pD3DDevice, &fd, &m_pFont);

	LPDIRECT3DTEXTURE9 pTexture = NULL;
	D3DXCreateTextureFromFileEx(
		g_pD3DDevice,
		"인상이가 좋아하는 설현.png", 
		D3DX_DEFAULT_NONPOW2,
		D3DX_DEFAULT_NONPOW2,
		D3DX_DEFAULT,
		0,
		D3DFMT_UNKNOWN, 
		D3DPOOL_MANAGED, 
		D3DX_FILTER_NONE, 
		D3DX_DEFAULT, 
		0,
		&m_stImageInfo,
		NULL,
		&m_pTexture );

	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);

	SetLight();
}

void cMainGame::Update()
{
	g_pTimeManager->Update();
	
	if(m_pCamera)
		m_pCamera->Update(NULL/*m_pController->GetPosition()*/);
	
	g_pAutoReleasePool->Drain();
}

void cMainGame::Render()
{
	g_pD3DDevice->Clear(NULL,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(47, 121, 112),
		//D3DCOLOR_XRGB(0, 0, 255),
		1.0f, 0);

	g_pD3DDevice->BeginScene();

	// 그림을 그린다.
	m_pGrid->Render();
	
	
	static float fAngle = 0.0f;
	fAngle += 0.1f;
	D3DXMATRIXA16 matWorld;
	D3DXMatrixTranslation(&matWorld, 100, 50, 0);
	//D3DXMatrixRotationZ(&matWorld, fAngle);

	RECT rc;
	SetRect(&rc, 0, 0, m_stImageInfo.Width, m_stImageInfo.Height);

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	m_pSprite->SetTransform(&matWorld);
	m_pSprite->Draw(m_pTexture,
		&rc,
		&D3DXVECTOR3(0, 0, 0),
		&D3DXVECTOR3(0, 0, 0),
		D3DCOLOR_XRGB(255, 0, 0));
	m_pSprite->End();


	std::string s("!@# 123 ABC");

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	//m_pSprite->SetTransform(&matWorld);
	D3DXMatrixScaling(&matWorld, 2, 2, 1);
	m_pSprite->SetTransform(&matWorld);
	m_pFont->DrawTextA(m_pSprite, 
		s.c_str(),
		s.length(),
		&rc, 
		DT_TOP | DT_LEFT | DT_WORDBREAK, 
		D3DCOLOR_XRGB(255, 255, 255));
	m_pSprite->End();


	g_pD3DDevice->EndScene();

	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void cMainGame::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	if (m_pCamera)
	{
		m_pCamera->WndProc(hWnd, message, wParam, lParam);
	}

	switch(message)
	{
	case WM_KEYDOWN:
		{
			switch(wParam)
			{
			case VK_SPACE:
				{
				}
				break;
			}
		}
		break;

	case WM_LBUTTONDOWN:
		{
		}
		break;
	}
}

void cMainGame::SetLight()
{
	D3DLIGHT9 stLight;
	ZeroMemory(&stLight, sizeof(D3DLIGHT9));
	stLight.Type = D3DLIGHT_DIRECTIONAL;
	stLight.Ambient = stLight.Diffuse = stLight.Specular = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	stLight.Direction = D3DXVECTOR3( 1,-1, 1);
	g_pD3DDevice->SetLight(0, &stLight);
	g_pD3DDevice->LightEnable(0, true);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	g_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
}

