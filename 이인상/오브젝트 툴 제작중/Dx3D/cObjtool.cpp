#include "stdafx.h"
#include "cObjtool.h"

cObjtool::cObjtool()
	:Xplus(0)
	,Yplus(0)
	,Zplus(0)
	,scaling(0.01f)
	,RotationX(0)
	,RotationY(0)
	,RotationZ(0)
{
}


cObjtool::~cObjtool()
{
}

void cObjtool::Setup()
{
	o_Object = new cObjectXfile;
	o_Object->Load("./Object/1.바위/", "거대모스바위A.X", 0);
	o_Object->SetPosition(D3DXVECTOR3(0, 0, 0));
	o_Object->SetScale(D3DXVECTOR3(0.01f, 0.01f, 0.01f));
	o_Object->SetRotateX(0.0f);
	o_Object->SetRotateY(0.0f);
	o_Object->SetRotateZ(0.0f);
	ObjPasing();
}

void cObjtool::Update()
{
	o_Object->Update();

	for (size_t i = 0; i < o_Image.size(); i++)
	{
		o_Image[i]->Update();
	}

	ObjNumselect();
}

void cObjtool::Render()
{
	g_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000000);

	o_Object->Render();

	for (size_t i = 0; i < o_Image.size(); i++)
	{
		o_Image[i]->Render();
	}
}

void cObjtool::ObjMove()
{
	if (g_pkeyManager->isOnceKeyDown(VK_BACK))
	{
		scaling = 0.01f;
		RotationX = 0.0f;
		RotationY = 0.0f;
		RotationZ = 0.0f;
	}

	if (g_pkeyManager->isStayKeyDown(VK_F1))
	{
		Yplus += 0.5f;
	}
	else if (g_pkeyManager->isStayKeyDown(VK_F2))
	{
		Yplus -= 0.5f;
	}

	if (g_pkeyManager->isStayKeyDown(VK_LEFT))
	{
		Xplus -= 0.5f;
	}
	else if (g_pkeyManager->isStayKeyDown(VK_RIGHT))
	{
		Xplus += 0.5f;
	}

	if (g_pkeyManager->isStayKeyDown(VK_UP))
	{
		Zplus += 0.5f;
	}
	else if (g_pkeyManager->isStayKeyDown(VK_DOWN))
	{
		Zplus -= 0.5f;
	}

	if (g_pkeyManager->isStayKeyDown(VK_NUMPAD8))
	{
		scaling += 0.0001f;
	}
	else if (g_pkeyManager->isStayKeyDown(VK_NUMPAD2))
	{
		scaling -= 0.0001f;
	}

	if (g_pkeyManager->isStayKeyDown(VK_NUMPAD7))
	{
		RotationX += 0.01f;
		if (RotationX >= D3DX_PI * 2) RotationX = 0;
	}
	else if (g_pkeyManager->isStayKeyDown(VK_NUMPAD9))
	{
		RotationX -= 0.01f;
		if (RotationX <= -D3DX_PI * 2) RotationX = 0;
	}

	if (g_pkeyManager->isStayKeyDown(VK_NUMPAD4))
	{
		RotationY += 0.01f;
		if (RotationY >= D3DX_PI * 2) RotationY = 0;
	}
	else if (g_pkeyManager->isStayKeyDown(VK_NUMPAD6))
	{
		RotationY -= 0.01f;
		if (RotationY <= -D3DX_PI * 2) RotationY = 0;
	}

	if (g_pkeyManager->isStayKeyDown(VK_NUMPAD1))
	{
		RotationZ += 0.01f;
		if (RotationZ >= D3DX_PI * 2) RotationZ = 0;
	}
	else if (g_pkeyManager->isStayKeyDown(VK_NUMPAD3))
	{
		RotationZ -= 0.01f;
		if (RotationZ <= -D3DX_PI * 2) RotationZ = 0;
	}

	if (g_pkeyManager->isOnceKeyDown('Z'))
	{
		ObjSetting();
	}

	D3DXMATRIXA16 rotation;
	o_Object->SetScale(D3DXVECTOR3(scaling, scaling, scaling));
	o_Object->SetRotateX(RotationX);
	o_Object->SetRotateY(RotationY);
	o_Object->SetRotateZ(RotationZ);
	o_Object->SetPosition(D3DXVECTOR3(Xplus, Yplus, Zplus));
}

void cObjtool::ObjSetting()
{
	o_Objectswap = new cObjectXfile;
	SetObjSelect();
	o_Objectswap->SetPosition(o_Object->GetPosition());
	o_Objectswap->SetScale(o_Object->GetScale());
	o_Objectswap->SetRotateX(o_Object->GetRotateX());
	o_Objectswap->SetRotateY(o_Object->GetRotateY());
	o_Objectswap->SetRotateZ(o_Object->GetRotateZ());

	o_Image.push_back(o_Objectswap);

	OutputSave();
}

void cObjtool::SetObjSelect()
{
	if (Objnum == rock1)  o_Objectswap->Load("./Object/1.바위/", "거대모스바위A.X", 0);
	if (Objnum == rock2)  o_Objectswap->Load("./Object/1.바위/", "거대바위벽.X", 1);
	if (Objnum == rock3)  o_Objectswap->Load("./Object/1.바위/", "긴발판바위.X", 2);
	if (Objnum == rock4)  o_Objectswap->Load("./Object/1.바위/", "납작한바위.X", 3);
	if (Objnum == rock5)  o_Objectswap->Load("./Object/1.바위/", "눈물바위.X", 4);
	if (Objnum == rock6)  o_Objectswap->Load("./Object/1.바위/", "락필리아A.X", 5);
	if (Objnum == rock7)  o_Objectswap->Load("./Object/1.바위/", "락필리아B.X", 6);
	if (Objnum == rock8)  o_Objectswap->Load("./Object/1.바위/", "모스바위A.X", 7);
	if (Objnum == rock9)  o_Objectswap->Load("./Object/1.바위/", "모스바위B.X", 8);
	if (Objnum == rock10) o_Objectswap->Load("./Object/1.바위/", "모스바위C.X", 9);

	if (Objnum == rock11) o_Objectswap->Load("./Object/1.바위/", "모스바위더미.X", 10);
	if (Objnum == tree1)  o_Objectswap->Load("./Object/2.나무/", "글로리나무 그룹.X", 11);
	if (Objnum == tree2)  o_Objectswap->Load("./Object/2.나무/", "글로리나무.X", 12);
	if (Objnum == tree3)  o_Objectswap->Load("./Object/2.나무/", "글로리할로우나무.X", 13);
	if (Objnum == tree4)  o_Objectswap->Load("./Object/2.나무/", "두꺼운나무.X", 14);
	if (Objnum == tree5)  o_Objectswap->Load("./Object/2.나무/", "얇은나무.X", 15);
	if (Objnum == mush1)  o_Objectswap->Load("./Object/3.버섯/", "거미버섯.X", 16);
	if (Objnum == mush2)  o_Objectswap->Load("./Object/3.버섯/", "버섯발판.X", 17);
	if (Objnum == mush3)  o_Objectswap->Load("./Object/3.버섯/", "에일리언버섯 그룹(벽).X", 18);
	if (Objnum == mush4)  o_Objectswap->Load("./Object/3.버섯/", "에일리언버섯 그룹.X", 19);

	if (Objnum == mush5)  o_Objectswap->Load("./Object/3.버섯/", "에일리언버섯.X", 20);
	if (Objnum == mush6)  o_Objectswap->Load("./Object/3.버섯/", "체커버섯 그룹(벽).X", 21);
	if (Objnum == mush7)  o_Objectswap->Load("./Object/3.버섯/", "체커버섯 그룹.X", 22);
	if (Objnum == mush8)  o_Objectswap->Load("./Object/3.버섯/", "체커버섯.X", 23);
	if (Objnum == mush9)  o_Objectswap->Load("./Object/3.버섯/", "토드스툴A.X", 24);
	if (Objnum == mush10) o_Objectswap->Load("./Object/3.버섯/", "토드스툴B.X", 25);
	if (Objnum == flow1)  o_Objectswap->Load("./Object/4.꽃/", "꽃밭 그룹A.X", 26);
	if (Objnum == flow2)  o_Objectswap->Load("./Object/4.꽃/", "꽃밭 그룹B.X", 27);
	if (Objnum == flow3)  o_Objectswap->Load("./Object/4.꽃/", "꽃밭A.X", 28);
	if (Objnum == flow4)  o_Objectswap->Load("./Object/4.꽃/", "꽃밭B.X", 29);

	if (Objnum == flow5)  o_Objectswap->Load("./Object/4.꽃/", "꽃밭C.X", 30);
	if (Objnum == flow6)  o_Objectswap->Load("./Object/4.꽃/", "꽃밭D.X", 31);
	if (Objnum == flow7)  o_Objectswap->Load("./Object/4.꽃/", "나팔꽃.X", 32);
	if (Objnum == flow8)  o_Objectswap->Load("./Object/4.꽃/", "분홍공꽃.X", 33);
	if (Objnum == flow9)  o_Objectswap->Load("./Object/4.꽃/", "브루밍꽃B.X", 34);
	if (Objnum == flow10) o_Objectswap->Load("./Object/4.꽃/", "잎사귀.X", 35);
	if (Objnum == flow11) o_Objectswap->Load("./Object/4.꽃/", "절벽꽃.X", 36);
	if (Objnum == furn1)  o_Objectswap->Load("./Object/5.가구/", "그림.X", 37);
	if (Objnum == furn2)  o_Objectswap->Load("./Object/5.가구/", "나무테이블.X", 38);
	if (Objnum == furn3)  o_Objectswap->Load("./Object/5.가구/", "두체스나무.X", 39);

	if (Objnum == furn5)  o_Objectswap->Load("./Object/5.가구/", "상자.X", 41);
	if (Objnum == furn6)  o_Objectswap->Load("./Object/5.가구/", "선반.X", 42);
	if (Objnum == furn7)  o_Objectswap->Load("./Object/5.가구/", "수납장.X", 43);
	if (Objnum == furn8)  o_Objectswap->Load("./Object/5.가구/", "수납장2.X", 44);
	if (Objnum == furn9)  o_Objectswap->Load("./Object/5.가구/", "스토브.X", 45);
	if (Objnum == furn10) o_Objectswap->Load("./Object/5.가구/", "의자.X", 46);
	if (Objnum == furn11) o_Objectswap->Load("./Object/5.가구/", "접시.X", 47);
	if (Objnum == furn12) o_Objectswap->Load("./Object/5.가구/", "주방도구.X", 48);
	if (Objnum == furn13) o_Objectswap->Load("./Object/5.가구/", "책장.X", 49);

	if (Objnum == etc1)   o_Objectswap->Load("./Object/6.기타/", "달팽이껍질.X", 50);
	if (Objnum == etc2)   o_Objectswap->Load("./Object/6.기타/", "도미노 그룹.X", 51);
	if (Objnum == etc3)   o_Objectswap->Load("./Object/6.기타/", "도미노.X", 52);
	if (Objnum == etc4)   o_Objectswap->Load("./Object/6.기타/", "도미노C.X", 53);
	if (Objnum == etc5)   o_Objectswap->Load("./Object/6.기타/", "미끄럼틀 직선A.X", 54);
	if (Objnum == etc6)   o_Objectswap->Load("./Object/6.기타/", "열쇠구멍 글로리.X", 55);
	if (Objnum == etc7)   o_Objectswap->Load("./Object/6.기타/", "열쇠구멍문.X", 56);
	if (Objnum == etc8)   o_Objectswap->Load("./Object/6.기타/", "열쇠구멍터널.X", 57);
	if (Objnum == etc9)   o_Objectswap->Load("./Object/6.기타/", "주사위.X", 58);
	if (Objnum == etc10)  o_Objectswap->Load("./Object/6.기타/", "집(기억).X", 59);
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

	float scalex;
	scalex = o_Object->GetScale().x;

	float angleX, angleY, angleZ;
	angleX = o_Object->GetRotateX();
	angleY = o_Object->GetRotateY();
	angleZ = o_Object->GetRotateZ();

	fprintf(wfp, "%d %f %f %f %f %f %f %f\n", Num, posx, posy, posz, scalex, angleX, angleY, angleZ);

	fclose(wfp);
}

void cObjtool::ObjPasing()
{
	FILE *fp = NULL;
	fopen_s(&fp, "오브젝트파싱.txt", "r");
	while (!feof(fp))
	{
		float Pposx, Pposy, Pposz;
		float Pscalex;
		float PangleX, PangleY, PangleZ;
		float PminX, PminY, PminZ;
		float PmaxX, PmaxY, PmaxZ;

		char szBuf[1024] = { '\0', };
		fgets(szBuf, 1024, fp);
		if (szBuf[0] == NULL) break;

		sscanf(szBuf, "%d %f %f %f %f %f %f %f %f %f %f %f %f %f", &PNum, &Pposx, &Pposy, &Pposz, &Pscalex, &PangleX, &PangleY, &PangleZ);
		o_Objectswap = new cObjectXfile;
		ObjSelect();
		o_Objectswap->SetPosition(D3DXVECTOR3(Pposx, Pposy, Pposz));
		o_Objectswap->SetScale(D3DXVECTOR3(Pscalex, Pscalex, Pscalex));
		o_Objectswap->SetRotateX(PangleX);
		o_Objectswap->SetRotateY(PangleY);
		o_Objectswap->SetRotateZ(PangleZ);

		o_Image.push_back(o_Objectswap);
	}

	fclose(fp);
}

void cObjtool::ObjSelect()
{
	if (PNum == rock1)  o_Objectswap->Load("./Object/1.바위/", "거대모스바위A.X", 0);
	if (PNum == rock2)  o_Objectswap->Load("./Object/1.바위/", "거대바위벽.X", 1);
	if (PNum == rock3)  o_Objectswap->Load("./Object/1.바위/", "긴발판바위.X", 2);
	if (PNum == rock4)  o_Objectswap->Load("./Object/1.바위/", "납작한바위.X", 3);
	if (PNum == rock5)  o_Objectswap->Load("./Object/1.바위/", "눈물바위.X", 4);
	if (PNum == rock6)  o_Objectswap->Load("./Object/1.바위/", "락필리아A.X", 5);
	if (PNum == rock7)  o_Objectswap->Load("./Object/1.바위/", "락필리아B.X", 6);
	if (PNum == rock8)  o_Objectswap->Load("./Object/1.바위/", "모스바위A.X", 7);
	if (PNum == rock9)  o_Objectswap->Load("./Object/1.바위/", "모스바위B.X", 8);
	if (PNum == rock10) o_Objectswap->Load("./Object/1.바위/", "모스바위C.X", 9);

	if (PNum == rock11) o_Objectswap->Load("./Object/1.바위/", "모스바위더미.X", 10);
	if (PNum == tree1)  o_Objectswap->Load("./Object/2.나무/", "글로리나무 그룹.X", 11);
	if (PNum == tree2)  o_Objectswap->Load("./Object/2.나무/", "글로리나무.X", 12);
	if (PNum == tree3)  o_Objectswap->Load("./Object/2.나무/", "글로리할로우나무.X", 13);
	if (PNum == tree4)  o_Objectswap->Load("./Object/2.나무/", "두꺼운나무.X", 14);
	if (PNum == tree5)  o_Objectswap->Load("./Object/2.나무/", "얇은나무.X", 15);
	if (PNum == mush1)  o_Objectswap->Load("./Object/3.버섯/", "거미버섯.X", 16);
	if (PNum == mush2)  o_Objectswap->Load("./Object/3.버섯/", "버섯발판.X", 17);
	if (PNum == mush3)  o_Objectswap->Load("./Object/3.버섯/", "에일리언버섯 그룹(벽).X", 18);
	if (PNum == mush4)  o_Objectswap->Load("./Object/3.버섯/", "에일리언버섯 그룹.X", 19);

	if (PNum == mush5)  o_Objectswap->Load("./Object/3.버섯/", "에일리언버섯.X", 20);
	if (PNum == mush6)  o_Objectswap->Load("./Object/3.버섯/", "체커버섯 그룹(벽).X", 21);
	if (PNum == mush7)  o_Objectswap->Load("./Object/3.버섯/", "체커버섯 그룹.X", 22);
	if (PNum == mush8)  o_Objectswap->Load("./Object/3.버섯/", "체커버섯.X", 23);
	if (PNum == mush9)  o_Objectswap->Load("./Object/3.버섯/", "토드스툴A.X", 24);
	if (PNum == mush10) o_Objectswap->Load("./Object/3.버섯/", "토드스툴B.X", 25);
	if (PNum == flow1)  o_Objectswap->Load("./Object/4.꽃/", "꽃밭 그룹A.X", 26);
	if (PNum == flow2)  o_Objectswap->Load("./Object/4.꽃/", "꽃밭 그룹B.X", 27);
	if (PNum == flow3)  o_Objectswap->Load("./Object/4.꽃/", "꽃밭A.X", 28);
	if (PNum == flow4)  o_Objectswap->Load("./Object/4.꽃/", "꽃밭B.X", 29);

	if (PNum == flow5)  o_Objectswap->Load("./Object/4.꽃/", "꽃밭C.X", 30);
	if (PNum == flow6)  o_Objectswap->Load("./Object/4.꽃/", "꽃밭D.X", 31);
	if (PNum == flow7)  o_Objectswap->Load("./Object/4.꽃/", "나팔꽃.X", 32);
	if (PNum == flow8)  o_Objectswap->Load("./Object/4.꽃/", "분홍공꽃.X", 33);
	if (PNum == flow9)  o_Objectswap->Load("./Object/4.꽃/", "브루밍꽃B.X", 34);
	if (PNum == flow10) o_Objectswap->Load("./Object/4.꽃/", "잎사귀.X", 35);
	if (PNum == flow11) o_Objectswap->Load("./Object/4.꽃/", "절벽꽃.X", 36);
	if (PNum == furn1)  o_Objectswap->Load("./Object/5.가구/", "그림.X", 37);
	if (PNum == furn2)  o_Objectswap->Load("./Object/5.가구/", "나무테이블.X", 38);
	if (PNum == furn3)  o_Objectswap->Load("./Object/5.가구/", "두체스나무.X", 39);

	if (PNum == furn5)  o_Objectswap->Load("./Object/5.가구/", "상자.X", 41);
	if (PNum == furn6)  o_Objectswap->Load("./Object/5.가구/", "선반.X", 42);
	if (PNum == furn7)  o_Objectswap->Load("./Object/5.가구/", "수납장.X", 43);
	if (PNum == furn8)  o_Objectswap->Load("./Object/5.가구/", "수납장2.X", 44);
	if (PNum == furn9)  o_Objectswap->Load("./Object/5.가구/", "스토브.X", 45);
	if (PNum == furn10) o_Objectswap->Load("./Object/5.가구/", "의자.X", 46);
	if (PNum == furn11) o_Objectswap->Load("./Object/5.가구/", "접시.X", 47);
	if (PNum == furn12) o_Objectswap->Load("./Object/5.가구/", "주방도구.X", 48);
	if (PNum == furn13) o_Objectswap->Load("./Object/5.가구/", "책장.X", 49);

	if (PNum == etc1)   o_Objectswap->Load("./Object/6.기타/", "달팽이껍질.X", 50);
	if (PNum == etc2)   o_Objectswap->Load("./Object/6.기타/", "도미노 그룹.X", 51);
	if (PNum == etc3)   o_Objectswap->Load("./Object/6.기타/", "도미노.X", 52);
	if (PNum == etc4)   o_Objectswap->Load("./Object/6.기타/", "도미노C.X", 53);
	if (PNum == etc5)   o_Objectswap->Load("./Object/6.기타/", "미끄럼틀 직선A.X", 54);
	if (PNum == etc6)   o_Objectswap->Load("./Object/6.기타/", "열쇠구멍 글로리.X", 55);
	if (PNum == etc7)   o_Objectswap->Load("./Object/6.기타/", "열쇠구멍문.X", 56);
	if (PNum == etc8)   o_Objectswap->Load("./Object/6.기타/", "열쇠구멍터널.X", 57);
	if (PNum == etc9)   o_Objectswap->Load("./Object/6.기타/", "주사위.X", 58);
	if (PNum == etc10)  o_Objectswap->Load("./Object/6.기타/", "집(기억).X", 59);
}

void cObjtool::ObjNumselect()
{
	if (g_pkeyManager->isOnceKeyDown('E'))
	{
		if (Objnumtab != 5) Objnumtab++;
	}

	if (g_pkeyManager->isOnceKeyDown('Q'))
	{
		if(Objnumtab != 0) Objnumtab--;
	}

	if (Objnumtab == 0)
	{
		if (g_pkeyManager->isOnceKeyDown('1'))
		{
			Objnum = rock1;
			o_Object->Load("./Object/1.바위/", "거대모스바위A.X", 0);
		}
		else if (g_pkeyManager->isOnceKeyDown('2'))
		{
			Objnum = rock2;
			o_Object->Load("./Object/1.바위/", "거대바위벽.X", 1);
		}
		else if (g_pkeyManager->isOnceKeyDown('3'))
		{
			Objnum = rock3;
			o_Object->Load("./Object/1.바위/", "긴발판바위.X", 2);
		}
		else if (g_pkeyManager->isOnceKeyDown('4'))
		{
			Objnum = rock4;
			o_Object->Load("./Object/1.바위/", "납작한바위.X", 3);
		}
		else if (g_pkeyManager->isOnceKeyDown('5'))
		{
			Objnum = rock5;
			o_Object->Load("./Object/1.바위/", "눈물바위.X", 4);
		}
		else if (g_pkeyManager->isOnceKeyDown('6'))
		{
			Objnum = rock6;
			o_Object->Load("./Object/1.바위/", "락필리아A.X", 5);
		}
		else if (g_pkeyManager->isOnceKeyDown('7'))
		{
			Objnum = rock7;
			o_Object->Load("./Object/1.바위/", "락필리아B.X", 6);
		}
		else if (g_pkeyManager->isOnceKeyDown('8'))
		{
			Objnum = rock8;
			o_Object->Load("./Object/1.바위/", "모스바위A.X", 7);
		}
		else if (g_pkeyManager->isOnceKeyDown('9'))
		{
			Objnum = rock9;
			o_Object->Load("./Object/1.바위/", "모스바위B.X", 8);
		}
		else if (g_pkeyManager->isOnceKeyDown('0'))
		{
			Objnum = rock10;
			o_Object->Load("./Object/1.바위/", "모스바위C.X", 9);
		}
	}

	if (Objnumtab == 1)
	{
		if (g_pkeyManager->isOnceKeyDown('1'))
		{
			Objnum = rock11;
			o_Object->Load("./Object/1.바위/", "모스바위더미.X", 10);
		}
		else if (g_pkeyManager->isOnceKeyDown('2'))
		{
			Objnum = tree1;
			o_Object->Load("./Object/2.나무/", "글로리나무 그룹.X", 11);
		}
		else if (g_pkeyManager->isOnceKeyDown('3'))
		{
			Objnum = tree2;
			o_Object->Load("./Object/2.나무/", "글로리나무.X", 12);
		}
		else if (g_pkeyManager->isOnceKeyDown('4'))
		{
			Objnum = tree3;
			o_Object->Load("./Object/2.나무/", "글로리할로우나무.X", 13);
		}
		else if (g_pkeyManager->isOnceKeyDown('5'))
		{
			Objnum = tree4;
			o_Object->Load("./Object/2.나무/", "두꺼운나무.X", 14);
		}
		else if (g_pkeyManager->isOnceKeyDown('6'))
		{
			Objnum = tree5;
			o_Object->Load("./Object/2.나무/", "얇은나무.X", 15);
		}
		else if (g_pkeyManager->isOnceKeyDown('7'))
		{
			Objnum = mush1;
			o_Object->Load("./Object/3.버섯/", "거미버섯.X", 16);
		}
		else if (g_pkeyManager->isOnceKeyDown('8'))
		{
			Objnum = mush2;
			o_Object->Load("./Object/3.버섯/", "버섯발판.X", 17);
		}
		else if (g_pkeyManager->isOnceKeyDown('9'))
		{
			Objnum = mush3;
			o_Object->Load("./Object/3.버섯/", "에일리언버섯 그룹(벽).X", 18);
		}
		else if (g_pkeyManager->isOnceKeyDown('0'))
		{
			Objnum = mush4;
			o_Object->Load("./Object/3.버섯/", "에일리언버섯 그룹.X", 19);
		}
	}

	if (Objnumtab == 2)
	{
		if (g_pkeyManager->isOnceKeyDown('1'))
		{
			Objnum = mush5;
			o_Object->Load("./Object/3.버섯/", "에일리언버섯.X", 20);
		}
		else if (g_pkeyManager->isOnceKeyDown('2'))
		{
			Objnum = mush6;
			o_Object->Load("./Object/3.버섯/", "체커버섯 그룹(벽).X", 21);
		}
		else if (g_pkeyManager->isOnceKeyDown('3'))
		{
			Objnum = mush7;
			o_Object->Load("./Object/3.버섯/", "체커버섯 그룹.X", 22);
		}
		else if (g_pkeyManager->isOnceKeyDown('4'))
		{
			Objnum = mush8;
			o_Object->Load("./Object/3.버섯/", "체커버섯.X", 23);
		}
		else if (g_pkeyManager->isOnceKeyDown('5'))
		{
			Objnum = mush9;
			o_Object->Load("./Object/3.버섯/", "토드스툴A.X", 24);
		}
		else if (g_pkeyManager->isOnceKeyDown('6'))
		{
			Objnum = mush10;
			o_Object->Load("./Object/3.버섯/", "토드스툴B.X", 25);
		}
		else if (g_pkeyManager->isOnceKeyDown('7'))
		{
			Objnum = flow1;
			o_Object->Load("./Object/4.꽃/", "꽃밭 그룹A.X", 26);
		}
		else if (g_pkeyManager->isOnceKeyDown('8'))
		{
			Objnum = flow2;
			o_Object->Load("./Object/4.꽃/", "꽃밭 그룹B.X", 27);
		}
		else if (g_pkeyManager->isOnceKeyDown('9'))
		{
			Objnum = flow3;
			o_Object->Load("./Object/4.꽃/", "꽃밭A.X", 28);
		}
		else if (g_pkeyManager->isOnceKeyDown('0'))
		{
			Objnum = flow4;
			o_Object->Load("./Object/4.꽃/", "꽃밭B.X", 29);
		}
	}

	if (Objnumtab == 3)
	{
		if (g_pkeyManager->isOnceKeyDown('1'))
		{
			Objnum = flow5;
			o_Object->Load("./Object/4.꽃/", "꽃밭C.X", 30);
		}
		else if (g_pkeyManager->isOnceKeyDown('2'))
		{
			Objnum = flow6;
			o_Object->Load("./Object/4.꽃/", "꽃밭D.X", 31);
		}
		else if (g_pkeyManager->isOnceKeyDown('3'))
		{
			Objnum = flow7;
			o_Object->Load("./Object/4.꽃/", "나팔꽃.X", 32);
		}
		else if (g_pkeyManager->isOnceKeyDown('4'))
		{
			Objnum = flow8;
			o_Object->Load("./Object/4.꽃/", "분홍공꽃.X", 33);
		}
		else if (g_pkeyManager->isOnceKeyDown('5'))
		{
			Objnum = flow9;
			o_Object->Load("./Object/4.꽃/", "브루밍꽃B.X", 34);
		}
		else if (g_pkeyManager->isOnceKeyDown('6'))
		{
			Objnum = flow10;
			o_Object->Load("./Object/4.꽃/", "잎사귀.X", 35);
		}
		else if (g_pkeyManager->isOnceKeyDown('7'))
		{
			Objnum = flow11;
			o_Object->Load("./Object/4.꽃/", "절벽꽃.X", 36);
		}
		else if (g_pkeyManager->isOnceKeyDown('8'))
		{
			Objnum = furn1;
			o_Object->Load("./Object/5.가구/", "그림.X", 37);
		}
		else if (g_pkeyManager->isOnceKeyDown('9'))
		{
			Objnum = furn2;
			o_Object->Load("./Object/5.가구/", "나무테이블.X", 38);
		}
		else if (g_pkeyManager->isOnceKeyDown('0'))
		{
			Objnum = furn3;
			o_Object->Load("./Object/5.가구/", "두체스나무.X", 39);
		}
	}

	if (Objnumtab == 4)
	{
		if (g_pkeyManager->isOnceKeyDown('1'))
		{
			Objnum = furn5;
			o_Object->Load("./Object/5.가구/", "상자.X", 41);
		}
		else if (g_pkeyManager->isOnceKeyDown('2'))
		{
			Objnum = furn6;
			o_Object->Load("./Object/5.가구/", "선반.X", 42);
		}
		else if (g_pkeyManager->isOnceKeyDown('3'))
		{
			Objnum = furn7;
			o_Object->Load("./Object/5.가구/", "수납장.X", 43);
		}
		else if (g_pkeyManager->isOnceKeyDown('4'))
		{
			Objnum = furn8;
			o_Object->Load("./Object/5.가구/", "수납장2.X", 44);
		}
		else if (g_pkeyManager->isOnceKeyDown('5'))
		{
			Objnum = furn9;
			o_Object->Load("./Object/5.가구/", "스토브.X", 45);
		}
		else if (g_pkeyManager->isOnceKeyDown('6'))
		{
			Objnum = furn10;
			o_Object->Load("./Object/5.가구/", "의자.X", 46);
		}
		else if (g_pkeyManager->isOnceKeyDown('7'))
		{
			Objnum = furn11;
			o_Object->Load("./Object/5.가구/", "접시.X", 47);
		}
		else if (g_pkeyManager->isOnceKeyDown('8'))
		{
			Objnum = furn12;
			o_Object->Load("./Object/5.가구/", "주방도구.X", 48);
		}
		else if (g_pkeyManager->isOnceKeyDown('9'))
		{
			Objnum = furn13;
			o_Object->Load("./Object/5.가구/", "책장.X", 49);
		}
	}

	if (Objnumtab == 5)
	{
		if (g_pkeyManager->isOnceKeyDown('1'))
		{
			Objnum = etc1;
			o_Object->Load("./Object/6.기타/", "달팽이껍질.X", 50);
		}
		else if (g_pkeyManager->isOnceKeyDown('2'))
		{
			Objnum = etc2;
			o_Object->Load("./Object/6.기타/", "도미노 그룹.X", 51);
		}
		else if (g_pkeyManager->isOnceKeyDown('3'))
		{
			Objnum = etc3;
			o_Object->Load("./Object/6.기타/", "도미노.X", 52);
		}
		else if (g_pkeyManager->isOnceKeyDown('4'))
		{
			Objnum = etc4;
			o_Object->Load("./Object/6.기타/", "도미노C.X", 53);
		}
		else if (g_pkeyManager->isOnceKeyDown('5'))
		{
			Objnum = etc5;
			o_Object->Load("./Object/6.기타/", "미끄럼틀 직선A.X", 54);
		}
		else if (g_pkeyManager->isOnceKeyDown('6'))
		{
			Objnum = etc6;
			o_Object->Load("./Object/6.기타/", "열쇠구멍 글로리.X", 55);
		}
		else if (g_pkeyManager->isOnceKeyDown('7'))
		{
			Objnum = etc7;
			o_Object->Load("./Object/6.기타/", "열쇠구멍문.X", 56);
		}
		else if (g_pkeyManager->isOnceKeyDown('8'))
		{
			Objnum = etc8;
			o_Object->Load("./Object/6.기타/", "열쇠구멍터널.X", 57);
		}
		else if (g_pkeyManager->isOnceKeyDown('9'))
		{
			Objnum = etc9;
			o_Object->Load("./Object/6.기타/", "주사위.X", 58);
		}
		else if (g_pkeyManager->isOnceKeyDown('0'))
		{
			Objnum = etc10;
			o_Object->Load("./Object/6.기타/", "집(기억).X", 59);
		}
	}

}