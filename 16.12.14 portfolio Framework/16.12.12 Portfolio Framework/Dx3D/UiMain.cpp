#include "stdafx.h"
#include "UiMain.h"


UiMain::UiMain()
	: secne(1)
	, Uisecne(true)
	, TitleSelect(false)
	, U_Volume(100)
{

}


UiMain::~UiMain()
{
}

void UiMain::Setup()
{
	g_pSoundManager->addSound("오프닝 사운드", "./sound/엘리스 오프닝 테마.wav", true);
	g_pSoundManager->addSound("VOT 사운드", "./sound/엘리스 VOT 테마.wav", true);

	background1 = new Image;
	background1->Setup("./Image/엘리스시작화면1.png", 0, 0, 0);

	Hour = new Image;
	Hour->Setup("./Image/시침.png", -6, -24, 0);

	Min = new Image;
	Min->Setup("./Image/분침.png", -6, -5, 0);

	clock = new Image;
	clock->Setup("./Image/시계.png", 624, 484, 0);

	background2 = new Image;
	background2->Setup("./Image/엘리스시작화면2.png", 0, 0, 0);

	Titleground = new Image;
	Titleground->Setup("./Image/타이틀모음/프로필화면.png", 0, 0, 0);

	Canclerc = cUt::RectMake(385, 540, 235, 45);
	Canclebutton1 = new Image;
	Canclebutton1->Setup("./Image/타이틀모음/캔슬버튼1.png", 385, 585, 0);

	Canclebutton2 = new Image;
	Canclebutton2->Setup("./Image/타이틀모음/캔슬버튼2.png", 385, 585, 0);

	Enterrc = cUt::RectMake(739, 540, 235, 45);
	Enterbutton1 = new Image;
	Enterbutton1->Setup("./Image/타이틀모음/엔터버튼1.png", 739, 585, 0);

	Enterbutton2 = new Image;
	Enterbutton2->Setup("./Image/타이틀모음/엔터버튼2.png", 739, 585, 0);

	Titlerc = cUt::RectMake(358, 198, 668, 99);
	Titlebutton1 = new Image;
	Titlebutton1->Setup("./Image/타이틀모음/타이틀버튼1.png", 358, 198, 0);

	Titlebutton2 = new Image;
	Titlebutton2->Setup("./Image/타이틀모음/타이틀버튼2.png", 358, 198, 0);

	Titlebutton3 = new Image;
	Titlebutton3->Setup("./Image/타이틀모음/타이틀버튼3.png", 358, 198, 0);

	background3 = new Image;
	background3->Setup("./Image/씬4/엘리스 시작화면3.png", 0, 0, 0);

	Startrc = cUt::RectMake(0, 607, 200, 80);
	Startbutton1 = new Image;
	Startbutton1->Setup("./Image/씬4/게임시작1.png", 0, 647, 0);
	Startbutton2 = new Image;
	Startbutton2->Setup("./Image/씬4/게임시작2.png", 0, 647, 0);

	Setrc = cUt::RectMake(217, 607, 200, 80);
	Setbutton1 = new Image;
	Setbutton1->Setup("./Image/씬4/게임설정1.png", 217, 647, 0);
	Setbutton2 = new Image;
	Setbutton2->Setup("./Image/씬4/게임설정2.png", 217, 647, 0);

	Quitrc = cUt::RectMake(434, 607, 200, 80);
	Quitbutton1 = new Image;
	Quitbutton1->Setup("./Image/씬4/게임종료1.png", 434, 647, 0);
	Quitbutton2 = new Image;
	Quitbutton2->Setup("./Image/씬4/게임종료2.png", 434, 647, 0);

	Setground = new Image;
	Setground->Setup("./Image/게임설정/게임설정창.png", 0, 0, 0);

	Okrc = cUt::RectMake(966, 618, 400, 100);
	Volumebuttonrc = cUt::RectMakeCenter(417, 314, 48, 48);
	Volumerc = cUt::RectMake(117, 285, 300, 30);
	Okbutton1 = new Image;
	Okbutton1->Setup("./Image/게임설정/확인버튼1.png", 966, 668, 0);
	Okbutton2 = new Image;
	Okbutton2->Setup("./Image/게임설정/확인버튼2.png", 966, 668, 0);
	Volumebutton = new Image;
	Volumebutton->Setup("./Image/게임설정/음량버튼.png", Volumebuttonrc.left, Volumebuttonrc.top, 0);

	HpSetUp();
}

void UiMain::Update()
{
	if (Uisecne)
	{
		g_pSoundManager->play("오프닝 사운드", true, U_Volume);
	}

	if (g_pkeyManager->isOnceKeyDown(VK_TAB) && Uisecne && secne != 6)
	{
		secne++;
	}

	if (secne == 5)
	{
		g_pSoundManager->stop("오프닝 사운드");
		g_pSoundManager->play("VOT 사운드", true, U_Volume);
		Uisecne = false;
	}
}

void UiMain::Render()
{
	HpRender();

	if (secne == 1) secne1Render();
	if (secne == 2) secne2Render();
	if (secne == 3) TitleRender();
	if (secne == 4) secne3Render();
	if (secne == 6) SettingRender();
}

void UiMain::secne1Render()
{
	background1->Render();

	clock->Render();

	moveHour += 0.01f;
	moveMin += 0.05f;
	if (moveHour >= D3DX_PI * 2) moveHour = 0.0f;
	if (moveMin >= D3DX_PI * 2) moveMin = 0.0f;

	D3DXMATRIXA16 matworld;
	D3DXMATRIXA16 matworld2;

	D3DXMATRIXA16 rotate;
	D3DXMATRIXA16 rotate2;
	D3DXMatrixRotationZ(&rotate, moveHour);
	D3DXMatrixRotationZ(&rotate2, moveMin);

	D3DXMATRIXA16 trans;
	D3DXMatrixTranslation(&trans, 692, 545, 0);

	matworld = rotate * trans;
	matworld2 = rotate2 * trans;

	Min->Render(&matworld2);
	Hour->Render(&matworld);
}

void UiMain::secne2Render()
{
	background2->Render();
}

void UiMain::TitleRender()
{
	Titleground->Render();
	Canclebutton1->Render();
	Enterbutton1->Render();
	Titlebutton1->Render();

	if (PtInRect(&Titlerc, cUt::GetMousePos()))
	{
		Titlebutton2->Render();
		if (g_pkeyManager->isOnceKeyDown(VK_LBUTTON)) TitleSelect = true;
	}

	if (TitleSelect) Titlebutton3->Render();

	if (PtInRect(&Canclerc, cUt::GetMousePos()))
	{
		Canclebutton2->Render();
		if (g_pkeyManager->isOnceKeyDown(VK_LBUTTON)) secne--;
	}

	if (PtInRect(&Enterrc, cUt::GetMousePos()))
	{
		Enterbutton2->Render();
		if (g_pkeyManager->isOnceKeyDown(VK_LBUTTON) && TitleSelect) secne++;
	}
}

void UiMain::secne3Render()
{
	background3->Render();

	Startbutton1->Render();
	if (PtInRect(&Startrc, cUt::GetMousePos()))
	{
		Startbutton2->Render();
		if (g_pkeyManager->isOnceKeyDown(VK_LBUTTON)) secne++;
	}

	Setbutton1->Render();
	if (PtInRect(&Setrc, cUt::GetMousePos()))
	{
		Setbutton2->Render();
		if (g_pkeyManager->isOnceKeyDown(VK_LBUTTON)) secne = 6;
	}

	Quitbutton1->Render();
	if (PtInRect(&Quitrc, cUt::GetMousePos()))
	{
		Quitbutton2->Render();
		if (g_pkeyManager->isOnceKeyDown(VK_LBUTTON)) PostQuitMessage(0);
	}
}

void UiMain::SettingRender()
{
	Setground->Render();
	Okbutton1->Render();

	if (g_pkeyManager->isStayKeyDown(VK_LBUTTON) && PtInRect(&Volumerc, cUt::GetMousePos()))
	{
		Volumebuttonrc.left = cUt::GetMousePos().x - 24;
		U_Volume = cUt::GetMousePos().x - 230;
	}
	Volumebutton->Render(Volumebuttonrc.left, Volumebuttonrc.top, 0);

	if (PtInRect(&Okrc, cUt::GetMousePos()))
	{
		Okbutton2->Render();
		if (g_pkeyManager->isOnceKeyDown(VK_LBUTTON)) secne = 4;
	}
}

void UiMain::HpSetUp()
{
	for (int i = 0; i < 21; i++)
	{
		HpBar[i] = new Image;
	}

	HpBar[0]->Setup("./Image/체력모음/체력바.png", 30, 0, 0);
	HpBar[1]->Setup("./Image/체력모음/체력바5.png", 30, 0, 0);
	HpBar[2]->Setup("./Image/체력모음/체력바10.png", 30, 0, 0);
	HpBar[3]->Setup("./Image/체력모음/체력바15.png", 30, 0, 0);
	HpBar[4]->Setup("./Image/체력모음/체력바20.png", 30, 0, 0);
	HpBar[5]->Setup("./Image/체력모음/체력바25.png", 30, 0, 0);
	HpBar[6]->Setup("./Image/체력모음/체력바30.png", 30, 0, 0);
	HpBar[7]->Setup("./Image/체력모음/체력바35.png", 30, 0, 0);
	HpBar[8]->Setup("./Image/체력모음/체력바40.png", 30, 0, 0);
	HpBar[9]->Setup("./Image/체력모음/체력바45.png", 30, 0, 0);
	HpBar[10]->Setup("./Image/체력모음/체력바50.png", 30, 0, 0);
	HpBar[11]->Setup("./Image/체력모음/체력바55.png", 30, 0, 0);
	HpBar[12]->Setup("./Image/체력모음/체력바60.png", 30, 0, 0);
	HpBar[13]->Setup("./Image/체력모음/체력바65.png", 30, 0, 0);
	HpBar[14]->Setup("./Image/체력모음/체력바70.png", 30, 0, 0);
	HpBar[15]->Setup("./Image/체력모음/체력바75.png", 30, 0, 0);
	HpBar[16]->Setup("./Image/체력모음/체력바80.png", 30, 0, 0);
	HpBar[17]->Setup("./Image/체력모음/체력바85.png", 30, 0, 0);
	HpBar[18]->Setup("./Image/체력모음/체력바90.png", 30, 0, 0);
	HpBar[19]->Setup("./Image/체력모음/체력바95.png", 30, 0, 0);
	HpBar[20]->Setup("./Image/체력모음/체력바100.png", 30, 0, 0);
}

void UiMain::HpRender()
{
	if (TestHp == 0) HpBar[0]->Render();
	if (TestHp == 5) HpBar[1]->Render();
	if (TestHp == 10) HpBar[2]->Render();
	if (TestHp == 15) HpBar[3]->Render();
	if (TestHp == 20) HpBar[4]->Render();
	if (TestHp == 25) HpBar[5]->Render();
	if (TestHp == 30) HpBar[6]->Render();
	if (TestHp == 35) HpBar[7]->Render();
	if (TestHp == 40) HpBar[8]->Render();
	if (TestHp == 45) HpBar[9]->Render();
	if (TestHp == 50) HpBar[10]->Render();

	if (TestHp == 55) HpBar[11]->Render();
	if (TestHp == 60) HpBar[12]->Render();
	if (TestHp == 65) HpBar[13]->Render();
	if (TestHp == 70) HpBar[14]->Render();
	if (TestHp == 75) HpBar[15]->Render();
	if (TestHp == 80) HpBar[16]->Render();
	if (TestHp == 85) HpBar[17]->Render();
	if (TestHp == 90) HpBar[18]->Render();
	if (TestHp == 95) HpBar[19]->Render();
	if (TestHp == 100) HpBar[20]->Render();
}