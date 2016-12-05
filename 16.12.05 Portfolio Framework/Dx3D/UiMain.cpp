#include "stdafx.h"
#include "UiMain.h"


UiMain::UiMain()
	: secne(1)
	, Uisecne(true)
	, TitleSelect(false)
{

}


UiMain::~UiMain()
{
}

void UiMain::Setup()
{
	background1 = new Image;
	background1->Setup("./Image/����������ȭ��1.png", 0, 0, 0);

	Min = new Image;
	Min->Setup("./Image/��ħ.png", -6, -24, 0);

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

	HpSetUp();
}

void UiMain::Update()
{
	if (g_pkeyManager->isOnceKeyDown(VK_SPACE) && Uisecne)
	{
		secne++;
	}

	if (secne == 4)
	{
		Uisecne = false;
	}
}

void UiMain::Render()
{
	HpRender();

	if (secne == 1) secne1Render();
	if (secne == 2) secne2Render();
	if (secne == 3) TitleRender();
}

void UiMain::secne1Render()
{
	background1->Render();

	moveMin += 0.05f;
	if (moveMin >= D3DX_PI * 2) moveMin = 0.0f;

	D3DXMATRIXA16 matworld;

	D3DXMATRIXA16 rotate;
	D3DXMatrixRotationZ(&rotate, moveMin);

	D3DXMATRIXA16 trans;
	D3DXMatrixTranslation(&trans, 692, 545, 0);

	matworld = rotate * trans;

	Min->Render(&matworld);
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