#include "stdafx.h"
#include "cObjtool.h"

cObjtool::cObjtool()
	:Xplus(0)
	, Yplus(0)
	, Zplus(0)
	, scaling(0.01f)
	, RotationX(0)
	, RotationY(0)
	, RotationZ(0)
{
}


cObjtool::~cObjtool()
{
}

void cObjtool::Setup()
{
	o_Object = new cObjectXfile;
	o_Object->Load("./Object/맵/", "Map.X", 0);
	o_Object->SetPosition(D3DXVECTOR3(-200.0f, 0, 600.f));
	o_Object->SetScale(D3DXVECTOR3(10, 5, 10));
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

	if (PNum == tooth)  o_Objectswap->Load("./Object/이빨/", "이빨.X", 40);
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