#include "stdafx.h"
#include "Shadow.h"
Shadow::Shadow(){}
Shadow::~Shadow() {}

void Shadow::SetUp()
{
	gMeshColor = D3DXVECTOR4(0.2f, 0.5f, 1.0f, 1);
	gBackgroundColor = D3DXVECTOR4(0.8f, 0.8f, 0.8f, 1);
	gWorldLightPos = D3DXVECTOR4(500.0f, 500.0f, -500.0f, 1.0f);
	gWorldCameraPos = D3DXVECTOR4(0.0f, 0.0f, -200.0f, 1.0f);
}

bool Shadow::InitShadowRenderTarget()
{
	//렌더타겟 생성
	const int shadowMapSize = 1024;
	g_pD3DDevice->CreateTexture(
		shadowMapSize, shadowMapSize,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_R32F, D3DPOOL_DEFAULT,
		&gShadowRenderTarget, NULL);


	//그림자맵과 동일한 크기의 깊이버퍼 생성
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
	//월드행렬
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);

	//뷰행렬
	D3DXMATRIXA16 matView;
	static D3DXVECTOR3 inOutEyePos(0, 0, -100);
	D3DXVECTOR3 retLookatPos(0, 0, 0);
	D3DXVECTOR3 upVec(0, 1, 0);

	D3DXMatrixLookAtLH(&matView, &inOutEyePos, &retLookatPos, &upVec);

	//투영행렬
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


// 쉐이더 로딩
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

	// 쉐이더 에러 출력
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

bool Shadow::LoadAsset()
{
	// 셰이더 로딩
	gShader = LoadShader("ApplyShadow.fx");
	if (gShader == NULL)
		return false;

	//CreateShadow 셰이더 로딩
	gCreateShadowShader = LoadShader("CreateShadow.fx");
	if (gCreateShadowShader == NULL)
		return false;

	return true;
}


void Shadow::RenderScene(D3DXMATRIXA16* matworld, D3DXVECTOR3 cubePos)
{
	// 광원-뷰 행렬
	D3DXMATRIXA16 matLightView;
	{
		D3DXVECTOR3 vEyePt(gWorldLightPos.x, gWorldLightPos.y, gWorldLightPos.z);
		D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&matLightView, &vEyePt, &vLookatPt, &vUpVec);
	}

	// 광원-투영 행렬
	D3DXMATRIXA16 matLightProjection;
	D3DXMatrixPerspectiveFovLH(&matLightProjection, D3DX_PI / 4.0f, 1, 1, 3000);

	// 뷰-투영행렬
	D3DXMATRIXA16 matViewProjection;
	{
		//뷰행렬
		D3DXMATRIXA16 matView;
		D3DXVECTOR3 vEyePt(cubePos.x, cubePos.y, cubePos.z);
		D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);

		//투영행렬
		D3DXMATRIXA16 matProjection;
		RECT rc;
		GetClientRect(g_hWnd, &rc);
		D3DXMatrixPerspectiveFovLH(&matProjection, D3DX_PI / 4.0f, rc.right / (float)rc.bottom, 1, 1000);
		D3DXMatrixMultiply(&matViewProjection, &matView, &matProjection);
	}


	// 현재 하드웨어 백버퍼와 깊이버퍼를 보관한다.
	LPDIRECT3DSURFACE9 pHWBackBuffer = NULL;
	LPDIRECT3DSURFACE9 pHWDepthStencilBuffer = NULL;

	g_pD3DDevice->GetRenderTarget(0, &pHWBackBuffer);
	g_pD3DDevice->GetDepthStencilSurface(&pHWDepthStencilBuffer);

	//1. 그림자 만들기

	// 그림자 맵의 렌더타깃과 깊이버퍼를 사용한다.
	LPDIRECT3DSURFACE9 pShadowSurface = NULL;
	if (SUCCEEDED(gShadowRenderTarget->GetSurfaceLevel(0, &pShadowSurface)))
	{
		g_pD3DDevice->SetRenderTarget(0, pShadowSurface);
		pShadowSurface->Release();
		pShadowSurface = NULL;
	}
	g_pD3DDevice->SetDepthStencilSurface(gShadowDepthStencil);

	//이전 프레임에 그렸던 그림자 정보를 지움
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), 0xFFFFFFFF, 1.0f, 0);

	// 그림자 만들기 쉐이더 전역변수들을 설정
	gCreateShadowShader->SetMatrix("gWorldMat", matworld);
	gCreateShadowShader->SetMatrix("gLightViewMat", &matLightView);
	gCreateShadowShader->SetMatrix("gLightProjMat", &matLightProjection);

	// 그림자 만들기 쉐이더를 시작
	{
		UINT numPasses = 0;
		gCreateShadowShader->Begin(&numPasses, NULL);
		{
			for (UINT i = 0; i < numPasses; ++i)
			{
				gCreateShadowShader->BeginPass(i);
				gCreateShadowShader->EndPass();
			}
		}
		gCreateShadowShader->End();
	}


	// 2. 그림자 그리기

	//하드웨어 백버퍼, 깊이버퍼 사용
	g_pD3DDevice->SetRenderTarget(0, pHWBackBuffer);
	g_pD3DDevice->SetDepthStencilSurface(pHWDepthStencilBuffer);

	pHWBackBuffer->Release();
	pHWBackBuffer = NULL;
	pHWDepthStencilBuffer->Release();
	pHWDepthStencilBuffer = NULL;


	// 그림자 입히기 쉐이더 전역변수들을 설정
	gShader->SetMatrix("gWorldMat", matworld);	//원환체
	gShader->SetMatrix("gLightViewMat", &matLightView);
	gShader->SetMatrix("gLightProjMat", &matLightProjection);
	gShader->SetMatrix("gViewProjMat", &matViewProjection);

	gShader->SetVector("gWorldLightPos", &gWorldLightPos);
	gShader->SetVector("gWorldCameraPos", &gWorldCameraPos);

	gShader->SetVector("gObjectColor", &gMeshColor);

	gShader->SetTexture("ShadowMap_Tex", gShadowRenderTarget);

	UINT numPasses = 0;
	gShader->Begin(&numPasses, NULL);
	{
		for (UINT i = 0; i < numPasses; ++i)
		{
			gShader->BeginPass(i);
			{
				gShader->CommitChanges();
			}
			gShader->EndPass();
		}
	}
	gShader->End();
}



