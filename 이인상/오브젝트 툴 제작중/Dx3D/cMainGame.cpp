#include "StdAfx.h"
#include "cMainGame.h"

cMainGame::cMainGame(void)
	: m_pCamera(NULL)
	, m_pRoot(NULL)
	, m_pGrid(NULL)
	, m_pController(NULL)
	, m_pSky(NULL)
{
}

cMainGame::~cMainGame(void)
{
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);
	SAFE_DELETE(m_pController);

	/* --------- */
	SAFE_RELEASE(m_pSky);
	/* --------- */




	g_pObjectManager->Destroy();
	g_pTextureManager->Destroy();
	g_pDeviceManager->Destroy();
	Animation_M->Destroy();
	g_pFontManager->Release();
}


void cMainGame::Setup()
{
	m_pCamera = new cCamera;
	m_pCamera->Setup();

	m_pController = new cCrtController;
	m_pController->Setup();

	m_pGrid = new cGrid;
	m_pGrid->Setup(30);

	SetLight();
	//_Animation->SetAnimationIndex(4);

	/* --------- */
	m_pSky = new cSky;
	m_pSky->Init();
	/* --------- */





	/* --------- */
	m_UiMain = new UiMain;
	m_UiMain->Setup();

	m_Objtool = new cObjtool;
	m_Objtool->Setup();
	/* --------- */
}

void cMainGame::Update()
{
	g_pTimeManager->Update();


	if (m_pController)
		m_pController->Update(NULL);


	if (m_pCamera) {
		m_pCamera->Update(&m_pController->GetPosition());

	}

	/* --------- */
	if (m_UiMain)
		m_UiMain->Update();

	m_Objtool->Update();
	m_Objtool->ObjMove();
	/* --------- */

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

	/* ---------- */
	if (m_pSky)
	{
		m_pSky->Render(m_pController->GetWorldTM(), m_pCamera->GetViewProjMatrix());
	}

	/* ---------- */


	// 그림을 그린다.
	m_pGrid->Render();

	D3DXMATRIXA16 matI, matT;
	D3DXMatrixIdentity(&matI);
	D3DXMatrixIdentity(&matT);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matI);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);


	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);

	/* ---------- */
	m_Objtool->Render();
	m_UiMain->Render();
	/* ---------- */

	g_pD3DDevice->EndScene();

	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void cMainGame::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pCamera)
	{
		m_pCamera->WndProc(hWnd, message, wParam, lParam);
	}
}

void cMainGame::SetLight()
{
	D3DLIGHT9 stLight;
	ZeroMemory(&stLight, sizeof(D3DLIGHT9));
	stLight.Type = D3DLIGHT_DIRECTIONAL;
	stLight.Ambient = stLight.Diffuse = stLight.Specular = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	stLight.Direction = D3DXVECTOR3(1, -1, 1);
	g_pD3DDevice->SetLight(0, &stLight);
	g_pD3DDevice->LightEnable(0, true);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	g_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
}
