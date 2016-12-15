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
	g_pSoundManager->addSound("������ ����", "./sound/������ ������ �׸�.wav", true);
	g_pSoundManager->addSound("VOT ����", "./sound/������ VOT �׸�.wav", true);

	background1 = new Image;
	background1->Setup("./Image/����������ȭ��1.png", 0, 0, 0);

	Hour = new Image;
	Hour->Setup("./Image/��ħ.png", -6, -24, 0);

	Min = new Image;
	Min->Setup("./Image/��ħ.png", -6, -5, 0);

	clock = new Image;
	clock->Setup("./Image/�ð�.png", 624, 484, 0);

	background2 = new Image;
	background2->Setup("./Image/����������ȭ��2.png", 0, 0, 0);

	Titleground = new Image;
	Titleground->Setup("./Image/Ÿ��Ʋ����/������ȭ��.png", 0, 0, 0);

	Canclerc = cUt::RectMake(385, 540, 235, 45);
	Canclebutton1 = new Image;
	Canclebutton1->Setup("./Image/Ÿ��Ʋ����/ĵ����ư1.png", 385, 585, 0);

	Canclebutton2 = new Image;
	Canclebutton2->Setup("./Image/Ÿ��Ʋ����/ĵ����ư2.png", 385, 585, 0);

	Enterrc = cUt::RectMake(739, 540, 235, 45);
	Enterbutton1 = new Image;
	Enterbutton1->Setup("./Image/Ÿ��Ʋ����/���͹�ư1.png", 739, 585, 0);

	Enterbutton2 = new Image;
	Enterbutton2->Setup("./Image/Ÿ��Ʋ����/���͹�ư2.png", 739, 585, 0);

	Titlerc = cUt::RectMake(358, 198, 668, 99);
	Titlebutton1 = new Image;
	Titlebutton1->Setup("./Image/Ÿ��Ʋ����/Ÿ��Ʋ��ư1.png", 358, 198, 0);

	Titlebutton2 = new Image;
	Titlebutton2->Setup("./Image/Ÿ��Ʋ����/Ÿ��Ʋ��ư2.png", 358, 198, 0);

	Titlebutton3 = new Image;
	Titlebutton3->Setup("./Image/Ÿ��Ʋ����/Ÿ��Ʋ��ư3.png", 358, 198, 0);

	background3 = new Image;
	background3->Setup("./Image/��4/������ ����ȭ��3.png", 0, 0, 0);

	Startrc = cUt::RectMake(0, 607, 200, 80);
	Startbutton1 = new Image;
	Startbutton1->Setup("./Image/��4/���ӽ���1.png", 0, 647, 0);
	Startbutton2 = new Image;
	Startbutton2->Setup("./Image/��4/���ӽ���2.png", 0, 647, 0);

	Setrc = cUt::RectMake(217, 607, 200, 80);
	Setbutton1 = new Image;
	Setbutton1->Setup("./Image/��4/���Ӽ���1.png", 217, 647, 0);
	Setbutton2 = new Image;
	Setbutton2->Setup("./Image/��4/���Ӽ���2.png", 217, 647, 0);

	Quitrc = cUt::RectMake(434, 607, 200, 80);
	Quitbutton1 = new Image;
	Quitbutton1->Setup("./Image/��4/��������1.png", 434, 647, 0);
	Quitbutton2 = new Image;
	Quitbutton2->Setup("./Image/��4/��������2.png", 434, 647, 0);

	Setground = new Image;
	Setground->Setup("./Image/���Ӽ���/���Ӽ���â.png", 0, 0, 0);

	Okrc = cUt::RectMake(966, 618, 400, 100);
	Volumebuttonrc = cUt::RectMakeCenter(417, 314, 48, 48);
	Volumerc = cUt::RectMake(117, 285, 300, 30);
	Okbutton1 = new Image;
	Okbutton1->Setup("./Image/���Ӽ���/Ȯ�ι�ư1.png", 966, 668, 0);
	Okbutton2 = new Image;
	Okbutton2->Setup("./Image/���Ӽ���/Ȯ�ι�ư2.png", 966, 668, 0);
	Volumebutton = new Image;
	Volumebutton->Setup("./Image/���Ӽ���/������ư.png", Volumebuttonrc.left, Volumebuttonrc.top, 0);

	HpSetUp();
}

void UiMain::Update()
{
	if (Uisecne)
	{
		g_pSoundManager->play("������ ����", true, U_Volume);
	}

	if (g_pkeyManager->isOnceKeyDown(VK_TAB) && Uisecne && secne != 6)
	{
		secne++;
	}

	if (secne == 5)
	{
		g_pSoundManager->stop("������ ����");
		g_pSoundManager->play("VOT ����", true, U_Volume);
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

	HpBar[0]->Setup("./Image/ü�¸���/ü�¹�.png", 30, 0, 0);
	HpBar[1]->Setup("./Image/ü�¸���/ü�¹�5.png", 30, 0, 0);
	HpBar[2]->Setup("./Image/ü�¸���/ü�¹�10.png", 30, 0, 0);
	HpBar[3]->Setup("./Image/ü�¸���/ü�¹�15.png", 30, 0, 0);
	HpBar[4]->Setup("./Image/ü�¸���/ü�¹�20.png", 30, 0, 0);
	HpBar[5]->Setup("./Image/ü�¸���/ü�¹�25.png", 30, 0, 0);
	HpBar[6]->Setup("./Image/ü�¸���/ü�¹�30.png", 30, 0, 0);
	HpBar[7]->Setup("./Image/ü�¸���/ü�¹�35.png", 30, 0, 0);
	HpBar[8]->Setup("./Image/ü�¸���/ü�¹�40.png", 30, 0, 0);
	HpBar[9]->Setup("./Image/ü�¸���/ü�¹�45.png", 30, 0, 0);
	HpBar[10]->Setup("./Image/ü�¸���/ü�¹�50.png", 30, 0, 0);
	HpBar[11]->Setup("./Image/ü�¸���/ü�¹�55.png", 30, 0, 0);
	HpBar[12]->Setup("./Image/ü�¸���/ü�¹�60.png", 30, 0, 0);
	HpBar[13]->Setup("./Image/ü�¸���/ü�¹�65.png", 30, 0, 0);
	HpBar[14]->Setup("./Image/ü�¸���/ü�¹�70.png", 30, 0, 0);
	HpBar[15]->Setup("./Image/ü�¸���/ü�¹�75.png", 30, 0, 0);
	HpBar[16]->Setup("./Image/ü�¸���/ü�¹�80.png", 30, 0, 0);
	HpBar[17]->Setup("./Image/ü�¸���/ü�¹�85.png", 30, 0, 0);
	HpBar[18]->Setup("./Image/ü�¸���/ü�¹�90.png", 30, 0, 0);
	HpBar[19]->Setup("./Image/ü�¸���/ü�¹�95.png", 30, 0, 0);
	HpBar[20]->Setup("./Image/ü�¸���/ü�¹�100.png", 30, 0, 0);
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