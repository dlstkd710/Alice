#include "stdafx.h"
#include "Shadow.h"
Shadow::Shadow()
	: Xpos(0)
	, Ypos(0)
{	 
}
Shadow::~Shadow() {}

void Shadow::SetUp()
{
	gMeshColor = D3DXVECTOR4(1.f, 1.f, 0.f, 1);
	gBackgroundColor = D3DXVECTOR4(1.f, 1.f, 0.f, 1);

	gWorldLightPos = D3DXVECTOR4(1000.0f, 1000.0f , -1000.0f, 1.0f);
	gWorldCameraPos = D3DXVECTOR4(0.0f, 100.0f, -150.0f, 1.0f);
}

void Shadow::Update()
{
	gWorldLightPos = D3DXVECTOR4(1000.0f, 1000.0f + Ypos, -1000.0f + Xpos, 1.0f);

	if (GetAsyncKeyState('A') & 0x8001) {
		Ypos -= 10;
	}
	if (GetAsyncKeyState('D') & 0x8001) {
		Ypos += 10;
	}
	if (GetAsyncKeyState('W') & 0x8001) {
		Xpos -= 10;
	}
	if (GetAsyncKeyState('S') & 0x8001) {
		Xpos += 10;
	}
}

bool Shadow::InitShadowRenderTarget()
{
	//����Ÿ�� ����
	const int shadowMapSize = 4096;
	g_pD3DDevice->CreateTexture(
		shadowMapSize, shadowMapSize,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_R32F, D3DPOOL_DEFAULT,
		&gShadowRenderTarget, NULL);


	//�׸��ڸʰ� ������ ũ���� ���̹��� ����
	g_pD3DDevice->CreateDepthStencilSurface(
		shadowMapSize, shadowMapSize,
		D3DFMT_D24X8,
		D3DMULTISAMPLE_NONE,
		0, TRUE,
		&gShadowDepthStencil, NULL);


	return true;
}
//-----------------------------------------------------------------------------
void Shadow::SetupMatrix(float aspect)
{
	//�������
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);

	//�����
	D3DXMATRIXA16 matView;
	static D3DXVECTOR3 inOutEyePos(0, 0, -100);
	D3DXVECTOR3 retLookatPos(0, 0, 0);
	D3DXVECTOR3 upVec(0, 1, 0);

	D3DXMatrixLookAtLH(&matView, &inOutEyePos, &retLookatPos, &upVec);

	//�������
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, aspect, 1.0f, 1000.0f);

	if (gShader)
	{
		gShader->SetMatrix("gWorldMat", &matWorld);
		gShader->SetMatrix("gViewMat", &matView);
		gShader->SetMatrix("gProjMat", &matProj);
	}
	else
	{
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
		g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);
		g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);
	}
}


// ���̴� �ε�
LPD3DXEFFECT Shadow::LoadShader(const char * filename)
{
	LPD3DXEFFECT ret = NULL;

	LPD3DXBUFFER pError = NULL;
	DWORD dwShaderFlags = 0;

#if _DEBUG
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

	D3DXCreateEffectFromFile(g_pD3DDevice, filename,
		NULL, NULL, dwShaderFlags, NULL, &ret, &pError);

	// ���̴� ���� ���
	if (!ret && pError)
	{
		int size = pError->GetBufferSize();
		void *ack = pError->GetBufferPointer();

		if (ack)
		{
			char* str = new char[size];
			sprintf_s(str, size, (const char*)ack);
			OutputDebugString(str);
			delete[] str;
		}
	}

	return ret;
}

// �� �ε�
LPD3DXMESH	LoadModel(const char * filename)
{
	LPD3DXMESH ret = NULL;
	D3DXLoadMeshFromX(filename, D3DXMESH_SYSTEMMEM, g_pD3DDevice, NULL, NULL, NULL, NULL, &ret);

	return ret;
}

bool Shadow::LoadAsset()
{
	// ���̴� �ε�
	// �� �ε�
	gShader = LoadShader("ApplyShadow.fx");
	if (gShader == NULL)
		return false;

	gMesh = LoadModel("AliceW.x");
	if (gMesh == NULL)
		return false;

	//CreateShadow ���̴� �ε�
	gCreateShadowShader = LoadShader("CreateShadow.fx");
	if (gCreateShadowShader == NULL)
		return false;

	//���� ��� �޽� �ε�
	gBackgroundMesh = LoadModel("Disc.x");
	if (gBackgroundMesh == NULL)
		return false;

	return true;
}


void Shadow::RenderScene(D3DXMATRIXA16* matworld, D3DXVECTOR3 cubePos)
{
	// ����-�� ���
	D3DXMATRIXA16 matLightView;
	{
		D3DXVECTOR3 vEyePt(gWorldLightPos.x, gWorldLightPos.y, gWorldLightPos.z);
		D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&matLightView, &vEyePt, &vLookatPt, &vUpVec);
	}

	// ����-���� ���
	D3DXMATRIXA16 matLightProjection;
	D3DXMatrixPerspectiveFovLH(&matLightProjection, D3DX_PI / 4.0f, 1, 1, 3000);

	// ��-�������
	D3DXMATRIXA16 matViewProjection;
	{
		//�����
		D3DXMATRIXA16 matView;
		D3DXVECTOR3 vEyePt(cubePos.x, cubePos.y, cubePos.z);
		D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);

		//�������
		D3DXMATRIXA16 matProjection;
		RECT rc;
		GetClientRect(g_hWnd, &rc);
		D3DXMatrixPerspectiveFovLH(&matProjection, D3DX_PI / 4.0f, rc.right / (float)rc.bottom, 1, 1000);
		D3DXMatrixMultiply(&matViewProjection, &matView, &matProjection);
	}

	D3DXMATRIXA16 matModel;
	{
		D3DXMATRIXA16 matScale;
		D3DXMatrixScaling(&matScale, 10, 10, 10);

		D3DXMATRIXA16 matTrans;
		D3DXMatrixTranslation(&matTrans, 0, 0, 0);

		D3DXMATRIXA16 matRote;

		D3DXMatrixMultiply(&matModel, &matScale, &matTrans);
	}


	D3DXMATRIXA16 matDiscWorld;
	{
		D3DXMATRIXA16 matScale;
		D3DXMatrixScaling(&matScale, 6, 6, 6);

		D3DXMATRIXA16 matTrans;
		D3DXMatrixTranslation(&matTrans, 0, -10, -20);

		D3DXMATRIXA16 matRote;

		D3DXMatrixMultiply(&matDiscWorld, &matScale, &matTrans);
	}


	// ���� �ϵ���� ����ۿ� ���̹��۸� �����Ѵ�.
	LPDIRECT3DSURFACE9 pHWBackBuffer = NULL;
	LPDIRECT3DSURFACE9 pHWDepthStencilBuffer = NULL;

	g_pD3DDevice->GetRenderTarget(0, &pHWBackBuffer);
	g_pD3DDevice->GetDepthStencilSurface(&pHWDepthStencilBuffer);

	//1. �׸��� �����

	// �׸��� ���� ����Ÿ��� ���̹��۸� ����Ѵ�.
	LPDIRECT3DSURFACE9 pShadowSurface = NULL;
	if (SUCCEEDED(gShadowRenderTarget->GetSurfaceLevel(0, &pShadowSurface)))
	{
		g_pD3DDevice->SetRenderTarget(0, pShadowSurface);
		pShadowSurface->Release();
		pShadowSurface = NULL;
	}
	g_pD3DDevice->SetDepthStencilSurface(gShadowDepthStencil);

	//���� �����ӿ� �׷ȴ� �׸��� ������ ����
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), 0xFFFFFFFF, 1.0f, 0);

	// �׸��� ����� ���̴� ������������ ����
	gCreateShadowShader->SetMatrix("gWorldMat", &matModel);
	gCreateShadowShader->SetMatrix("gLightViewMat", &matLightView);
	gCreateShadowShader->SetMatrix("gLightProjMat", &matLightProjection);

	// �׸��� ����� ���̴��� ����
	{
		UINT numPasses = 0;
		gCreateShadowShader->Begin(&numPasses, NULL);
		{
			for (UINT i = 0; i < numPasses; ++i)
			{
				gCreateShadowShader->BeginPass(i);

				g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
 
				for (int j = 0; j < 50; j++)
					gMesh->DrawSubset(j);
				
				gCreateShadowShader->EndPass();
			}
		}
		gCreateShadowShader->End();
	}


	// 2. �׸��� �׸���

	//�ϵ���� �����, ���̹��� ���		
	g_pD3DDevice->SetRenderTarget(0, pHWBackBuffer);
	g_pD3DDevice->SetDepthStencilSurface(pHWDepthStencilBuffer);

	pHWBackBuffer->Release();
	pHWBackBuffer = NULL;
	pHWDepthStencilBuffer->Release();
	pHWDepthStencilBuffer = NULL;


	// �׸��� ������ ���̴� ������������ ����
	gShader->SetVector("gWorldLightPos", &gWorldLightPos);
	gShader->SetMatrix("gWorldMat", &matModel);    //��ȯü
	gShader->SetMatrix("gLightViewMat", &matLightView);
	gShader->SetMatrix("gLightProjMat", &matLightProjection);
	gShader->SetMatrix("gViewProjMat", &matViewProjection);

	gShader->SetVector("gObjectColor", &gMeshColor);

	gShader->SetTexture("ShadowMap_Tex", gShadowRenderTarget);

	UINT numPasses = 0;
	gShader->Begin(&numPasses, NULL);
	{
		for (UINT i = 0; i < numPasses; ++i)
		{
			gShader->BeginPass(i);
			{
				for(int j = 0; j < 50; j++)
				gMesh->DrawSubset(j);


				gShader->SetMatrix("gWorldMat", &matDiscWorld);
				gShader->SetVector("gObjectColor", &gBackgroundColor);
				
				gShader->CommitChanges();
				
				gBackgroundMesh->DrawSubset(0);
			}
			gShader->EndPass();
		}
	}
	gShader->End();
}



