#include "StdAfx.h"
#include "cMainGame.h"
#include "cCubeP.h"
#include "cCamera.h"
#include "cGrid.h"
#include "Shadow.h"

cMainGame::cMainGame(void)
	: m_pCube(NULL)
	, m_pCamera(NULL)
	, m_pGrid(NULL)
	, m_pShadow(NULL)
{
}

cMainGame::~cMainGame(void)
{
	SAFE_DELETE(m_pCube);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);
	SAFE_DELETE(m_pShadow);
	g_pDeviceManager->Destroy();
}

void cMainGame::Setup()
{
	m_pCube = new cCubeP;
	m_pCube->Setup();

	m_pCamera = new cCamera;
	m_pCamera->Setup();

	m_pGrid = new cGrid;
	m_pGrid->Setup(30);

	m_pShadow = new Shadow;
	m_pShadow->SetUp();
	m_pShadow->LoadAsset();
	m_pShadow->InitShadowRenderTarget();
	
}

void cMainGame::Update()
{
	if(m_pCamera)
		m_pCamera->Update();
	if (m_pShadow)
		m_pShadow->Update();
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

	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	// 그림을 그린다.
	//if(m_pGrid)
	//	m_pGrid->Render();

	if(m_pCube)
		m_pCube->Render();

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	if (m_pShadow)
		m_pShadow->RenderScene(m_pCamera->GetViewProjMatrix(), *m_pCamera->GetEyesVecter());

	g_pD3DDevice->EndScene();

	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void cMainGame::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	if(m_pCamera)
	{
		m_pCamera->WndProc(hWnd, message, wParam, lParam);
	}
}
