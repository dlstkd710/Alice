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
	o_Object->Load("./Object/1.����/", "�Ŵ�𽺹���A.X");
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

	ObjNumselect();

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
	if (Objnum == rock1)  o_Objectswap->Load("./Object/1.����/", "�Ŵ�𽺹���A.X");
	if (Objnum == rock2)  o_Objectswap->Load("./Object/1.����/", "�Ŵ������.X");
	if (Objnum == rock3)  o_Objectswap->Load("./Object/1.����/", "����ǹ���.X");
	if (Objnum == rock4)  o_Objectswap->Load("./Object/1.����/", "�����ѹ���.X");
	if (Objnum == rock5)  o_Objectswap->Load("./Object/1.����/", "��������.X");
	if (Objnum == rock6)  o_Objectswap->Load("./Object/1.����/", "���ʸ���A.X");
	if (Objnum == rock7)  o_Objectswap->Load("./Object/1.����/", "���ʸ���B.X");
	if (Objnum == rock8)  o_Objectswap->Load("./Object/1.����/", "�𽺹���A.X");
	if (Objnum == rock9)  o_Objectswap->Load("./Object/1.����/", "�𽺹���B.X");
	if (Objnum == rock10) o_Objectswap->Load("./Object/1.����/", "�𽺹���C.X");

	if (Objnum == rock11) o_Objectswap->Load("./Object/1.����/", "�𽺹�������.X");
	if (Objnum == tree1)  o_Objectswap->Load("./Object/2.����/", "�۷θ����� �׷�.X");
	if (Objnum == tree2)  o_Objectswap->Load("./Object/2.����/", "�۷θ�����.X");
	if (Objnum == tree3)  o_Objectswap->Load("./Object/2.����/", "�۷θ��ҷο쳪��.X");
	if (Objnum == tree4)  o_Objectswap->Load("./Object/2.����/", "�β����.X");
	if (Objnum == tree5)  o_Objectswap->Load("./Object/2.����/", "��������.X");
	if (Objnum == mush1)  o_Objectswap->Load("./Object/3.����/", "�Ź̹���.X");
	if (Objnum == mush2)  o_Objectswap->Load("./Object/3.����/", "��������.X");
	if (Objnum == mush3)  o_Objectswap->Load("./Object/3.����/", "���ϸ������ �׷�(��).X");
	if (Objnum == mush4)  o_Objectswap->Load("./Object/3.����/", "���ϸ������ �׷�.X");

	if (Objnum == mush5)  o_Objectswap->Load("./Object/3.����/", "���ϸ������.X");
	if (Objnum == mush6)  o_Objectswap->Load("./Object/3.����/", "üĿ���� �׷�(��).X");
	if (Objnum == mush7)  o_Objectswap->Load("./Object/3.����/", "üĿ���� �׷�.X");
	if (Objnum == mush8)  o_Objectswap->Load("./Object/3.����/", "üĿ����.X");
	if (Objnum == mush9)  o_Objectswap->Load("./Object/3.����/", "��彺��A.X");
	if (Objnum == mush10) o_Objectswap->Load("./Object/3.����/", "��彺��B.X");
	if (Objnum == flow1)  o_Objectswap->Load("./Object/4.��/", "�ɹ� �׷�A.X");
	if (Objnum == flow2)  o_Objectswap->Load("./Object/4.��/", "�ɹ� �׷�B.X");
	if (Objnum == flow3)  o_Objectswap->Load("./Object/4.��/", "�ɹ�A.X");
	if (Objnum == flow4)  o_Objectswap->Load("./Object/4.��/", "�ɹ�B.X");

	if (Objnum == flow5)  o_Objectswap->Load("./Object/4.��/", "�ɹ�C.X");
	if (Objnum == flow6)  o_Objectswap->Load("./Object/4.��/", "�ɹ�D.X");
	if (Objnum == flow7)  o_Objectswap->Load("./Object/4.��/", "���Ȳ�.X");
	if (Objnum == flow8)  o_Objectswap->Load("./Object/4.��/", "��ȫ����.X");
	if (Objnum == flow9)  o_Objectswap->Load("./Object/4.��/", "���ֲ�B.X");
	if (Objnum == flow10) o_Objectswap->Load("./Object/4.��/", "�ٻ��.X");
	if (Objnum == flow11) o_Objectswap->Load("./Object/4.��/", "������.X");
	if (Objnum == furn1)  o_Objectswap->Load("./Object/5.����/", "�׸�.X");
	if (Objnum == furn2)  o_Objectswap->Load("./Object/5.����/", "�������̺�.X");
	if (Objnum == furn3)  o_Objectswap->Load("./Object/5.����/", "��ü������.X");

	if (Objnum == furn5)  o_Objectswap->Load("./Object/5.����/", "����.X");
	if (Objnum == furn6)  o_Objectswap->Load("./Object/5.����/", "����.X");
	if (Objnum == furn7)  o_Objectswap->Load("./Object/5.����/", "������.X");
	if (Objnum == furn8)  o_Objectswap->Load("./Object/5.����/", "������2.X");
	if (Objnum == furn9)  o_Objectswap->Load("./Object/5.����/", "�����.X");
	if (Objnum == furn10) o_Objectswap->Load("./Object/5.����/", "����.X");
	if (Objnum == furn11) o_Objectswap->Load("./Object/5.����/", "����.X");
	if (Objnum == furn12) o_Objectswap->Load("./Object/5.����/", "�ֹ浵��.X");
	if (Objnum == furn13) o_Objectswap->Load("./Object/5.����/", "å��.X");

	if (Objnum == etc1)  o_Objectswap->Load("./Object/6.��Ÿ/", "�����̲���.X");
	if (Objnum == etc2)  o_Objectswap->Load("./Object/6.��Ÿ/", "���̳� �׷�.X");
	if (Objnum == etc3)  o_Objectswap->Load("./Object/6.��Ÿ/", "���̳�.X");
	if (Objnum == etc4)  o_Objectswap->Load("./Object/6.��Ÿ/", "���̳�C.X");
	if (Objnum == etc5)  o_Objectswap->Load("./Object/6.��Ÿ/", "�̲���Ʋ ����A.X");
	if (Objnum == etc6)  o_Objectswap->Load("./Object/6.��Ÿ/", "���豸�� �۷θ�.X");
	if (Objnum == etc7)  o_Objectswap->Load("./Object/6.��Ÿ/", "���豸�۹�.X");
	if (Objnum == etc8)  o_Objectswap->Load("./Object/6.��Ÿ/", "���豸���ͳ�.X");
	if (Objnum == etc9)  o_Objectswap->Load("./Object/6.��Ÿ/", "�ֻ���.X");
	if (Objnum == etc10) o_Objectswap->Load("./Object/6.��Ÿ/", "��(���).X");
}

void cObjtool::OutputSave()
{
	FILE *wfp = fopen("������Ʈ�Ľ�.txt", "a");
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
	fopen_s(&fp, "������Ʈ�Ľ�.txt", "r");
	while (!feof(fp))
	{
		float Pposx, Pposy, Pposz;
		float Pscalex;
		float PangleX, PangleY, PangleZ;

		char szBuf[1024] = { '\0', };
		fgets(szBuf, 1024, fp);
		if (szBuf[0] == NULL) continue;

		sscanf(szBuf, "%d %f %f %f %f %f %f %f", &PNum, &Pposx, &Pposy, &Pposz, &Pscalex, &PangleX, &PangleY, &PangleZ);
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
	if (PNum == rock1)  o_Objectswap->Load("./Object/1.����/", "�Ŵ�𽺹���A.X");
	if (PNum == rock2)  o_Objectswap->Load("./Object/1.����/", "�Ŵ������.X");
	if (PNum == rock3)  o_Objectswap->Load("./Object/1.����/", "����ǹ���.X");
	if (PNum == rock4)  o_Objectswap->Load("./Object/1.����/", "�����ѹ���.X");
	if (PNum == rock5)  o_Objectswap->Load("./Object/1.����/", "��������.X");
	if (PNum == rock6)  o_Objectswap->Load("./Object/1.����/", "���ʸ���A.X");
	if (PNum == rock7)  o_Objectswap->Load("./Object/1.����/", "���ʸ���B.X");
	if (PNum == rock8)  o_Objectswap->Load("./Object/1.����/", "�𽺹���A.X");
	if (PNum == rock9)  o_Objectswap->Load("./Object/1.����/", "�𽺹���B.X");
	if (PNum == rock10) o_Objectswap->Load("./Object/1.����/", "�𽺹���C.X");

	if (PNum == rock11) o_Objectswap->Load("./Object/1.����/", "�𽺹�������.X");
	if (PNum == tree1)  o_Objectswap->Load("./Object/2.����/", "�۷θ����� �׷�.X");
	if (PNum == tree2)  o_Objectswap->Load("./Object/2.����/", "�۷θ�����.X");
	if (PNum == tree3)  o_Objectswap->Load("./Object/2.����/", "�۷θ��ҷο쳪��.X");
	if (PNum == tree4)  o_Objectswap->Load("./Object/2.����/", "�β����.X");
	if (PNum == tree5)  o_Objectswap->Load("./Object/2.����/", "��������.X");
	if (PNum == mush1)  o_Objectswap->Load("./Object/3.����/", "�Ź̹���.X");
	if (PNum == mush2)  o_Objectswap->Load("./Object/3.����/", "��������.X");
	if (PNum == mush3)  o_Objectswap->Load("./Object/3.����/", "���ϸ������ �׷�(��).X");
	if (PNum == mush4) o_Objectswap->Load("./Object/3.����/", "���ϸ������ �׷�.X");

	if (PNum == mush5)  o_Objectswap->Load("./Object/3.����/", "���ϸ������.X");
	if (PNum == mush6)  o_Objectswap->Load("./Object/3.����/", "üĿ���� �׷�(��).X");
	if (PNum == mush7)  o_Objectswap->Load("./Object/3.����/", "üĿ���� �׷�.X");
	if (PNum == mush8)  o_Objectswap->Load("./Object/3.����/", "üĿ����.X");
	if (PNum == mush9)  o_Objectswap->Load("./Object/3.����/", "��彺��A.X");
	if (PNum == mush10) o_Objectswap->Load("./Object/3.����/", "��彺��B.X");
	if (PNum == flow1)  o_Objectswap->Load("./Object/4.��/", "�ɹ� �׷�A.X");
	if (PNum == flow2)  o_Objectswap->Load("./Object/4.��/", "�ɹ� �׷�B.X");
	if (PNum == flow3)  o_Objectswap->Load("./Object/4.��/", "�ɹ�A.X");
	if (PNum == flow4)  o_Objectswap->Load("./Object/4.��/", "�ɹ�B.X");

	if (PNum == flow5)  o_Objectswap->Load("./Object/4.��/", "�ɹ�C.X");
	if (PNum == flow6)  o_Objectswap->Load("./Object/4.��/", "�ɹ�D.X");
	if (PNum == flow7)  o_Objectswap->Load("./Object/4.��/", "���Ȳ�.X");
	if (PNum == flow8)  o_Objectswap->Load("./Object/4.��/", "��ȫ����.X");
	if (PNum == flow9)  o_Objectswap->Load("./Object/4.��/", "���ֲ�B.X");
	if (PNum == flow10) o_Objectswap->Load("./Object/4.��/", "�ٻ��.X");
	if (PNum == flow11) o_Objectswap->Load("./Object/4.��/", "������.X");
	if (PNum == furn1)  o_Objectswap->Load("./Object/5.����/", "�׸�.X");
	if (PNum == furn2)  o_Objectswap->Load("./Object/5.����/", "�������̺�.X");
	if (PNum == furn3)  o_Objectswap->Load("./Object/5.����/", "��ü������.X");

	if (PNum == furn5)  o_Objectswap->Load("./Object/5.����/", "����.X");
	if (PNum == furn6)  o_Objectswap->Load("./Object/5.����/", "����.X");
	if (PNum == furn7)  o_Objectswap->Load("./Object/5.����/", "������.X");
	if (PNum == furn8)  o_Objectswap->Load("./Object/5.����/", "������2.X");
	if (PNum == furn9)  o_Objectswap->Load("./Object/5.����/", "�����.X");
	if (PNum == furn10) o_Objectswap->Load("./Object/5.����/", "����.X");
	if (PNum == furn11) o_Objectswap->Load("./Object/5.����/", "����.X");
	if (PNum == furn12) o_Objectswap->Load("./Object/5.����/", "�ֹ浵��.X");
	if (PNum == furn13) o_Objectswap->Load("./Object/5.����/", "å��.X");

	if (PNum == etc1)  o_Objectswap->Load("./Object/6.��Ÿ/", "�����̲���.X");
	if (PNum == etc2)  o_Objectswap->Load("./Object/6.��Ÿ/", "���̳� �׷�.X");
	if (PNum == etc3)  o_Objectswap->Load("./Object/6.��Ÿ/", "���̳�.X");
	if (PNum == etc4)  o_Objectswap->Load("./Object/6.��Ÿ/", "���̳�C.X");
	if (PNum == etc5)  o_Objectswap->Load("./Object/6.��Ÿ/", "�̲���Ʋ ����A.X");
	if (PNum == etc6)  o_Objectswap->Load("./Object/6.��Ÿ/", "���豸�� �۷θ�.X");
	if (PNum == etc7)  o_Objectswap->Load("./Object/6.��Ÿ/", "���豸�۹�.X");
	if (PNum == etc8)  o_Objectswap->Load("./Object/6.��Ÿ/", "���豸���ͳ�.X");
	if (PNum == etc9)  o_Objectswap->Load("./Object/6.��Ÿ/", "�ֻ���.X");
	if (PNum == etc10) o_Objectswap->Load("./Object/6.��Ÿ/", "��(���).X");
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
			o_Object->Load("./Object/1.����/", "�Ŵ�𽺹���A.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('2'))
		{
			Objnum = rock2;
			o_Object->Load("./Object/1.����/", "�Ŵ������.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('3'))
		{
			Objnum = rock3;
			o_Object->Load("./Object/1.����/", "����ǹ���.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('4'))
		{
			Objnum = rock4;
			o_Object->Load("./Object/1.����/", "�����ѹ���.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('5'))
		{
			Objnum = rock5;
			o_Object->Load("./Object/1.����/", "��������.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('6'))
		{
			Objnum = rock6;
			o_Object->Load("./Object/1.����/", "���ʸ���A.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('7'))
		{
			Objnum = rock7;
			o_Object->Load("./Object/1.����/", "���ʸ���B.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('8'))
		{
			Objnum = rock8;
			o_Object->Load("./Object/1.����/", "�𽺹���A.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('9'))
		{
			Objnum = rock9;
			o_Object->Load("./Object/1.����/", "�𽺹���B.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('0'))
		{
			Objnum = rock10;
			o_Object->Load("./Object/1.����/", "�𽺹���C.X");
		}
	}

	if (Objnumtab == 1)
	{
		if (g_pkeyManager->isOnceKeyDown('1'))
		{
			Objnum = rock11;
			o_Object->Load("./Object/1.����/", "�𽺹�������.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('2'))
		{
			Objnum = tree1;
			o_Object->Load("./Object/2.����/", "�۷θ����� �׷�.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('3'))
		{
			Objnum = tree2;
			o_Object->Load("./Object/2.����/", "�۷θ�����.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('4'))
		{
			Objnum = tree3;
			o_Object->Load("./Object/2.����/", "�۷θ��ҷο쳪��.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('5'))
		{
			Objnum = tree4;
			o_Object->Load("./Object/2.����/", "�β����.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('6'))
		{
			Objnum = tree5;
			o_Object->Load("./Object/2.����/", "��������.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('7'))
		{
			Objnum = mush1;
			o_Object->Load("./Object/3.����/", "�Ź̹���.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('8'))
		{
			Objnum = mush2;
			o_Object->Load("./Object/3.����/", "��������.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('9'))
		{
			Objnum = mush3;
			o_Object->Load("./Object/3.����/", "���ϸ������ �׷�(��).X");
		}
		else if (g_pkeyManager->isOnceKeyDown('0'))
		{
			Objnum = mush4;
			o_Object->Load("./Object/3.����/", "���ϸ������ �׷�.X");
		}
	}

	if (Objnumtab == 2)
	{
		if (g_pkeyManager->isOnceKeyDown('1'))
		{
			Objnum = mush5;
			o_Object->Load("./Object/3.����/", "���ϸ������.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('2'))
		{
			Objnum = mush6;
			o_Object->Load("./Object/3.����/", "üĿ���� �׷�(��).X");
		}
		else if (g_pkeyManager->isOnceKeyDown('3'))
		{
			Objnum = mush7;
			o_Object->Load("./Object/3.����/", "üĿ���� �׷�.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('4'))
		{
			Objnum = mush8;
			o_Object->Load("./Object/3.����/", "üĿ����.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('5'))
		{
			Objnum = mush9;
			o_Object->Load("./Object/3.����/", "��彺��A.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('6'))
		{
			Objnum = mush10;
			o_Object->Load("./Object/3.����/", "��彺��B.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('7'))
		{
			Objnum = flow1;
			o_Object->Load("./Object/4.��/", "�ɹ� �׷�A.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('8'))
		{
			Objnum = flow2;
			o_Object->Load("./Object/4.��/", "�ɹ� �׷�B.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('9'))
		{
			Objnum = flow3;
			o_Object->Load("./Object/4.��/", "�ɹ�A.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('0'))
		{
			Objnum = flow4;
			o_Object->Load("./Object/4.��/", "�ɹ�B.X");
		}
	}

	if (Objnumtab == 3)
	{
		if (g_pkeyManager->isOnceKeyDown('1'))
		{
			Objnum = flow5;
			o_Object->Load("./Object/4.��/", "�ɹ�C.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('2'))
		{
			Objnum = flow6;
			o_Object->Load("./Object/4.��/", "�ɹ�D.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('3'))
		{
			Objnum = flow7;
			o_Object->Load("./Object/4.��/", "���Ȳ�.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('4'))
		{
			Objnum = flow8;
			o_Object->Load("./Object/4.��/", "��ȫ����.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('5'))
		{
			Objnum = flow9;
			o_Object->Load("./Object/4.��/", "���ֲ�B.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('6'))
		{
			Objnum = flow10;
			o_Object->Load("./Object/4.��/", "�ٻ��.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('7'))
		{
			Objnum = flow11;
			o_Object->Load("./Object/4.��/", "������.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('8'))
		{
			Objnum = furn1;
			o_Object->Load("./Object/5.����/", "�׸�.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('9'))
		{
			Objnum = furn2;
			o_Object->Load("./Object/5.����/", "�������̺�.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('0'))
		{
			Objnum = furn3;
			o_Object->Load("./Object/5.����/", "��ü������.X");
		}
	}

	if (Objnumtab == 4)
	{
		if (g_pkeyManager->isOnceKeyDown('1'))
		{
			Objnum = furn5;
			o_Object->Load("./Object/5.����/", "����.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('2'))
		{
			Objnum = furn6;
			o_Object->Load("./Object/5.����/", "����.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('3'))
		{
			Objnum = furn7;
			o_Object->Load("./Object/5.����/", "������.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('4'))
		{
			Objnum = furn8;
			o_Object->Load("./Object/5.����/", "������2.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('5'))
		{
			Objnum = furn9;
			o_Object->Load("./Object/5.����/", "�����.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('6'))
		{
			Objnum = furn10;
			o_Object->Load("./Object/5.����/", "����.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('7'))
		{
			Objnum = furn11;
			o_Object->Load("./Object/5.����/", "����.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('8'))
		{
			Objnum = furn12;
			o_Object->Load("./Object/5.����/", "�ֹ浵��.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('9'))
		{
			Objnum = furn13;
			o_Object->Load("./Object/5.����/", "å��.X");
		}
	}

	if (Objnumtab == 5)
	{
		if (g_pkeyManager->isOnceKeyDown('1'))
		{
			Objnum = etc1;
			o_Object->Load("./Object/6.��Ÿ/", "�����̲���.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('2'))
		{
			Objnum = etc2;
			o_Object->Load("./Object/6.��Ÿ/", "���̳� �׷�.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('3'))
		{
			Objnum = etc3;
			o_Object->Load("./Object/6.��Ÿ/", "���̳�.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('4'))
		{
			Objnum = etc4;
			o_Object->Load("./Object/6.��Ÿ/", "���̳�C.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('5'))
		{
			Objnum = etc5;
			o_Object->Load("./Object/6.��Ÿ/", "�̲���Ʋ ����A.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('6'))
		{
			Objnum = etc6;
			o_Object->Load("./Object/6.��Ÿ/", "���豸�� �۷θ�.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('7'))
		{
			Objnum = etc7;
			o_Object->Load("./Object/6.��Ÿ/", "���豸�۹�.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('8'))
		{
			Objnum = etc8;
			o_Object->Load("./Object/6.��Ÿ/", "���豸���ͳ�.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('9'))
		{
			Objnum = etc9;
			o_Object->Load("./Object/6.��Ÿ/", "�ֻ���.X");
		}
		else if (g_pkeyManager->isOnceKeyDown('0'))
		{
			Objnum = etc10;
			o_Object->Load("./Object/6.��Ÿ/", "��(���).X");
		}
	}

}