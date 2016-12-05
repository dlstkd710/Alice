#include "StdAfx.h"
#include "cMainGame.h"

#define RADIUS 0.6f
#define DELTA_Y 0.1f

cMainGame::cMainGame(void)
	: m_pCamera(NULL)
	, m_pRoot(NULL)
	, m_pGrid(NULL)
	, m_pController(NULL)
	, m_pPyramid(NULL)
	, m_pMap(NULL)
	, m_pMesh(NULL)
	, m_pMapMesh(NULL)
	, m_Terrrain(NULL)
	, m_fSpeed(100.f)
	, m_fSpeedWalk(1.f / 5.7f)
	, m_dwCurrentTrack(0)
	, m_dTimeCurrent(0.0)
	, m_PickPos(0, 0 ,0)
	, taget(0,0 ,0)
	, n(0)
	, m(0)
	, a(false)
	, TestImage(NULL)
{
	D3DXMatrixIdentity(&mat);
	imagePt = cUt::PointMake(0, 0);
}

cMainGame::~cMainGame(void)
{

	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pRoot);
	SAFE_DELETE(m_pGrid);
	SAFE_DELETE(m_pController);
	SAFE_DELETE(m_Terrrain);
	SAFE_DELETE(m_FrustumClling);
	SAFE_DELETE(TestImage);

	SAFE_RELEASE(m_pPyramid);
	SAFE_RELEASE(m_pMap);
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pMapMesh);

	for each (auto p in m_vecMtlTex)
	{
		SAFE_RELEASE(p);
	}
	for each(auto p in m_vecSkinnedMesh)
	{
		SAFE_DELETE(p);
	}
	g_pObjectManager->Destroy();
	g_pTextureManager->Destroy();
	g_pDeviceManager->Destroy();
	Animation_M->Destroy();
	g_pFontManager->Release();

}


void cMainGame::Setup()
{

	D3DXCreateSphere(g_pD3DDevice, RADIUS, 20, 20, &m_pMesh, NULL);


	D3DXMATRIXA16 matS, matR, matT, mat;
	D3DXMatrixScaling(&matS, 0.01f, 0.01f, 0.01f);
	D3DXMatrixRotationX(&matR, -D3DX_PI / 2.0f);
	mat = matS * matR;

	cObjMap* pObjMap = new cObjMap;
	pObjMap->Load("./obj/Map.obj", "./obj/map_surface.obj", &mat);

	cObjLoader objloader;
	m_pMapMesh = objloader.Load("obj/Map.obj", m_vecMtlTex, &mat);

	m_pCamera = new cCamera;
	m_pCamera->Setup();

	m_pController = new cCrtController;
	m_pController->Setup();

	m_pGrid = new cGrid;
	m_pGrid->Setup(30);

	m_Terrrain = new cTerrain;
	m_Terrrain->Heightmap("./height.bmp", "./terrain.jpg", 1);
	m_pMap = m_Terrrain;


	m_CharacterClass = new CharacterClass;
	m_CharacterClass->SetUp();


	m_cText = new cText;
	m_cText->Setup();
	m_cText->fpsSetup();
	D3DXMatrixScaling(&matS, 0.1f, 1.0f, 0.1f);
	D3DXMatrixRotationX(&matR, D3DX_PI / 2.0f);
	D3DXMatrixTranslation(&matT, 0, 0, 0.5f);
	mat = matS * matR * matT;
	m_pPyramid = new cPyramid;
	m_pPyramid->Setup(D3DCOLOR_XRGB(255, 255, 255), &mat);

	SetLight();
}

void cMainGame::Update()
{
	g_pTimeManager->Update();
	m_cText->fpsUpdate(g_pTimeManager->GetDeltaTime());
	m_cText->update();

	if (m_pController)
		//m_pController->Update(m_pMap);

	if (m_CharacterClass)
		m_CharacterClass->Controll(m_pMap);


	if (m_pPyramid)
	{
		m_pPyramid->SetDirection(m_pController->GetDirection());
		m_pPyramid->SetPosition(m_pController->GetPosition());
		m_pPyramid->Update();
	}

	if (m_pCamera) {
		m_pCamera->Update(&m_CharacterClass->GetPosition());
	}


	
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

	m_cText->render();
	m_cText->fpsRender();

	if (m_pMap)
		m_pMap->Render();

	if (m_CharacterClass)
		m_CharacterClass->Render();


	D3DXMATRIXA16 matWorld;
	if (g_pkeyManager->isStayKeyDown(VK_RBUTTON))
	{
		POINT ptMt = cUt::GetMousePos();
		imagePt = ptMt;
	}
	D3DXMatrixTranslation(&matWorld, imagePt.x, imagePt.y, 0);

	//if (TestImage)
	//	TestImage->Render(&matWorld);

	g_pD3DDevice->EndScene();

	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void cMainGame::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
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
	stLight.Direction = D3DXVECTOR3( 1,-1, 1);
	g_pD3DDevice->SetLight(0, &stLight);
	g_pD3DDevice->LightEnable(0, true);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	g_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
}

//-----------------------------------------------------------------------------
void cMainGame::SetMoveKey(int intdex)
{
	DWORD dwNewTrack = (m_dwCurrentTrack == 0 ? 1 : 0);
	LPD3DXANIMATIONCONTROLLER pAC;
	LPD3DXANIMATIONSET pAS;
	_Animation->GetAnimController(&pAC);

	pAC->GetAnimationSet(intdex, &pAS);

	pAC->SetTrackAnimationSet(dwNewTrack, pAS);


	double b = 0;
	b = pAS->GetPeriod();

	pAC->UnkeyAllTrackEvents(m_dwCurrentTrack);
	pAC->UnkeyAllTrackEvents(dwNewTrack);

	pAC->KeyTrackEnable(m_dwCurrentTrack, FALSE, pAC->GetTime() + b);
	pAC->KeyTrackSpeed(m_dwCurrentTrack, 0.0f, pAC->GetTime(), b, D3DXTRANSITION_LINEAR);
	pAC->KeyTrackWeight(m_dwCurrentTrack, 0.0f, pAC->GetTime(), b, D3DXTRANSITION_LINEAR);
	pAC->SetTrackEnable(dwNewTrack, TRUE);
	pAC->KeyTrackSpeed(dwNewTrack, 1.0f, pAC->GetTime(), b, D3DXTRANSITION_LINEAR);
	pAC->KeyTrackWeight(dwNewTrack, 1.0f, pAC->GetTime(), b, D3DXTRANSITION_LINEAR);

	m_dwCurrentTrack = dwNewTrack;

	pAC->ResetTime();

	pAC->Release();
}