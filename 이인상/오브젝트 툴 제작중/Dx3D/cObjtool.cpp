#include "stdafx.h"
#include "cObjtool.h"


cObjtool::cObjtool()
	:Xplus(0)
	,Yplus(0)
	,Zplus(0)
	,scaling(0.01f)
	,Rotate(0)
{
}


cObjtool::~cObjtool()
{
}

void cObjtool::Setup()
{
	o_Object = new cObjectXfile;
	o_Object->Load("./Object/", "rock.X");
	o_Object->SetPosition(D3DXVECTOR3(0, 0, 0));
	o_Object->SetScale(D3DXVECTOR3(0.01f, 0.01f, 0.01f));
}

void cObjtool::Update()
{
	o_Object->Update();

	for (size_t i = 0; i < o_Image.size(); i++)
	{
		o_Image[i]->Update();
	}
}

void cObjtool::Render()
{
	o_Object->Render();
	for (size_t i = 0; i < o_Image.size(); i++)
	{
		o_Image[i]->Render();
	}
}

void cObjtool::ObjMove()
{
	if (g_pkeyManager->isStayKeyDown(VK_F1))
	{
		Yplus++;
	}
	else if (g_pkeyManager->isStayKeyDown(VK_F2))
	{
		Yplus--;
	}

	if (g_pkeyManager->isStayKeyDown(VK_LEFT))
	{
		Xplus--;
	}
	else if (g_pkeyManager->isStayKeyDown(VK_RIGHT))
	{
		Xplus++;
	}

	if (g_pkeyManager->isStayKeyDown(VK_UP))
	{
		Zplus++;
	}
	else if (g_pkeyManager->isStayKeyDown(VK_DOWN))
	{
		Zplus--;
	}

	if (g_pkeyManager->isStayKeyDown(VK_NUMPAD8))
	{
		scaling += 0.001f;
	}
	else if (g_pkeyManager->isStayKeyDown(VK_NUMPAD2))
	{
		scaling -= 0.001f;
	}

	if (g_pkeyManager->isOnceKeyDown('Z'))
	{
		ObjSetting();
	}

	D3DXMATRIXA16 rotation;
	o_Object->SetScale(D3DXVECTOR3(scaling, scaling, scaling));
	o_Object->SetPosition(D3DXVECTOR3(Xplus, Yplus, Zplus));
}

void cObjtool::Pasing()
{
	FILE *fp = fopen("오브젝트파싱.txt", "a");
}

void cObjtool::ObjSetting()
{
	o_Objectswap = new cObjectXfile;
	o_Objectswap->Load("./Object/", "rock.X");
	o_Objectswap->SetPosition(o_Object->GetPosition());
	o_Objectswap->SetScale(o_Object->GetScale());

	o_Image.push_back(o_Objectswap);

	OutputSave();
}

void cObjtool::OutputSave()
{
	FILE *wfp = fopen("오브젝트파싱.txt", "a");
	int Num;
	Num = Objnum;

	float posx, posy, posz;
	posx = o_Object->GetPosition().x;
	posy = o_Object->GetPosition().y;
	posz = o_Object->GetPosition().z;

	float scalex, scaley, scalez;
	scalex = o_Object->GetScale().x;
	scaley = o_Object->GetScale().y;
	scalez = o_Object->GetScale().z;

	fprintf(wfp, "%d %f %f %f %f %f %f\n", Num, posx, posy, posz, scalex, scaley, scalez);

	fclose(wfp);

	
}