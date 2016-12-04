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
	, m_FrustumClling(NULL)
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
	, m_pSky(NULL)
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

	/* --------- */
	SAFE_RELEASE(m_pSky);
	/* --------- */

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

	//cObjMap* pObjMap = new cObjMap;
	//pObjMap->Load("./obj/Map.obj", "./obj/map_surface.obj", &mat);
	//
	//cObjLoader objloader;
	//m_pMapMesh = objloader.Load("obj/Map.obj", m_vecMtlTex, &mat);

	m_pCamera = new cCamera;
	m_pCamera->Setup();

	m_pController = new cCrtController;
	m_pController->Setup();

	m_pGrid = new cGrid;
	m_pGrid->Setup(30);

	m_Terrrain = new Terrain;
	m_Terrrain->Load("./HeightMap.RAW", "./terrain.jpg", 1);
	m_pMap = m_Terrrain;

	m_cText = new cText;
	m_cText->Setup();
	m_cText->fpsSetup();
	D3DXMatrixScaling(&matS, 0.1f, 1.0f, 0.1f);
	D3DXMatrixRotationX(&matR, D3DX_PI / 2.0f);
	D3DXMatrixTranslation(&matT, 0, 0, 0.5f);
	mat = matS * matR * matT;
	m_pPyramid = new cPyramid;
	m_pPyramid->Setup(D3DCOLOR_XRGB(255, 255, 255), &mat);

	ZeroMemory(&m_stMtlNormal, sizeof(D3DMATERIAL9));
	m_stMtlNormal.Ambient = m_stMtlNormal.Diffuse = m_stMtlNormal.Specular = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);

	ZeroMemory(&m_stMtlPicked, sizeof(D3DMATERIAL9));
	m_stMtlPicked.Ambient = m_stMtlPicked.Diffuse = m_stMtlPicked.Specular = D3DXCOLOR(0.8f, 0.0f, 0.0f, 1.0f);

	_Animation = new cAnimation("./Zealot/", "alice1.X");

	SetLight();
	//_Animation->SetAnimationIndex(4);
	_Animation->SetPosition(D3DXVECTOR3(5, 0, 5));
	//TestImage = new Image;
	//TestImage->Setup("인상이가 좋아하는 설현.png", 0, 0, 0);




	ZeroMemory(&m_stMtlNormal, sizeof(D3DMATERIAL9));
	m_stMtlNormal.Ambient = m_stMtlNormal.Diffuse = m_stMtlNormal.Specular = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);

	ZeroMemory(&m_stMtlPicked, sizeof(D3DMATERIAL9));
	m_stMtlPicked.Ambient = m_stMtlPicked.Diffuse = m_stMtlPicked.Specular = D3DXCOLOR(0.8f, 0.0f, 0.0f, 1.0f);

	/* --------- */
	m_pSky = new cSky;
	m_pSky->Init();
	
	/* --------- */
}

void cMainGame::Update()
{
	g_pTimeManager->Update();
	m_cText->fpsUpdate(g_pTimeManager->GetDeltaTime(), _Animation->GetPosition().y);
	m_cText->update();

	if (fabs(m_PickPos.x - _Animation->GetPosition().x) <= EPSILON &&
		fabs(m_PickPos.y - _Animation->GetPosition().y) <= EPSILON &&
		fabs(m_PickPos.z - _Animation->GetPosition().z) <= EPSILON)
	{
		if (n != 4) {
	//		SetMoveKey(4);
			n = 4;
		}
	}

	ST_SPHERE q(D3DXVECTOR3(_Animation->GetPosition().x, _Animation->GetPosition().y, _Animation->GetPosition().z), RADIUS, false);



	if (m_pController)
		m_pController->Update(m_pMap);

	if (m_pPyramid)
	{
		m_pPyramid->SetDirection(m_pController->GetDirection());
		m_pPyramid->SetPosition(m_pController->GetPosition());
		m_pPyramid->Update();
	}

	if (m_pCamera) {
		m_pCamera->Update(&m_pController->GetPosition());

	}

	_Animation->SetDirection(_Animation->GetDirection() * (-1.0f));
	
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
	_Animation->UpdateAndRender(m_pController->GetWorldTM());

	D3DXMATRIXA16 matI, matT;
	D3DXMatrixIdentity(&matI);
	D3DXMatrixIdentity(&matT);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matI);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);


	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);

	m_cText->render();
	m_cText->fpsRender();

	if (m_pMap)
		m_pMap->Render();

	D3DXMATRIXA16 matWorld;
	if (g_pkeyManager->isStayKeyDown(VK_RBUTTON))
	{
		POINT ptMt = cUt::GetMousePos();
		imagePt = ptMt;
	}
	D3DXMatrixTranslation(&matWorld, (float)imagePt.x, (float)imagePt.y, 0.f);

	if (TestImage)
		TestImage->Render(&matWorld);

	

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