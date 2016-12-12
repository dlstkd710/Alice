#pragma once
#include "Image.h"
#include "cUIButton.h"

class UiMain
{
private:
	Image* background1;
	Image* Hour;
	Image* clock;
	Image* Min;

	Image* background2;

	Image* HpBar[21];

	Image* Titleground;

	Image* background3;

	RECT Canclerc;
	Image* Canclebutton1;
	Image* Canclebutton2;

	RECT Enterrc;
	Image* Enterbutton1;
	Image* Enterbutton2;

	RECT Titlerc;
	Image* Titlebutton1;
	Image* Titlebutton2;
	Image* Titlebutton3;

	bool TitleSelect;

	RECT Startrc;
	Image* Startbutton1;
	Image* Startbutton2;

	RECT Setrc;
	Image* Setbutton1;
	Image* Setbutton2;

	RECT Quitrc;
	Image* Quitbutton1;
	Image* Quitbutton2;

	float moveHour = 0.0f;
	float moveMin = 0.0f;

	int secne;

	int TestHp = 100;

public:
	bool Uisecne;
public:
	UiMain();
	~UiMain();

	void Setup();
	void Update();
	void Render();

	void secne1Render();
	void secne2Render();
	void TitleRender();
	void secne3Render();

	void HpSetUp();
	void HpRender();

};

