#include "stdafx.h"
#include "EffectClass.h"
#include "cObjectXfile.h"

EffectClass::EffectClass()
	:Rollmesh(NULL)
	, plus(0)
	, plus2(0)
	, plus3(0)
	, plus4(0)
{
}

EffectClass::~EffectClass()
{
}

void EffectClass::SetUpEffectTexture()
{
	Rollmesh = new cObjectXfile;
	Rollmesh->Load("./ÀÌÆåÆ®/", "tonado.X");
	Rollmesh->SetPosition(D3DXVECTOR3(0, 0, 0));

	Rollmesh2 = new cObjectXfile;
	Rollmesh2->Load("./ÀÌÆåÆ®/", "Combet.X");
	Rollmesh2->SetPosition(D3DXVECTOR3(0, 0, 0));

	Rollmesh3 = new cObjectXfile;
	Rollmesh3->Load("./ÀÌÆåÆ®/", "Combet.X");
	Rollmesh3->SetPosition(D3DXVECTOR3(0, 0, 0));

	Rollmesh4 = new cObjectXfile;
	Rollmesh4->Load("./ÀÌÆåÆ®/", "butter.X");
	Rollmesh4->SetPosition(D3DXVECTOR3(0, 0, 0));


	Rollmesh5 = new cObjectXfile;
	Rollmesh5->Load("./ÀÌÆåÆ®/", "oneline.X");
	Rollmesh5->SetPosition(D3DXVECTOR3(0, 0, 0));


	Rollmesh6 = new cObjectXfile;
	Rollmesh6->Load("./ÀÌÆåÆ®/", "oneline.X");
	Rollmesh6->SetPosition(D3DXVECTOR3(0, 0, 0));

	Rollmesh7 = new cObjectXfile;
	Rollmesh7->Load("./ÀÌÆåÆ®/", "oneline.X");
	Rollmesh7->SetPosition(D3DXVECTOR3(0, 0, 0));


	Rollmesh8 = new cObjectXfile;
	Rollmesh8->Load("./ÀÌÆåÆ®/", "jinJa2.X");
	Rollmesh8->SetPosition(D3DXVECTOR3(0, 0, 0));

	Rollmesh9 = new cObjectXfile;
	Rollmesh9->Load("./ÀÌÆåÆ®/", "jinji1.X");
	Rollmesh9->SetPosition(D3DXVECTOR3(0, 0, 0));


	Rollmesh10 = new cObjectXfile;
	Rollmesh10->Load("./ÀÌÆåÆ®/", "combo3.X");
	Rollmesh10->SetPosition(D3DXVECTOR3(0, 0, 0));
}

void EffectClass::Render(D3DXMATRIXA16 * trans)
{
	plus3 += 4.0f;

	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);

	//g_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	// D3DBLENDOP_ADD (±âº»°ª)	: src + dest
	// D3DBLENDOP_SUBTRACT		: src - dest
	// D3DBLENDOP_REVSUBTRACT	: dest - src
	// D3DBLENDOP_MIN			: MIN(src, dest)
	// D3DBLENDOP_MAX			: MAX(src, dest)
	g_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	// D3DBLEND_ZERO : (0, 0, 0, 0)
	// D3DBLEND_ONE : (1, 1, 1, 1)
	// D3DBLEND_SRCCOLOR : (rs, gs, bs, as)
	// D3DBLEND_INVSRCCOLOR : (1-rs, 1-gs, 1-bs, 1-as)
	// D3DBLEND_SRCALPHA : (as, as, as, as) (Source Blend ±âº»°ª)
	// D3DBLEND_INVSRCALPHA : (1-as, 1-as, 1-as, 1-as) (Destination Blend ±âº»°ª)
	// D3DBLEND_DESTALPHA : (ad, ad, ad, ad)
	// D3DBLEND_INVDESTALPHA : (1-ad, 1-ad, 1-ad, 1-ad)
	// D3DBLEND_DESTCOLOR : (rd, gd, bd, ad)
	// D3DBLEND_INVDESTCCOLOR : (1-rd, 1-gd, 1-bd, 1-ad)
	// D3DBLEND_SRCALPHASAT : (f, f, f, 1); f=min(as, 1-ad)
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000000);
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);

	D3DXMATRIXA16 matWorld, matBase, matWorld2, matWorld3, matWorld4, matS, matSS, matR, matR1, matT, matTT, matZ, matZ1;
	D3DXMatrixRotationY(&matR, °¢µµ(plus3));
	D3DXMatrixRotationY(&matR1, -°¢µµ(plus3));
	D3DXMatrixRotationZ(&matZ, °¢µµ(30.0f));
	D3DXMatrixRotationZ(&matZ1, -°¢µµ(30.0f));

	D3DXMatrixScaling(&matS, 1.8, 1.0, 1.8);
	D3DXMatrixScaling(&matSS, 0.05, 0.05, 0.05);

	matBase = matR* *trans;
	D3DXMatrixTranslation(&matTT, 0, -5, 0);
	matWorld = matS * matR * *trans * matTT;
	Rollmesh->UpdateMatrix(&matWorld);

	D3DXMatrixTranslation(&matT, 0, 1, 0);


	matWorld2 = matR * matZ * *trans * matT;
	Rollmesh2->UpdateMatrix(&matWorld2);

	matWorld3 = matR1 * matZ1 * *trans * matT;
	Rollmesh3->UpdateMatrix(&matWorld3);

	matWorld4 = matSS * matR* *trans * matT;
	Rollmesh4->UpdateMatrix(&matWorld4);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

void EffectClass::ToneRender(D3DXMATRIXA16 * trans)
{
	if (plus < 0.05f) {
		plus += 0.01f;
	}


	plus2 += 4.0f;
	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000000);
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);

	D3DXMATRIXA16 matBase, matBase1, matBase2, matR, matR2, matR3, matSS, matWorld6, matS, matTT;

	D3DXMatrixRotationY(&matR, °¢µµ(plus2));
	D3DXMatrixRotationZ(&matR2, -°¢µµ(plus2));
	D3DXMatrixRotationZ(&matR3, °¢µµ(plus2));
	D3DXMatrixTranslation(&matTT, 0, 0, 0);
	D3DXMatrixScaling(&matSS, 0.008 + plus, 0.008, 0.008 + plus);

	matBase = matSS *matR * matR2 *  *trans * matTT;

	matBase1 = matSS *  matR * *trans * matTT;

	matBase2 = matSS * matR * matR3 * *trans;

	Rollmesh5->UpdateMatrix(&matBase);

	Rollmesh6->UpdateMatrix(&matBase1);

	Rollmesh7->UpdateMatrix(&matBase2);

	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

void EffectClass::HammerCombo1(D3DXMATRIXA16 * trans)
{
	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000000);
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);

	D3DXMATRIXA16 matBase, matBase2, matSS, matS, matR;
	D3DXMatrixScaling(&matS, 0.5, 1, 0.5);
	D3DXMatrixScaling(&matSS, 0.6, 1, 0.6);
	matBase = matSS  * *trans;

	Rollmesh9->UpdateMatrix(&matBase);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

void EffectClass::EffectKnife(D3DXVECTOR3 * Position)
{

	//D3DXVec3CatmullRom(&temp, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(5, 5, 5), &D3DXVECTOR3(15, 0, 15), &D3DXVECTOR3(20, 5, 20), a);
	//
	//cUt::VecLerp(D3DXVECTOR3(0, 0, 0), temp, a);
}

void EffectClass::HammerCombo3(D3DXMATRIXA16 * trans)
{

	if (plus4 < 0.3f) {
		plus4 += 0.01f;
	}
	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000000);
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);

	D3DXMATRIXA16 matBase, matBase2, matSS, matS, matR;
	D3DXMatrixScaling(&matS, 0.7, 1, 0.7);
	D3DXMatrixScaling(&matSS, 0.6, 1, 0.6);
	matBase = matS  * *trans;

	matBase2 = matSS  * *trans;

	Rollmesh8->UpdateMatrix(&matBase);

	Rollmesh10->UpdateMatrix(&matBase2);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
}
