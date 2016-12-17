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
	//렌더타겟 생성
	const int shadowMapSize = 4096;
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

// 모델 로딩
LPD3DXMESH	LoadModel(const char * filename)
{
	LPD3DXMESH ret = NULL;
	D3DXLoadMeshFromX(filename, D3DXMESH_SYSTEMMEM, g_pD3DDevice, NULL, NULL, NULL, NULL, &ret);

	return ret;
}

bool Shadow::LoadAsset()
{
	// 셰이더 로딩
	// 모델 로딩
	gShader = LoadShader("ApplyShadow.fx");
	if (gShader == NULL)
		return false;

	gMesh = LoadModel("AliceW.x");
	if (gMesh == NULL)
		return false;

	//CreateShadow 셰이더 로딩
	gCreateShadowShader = LoadShader("CreateShadow.fx");
	if (gCreateShadowShader == NULL)
		return false;

	//원형 평면 메시 로딩
	gBackgroundMesh = LoadModel("Disc.x");
	if (gBackgroundMesh == NULL)
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
	gCreateShadowShader->SetMatrix("gWorldMat", &matModel);
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

				g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
 
				for (int j = 0; j < 50; j++)
					gMesh->DrawSubset(j);
				
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
	gShader->SetVector("gWorldLightPos", &gWorldLightPos);
	gShader->SetMatrix("gWorldMat", &matModel);    //원환체
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



