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
	o_Object->Load("./Object/��/", "Map.X", 0);
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
	fopen_s(&fp, "������Ʈ�Ľ�.txt", "r");
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
	if (PNum == rock1)  o_Objectswap->Load("./Object/1.����/", "�Ŵ�𽺹���A.X", 0);
	if (PNum == rock2)  o_Objectswap->Load("./Object/1.����/", "�Ŵ������.X", 1);
	if (PNum == rock3)  o_Objectswap->Load("./Object/1.����/", "����ǹ���.X", 2);
	if (PNum == rock4)  o_Objectswap->Load("./Object/1.����/", "�����ѹ���.X", 3);
	if (PNum == rock5)  o_Objectswap->Load("./Object/1.����/", "��������.X", 4);
	if (PNum == rock6)  o_Objectswap->Load("./Object/1.����/", "���ʸ���A.X", 5);
	if (PNum == rock7)  o_Objectswap->Load("./Object/1.����/", "���ʸ���B.X", 6);
	if (PNum == rock8)  o_Objectswap->Load("./Object/1.����/", "�𽺹���A.X", 7);
	if (PNum == rock9)  o_Objectswap->Load("./Object/1.����/", "�𽺹���B.X", 8);
	if (PNum == rock10) o_Objectswap->Load("./Object/1.����/", "�𽺹���C.X", 9);

	if (PNum == rock11) o_Objectswap->Load("./Object/1.����/", "�𽺹�������.X", 10);
	if (PNum == tree1)  o_Objectswap->Load("./Object/2.����/", "�۷θ����� �׷�.X", 11);
	if (PNum == tree2)  o_Objectswap->Load("./Object/2.����/", "�۷θ�����.X", 12);
	if (PNum == tree3)  o_Objectswap->Load("./Object/2.����/", "�۷θ��ҷο쳪��.X", 13);
	if (PNum == tree4)  o_Objectswap->Load("./Object/2.����/", "�β����.X", 14);
	if (PNum == tree5)  o_Objectswap->Load("./Object/2.����/", "��������.X", 15);
	if (PNum == mush1)  o_Objectswap->Load("./Object/3.����/", "�Ź̹���.X", 16);
	if (PNum == mush2)  o_Objectswap->Load("./Object/3.����/", "��������.X", 17);
	if (PNum == mush3)  o_Objectswap->Load("./Object/3.����/", "���ϸ������ �׷�(��).X", 18);
	if (PNum == mush4)  o_Objectswap->Load("./Object/3.����/", "���ϸ������ �׷�.X", 19);

	if (PNum == mush5)  o_Objectswap->Load("./Object/3.����/", "���ϸ������.X", 20);
	if (PNum == mush6)  o_Objectswap->Load("./Object/3.����/", "üĿ���� �׷�(��).X", 21);
	if (PNum == mush7)  o_Objectswap->Load("./Object/3.����/", "üĿ���� �׷�.X", 22);
	if (PNum == mush8)  o_Objectswap->Load("./Object/3.����/", "üĿ����.X", 23);
	if (PNum == mush9)  o_Objectswap->Load("./Object/3.����/", "��彺��A.X", 24);
	if (PNum == mush10) o_Objectswap->Load("./Object/3.����/", "��彺��B.X", 25);
	if (PNum == flow1)  o_Objectswap->Load("./Object/4.��/", "�ɹ� �׷�A.X", 26);
	if (PNum == flow2)  o_Objectswap->Load("./Object/4.��/", "�ɹ� �׷�B.X", 27);
	if (PNum == flow3)  o_Objectswap->Load("./Object/4.��/", "�ɹ�A.X", 28);
	if (PNum == flow4)  o_Objectswap->Load("./Object/4.��/", "�ɹ�B.X", 29);

	if (PNum == flow5)  o_Objectswap->Load("./Object/4.��/", "�ɹ�C.X", 30);
	if (PNum == flow6)  o_Objectswap->Load("./Object/4.��/", "�ɹ�D.X", 31);
	if (PNum == flow7)  o_Objectswap->Load("./Object/4.��/", "���Ȳ�.X", 32);
	if (PNum == flow8)  o_Objectswap->Load("./Object/4.��/", "��ȫ����.X", 33);
	if (PNum == flow9)  o_Objectswap->Load("./Object/4.��/", "���ֲ�B.X", 34);
	if (PNum == flow10) o_Objectswap->Load("./Object/4.��/", "�ٻ��.X", 35);
	if (PNum == flow11) o_Objectswap->Load("./Object/4.��/", "������.X", 36);
	if (PNum == furn1)  o_Objectswap->Load("./Object/5.����/", "�׸�.X", 37);
	if (PNum == furn2)  o_Objectswap->Load("./Object/5.����/", "�������̺�.X", 38);
	if (PNum == furn3)  o_Objectswap->Load("./Object/5.����/", "��ü������.X", 39);

	if (PNum == tooth)  o_Objectswap->Load("./Object/�̻�/", "�̻�.X", 40);
	if (PNum == furn5)  o_Objectswap->Load("./Object/5.����/", "����.X", 41);
	if (PNum == furn6)  o_Objectswap->Load("./Object/5.����/", "����.X", 42);
	if (PNum == furn7)  o_Objectswap->Load("./Object/5.����/", "������.X", 43);
	if (PNum == furn8)  o_Objectswap->Load("./Object/5.����/", "������2.X", 44);
	if (PNum == furn9)  o_Objectswap->Load("./Object/5.����/", "�����.X", 45);
	if (PNum == furn10) o_Objectswap->Load("./Object/5.����/", "����.X", 46);
	if (PNum == furn11) o_Objectswap->Load("./Object/5.����/", "����.X", 47);
	if (PNum == furn12) o_Objectswap->Load("./Object/5.����/", "�ֹ浵��.X", 48);
	if (PNum == furn13) o_Objectswap->Load("./Object/5.����/", "å��.X", 49);

	if (PNum == etc1)   o_Objectswap->Load("./Object/6.��Ÿ/", "�����̲���.X", 50);
	if (PNum == etc2)   o_Objectswap->Load("./Object/6.��Ÿ/", "���̳� �׷�.X", 51);
	if (PNum == etc3)   o_Objectswap->Load("./Object/6.��Ÿ/", "���̳�.X", 52);
	if (PNum == etc4)   o_Objectswap->Load("./Object/6.��Ÿ/", "���̳�C.X", 53);
	if (PNum == etc5)   o_Objectswap->Load("./Object/6.��Ÿ/", "�̲���Ʋ ����A.X", 54);
	if (PNum == etc6)   o_Objectswap->Load("./Object/6.��Ÿ/", "���豸�� �۷θ�.X", 55);
	if (PNum == etc7)   o_Objectswap->Load("./Object/6.��Ÿ/", "���豸�۹�.X", 56);
	if (PNum == etc8)   o_Objectswap->Load("./Object/6.��Ÿ/", "���豸���ͳ�.X", 57);
	if (PNum == etc9)   o_Objectswap->Load("./Object/6.��Ÿ/", "�ֻ���.X", 58);
	if (PNum == etc10)  o_Objectswap->Load("./Object/6.��Ÿ/", "��(���).X", 59);
}