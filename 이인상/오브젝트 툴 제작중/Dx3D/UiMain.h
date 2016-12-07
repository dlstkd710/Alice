#pragma once
#include "Image.h"
#include "cUIButton.h"

class UiMain
{
private:
	Image* background1;
	Image* Min;

	Image* background2;

	Image* HpBar[21];

	Image* Titleground;

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

	void HpSetUp();
	void HpRender();

};

