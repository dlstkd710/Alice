#include "StdAfx.h"
#include "cCubeP.h"


cCubeP::cCubeP(void)
	: m_pEffect(NULL)
{
}


cCubeP::~cCubeP(void)
{
	SAFE_RELEASE(m_pEffect);
}

void cCubeP::Setup()
{
	ST_P_VERTEX v;

	v.p = D3DXVECTOR3(-1.0f, -1.0f, -1.0f); m_vecVertex.push_back(v); 
	v.p = D3DXVECTOR3(-1.0f,  1.0f, -1.0f); m_vecVertex.push_back(v); 
	v.p = D3DXVECTOR3( 1.0f,  1.0f, -1.0f); m_vecVertex.push_back(v); 

	v.p = D3DXVECTOR3(-1.0f, -1.0f, -1.0f); m_vecVertex.push_back(v);  
	v.p = D3DXVECTOR3( 1.0f,  1.0f, -1.0f); m_vecVertex.push_back(v); 
	v.p = D3DXVECTOR3( 1.0f, -1.0f, -1.0f); m_vecVertex.push_back(v); 

	v.p = D3DXVECTOR3(-1.0f, -1.0f,  1.0f); m_vecVertex.push_back(v);  
	v.p = D3DXVECTOR3( 1.0f,  1.0f,  1.0f); m_vecVertex.push_back(v); 
	v.p = D3DXVECTOR3(-1.0f,  1.0f,  1.0f); m_vecVertex.push_back(v); 

	v.p = D3DXVECTOR3(-1.0f, -1.0f,  1.0f); m_vecVertex.push_back(v);  
	v.p = D3DXVECTOR3( 1.0f, -1.0f,  1.0f); m_vecVertex.push_back(v); 
	v.p = D3DXVECTOR3( 1.0f,  1.0f,  1.0f); m_vecVertex.push_back(v); 

	v.p = D3DXVECTOR3(-1.0f, -1.0f,  1.0f); m_vecVertex.push_back(v);  
	v.p = D3DXVECTOR3(-1.0f,  1.0f,  1.0f); m_vecVertex.push_back(v); 
	v.p = D3DXVECTOR3(-1.0f,  1.0f, -1.0f); m_vecVertex.push_back(v); 

	v.p = D3DXVECTOR3(-1.0f, -1.0f,  1.0f); m_vecVertex.push_back(v);  
	v.p = D3DXVECTOR3(-1.0f,  1.0f, -1.0f); m_vecVertex.push_back(v); 
	v.p = D3DXVECTOR3(-1.0f, -1.0f, -1.0f); m_vecVertex.push_back(v); 

	v.p = D3DXVECTOR3( 1.0f, -1.0f, -1.0f); m_vecVertex.push_back(v);  
	v.p = D3DXVECTOR3( 1.0f,  1.0f, -1.0f); m_vecVertex.push_back(v); 
	v.p = D3DXVECTOR3( 1.0f,  1.0f,  1.0f); m_vecVertex.push_back(v); 

	v.p = D3DXVECTOR3( 1.0f, -1.0f, -1.0f); m_vecVertex.push_back(v);  
	v.p = D3DXVECTOR3( 1.0f,  1.0f,  1.0f); m_vecVertex.push_back(v); 
	v.p = D3DXVECTOR3( 1.0f, -1.0f,  1.0f); m_vecVertex.push_back(v); 

	v.p = D3DXVECTOR3(-1.0f,  1.0f, -1.0f); m_vecVertex.push_back(v);  
	v.p = D3DXVECTOR3(-1.0f,  1.0f,  1.0f); m_vecVertex.push_back(v); 
	v.p = D3DXVECTOR3( 1.0f,  1.0f,  1.0f); m_vecVertex.push_back(v); 

	v.p = D3DXVECTOR3(-1.0f,  1.0f, -1.0f); m_vecVertex.push_back(v);  
	v.p = D3DXVECTOR3( 1.0f,  1.0f,  1.0f); m_vecVertex.push_back(v); 
	v.p = D3DXVECTOR3( 1.0f,  1.0f, -1.0f); m_vecVertex.push_back(v); 

	v.p = D3DXVECTOR3(-1.0f, -1.0f,  1.0f); m_vecVertex.push_back(v);  
	v.p = D3DXVECTOR3(-1.0f, -1.0f, -1.0f); m_vecVertex.push_back(v); 
	v.p = D3DXVECTOR3( 1.0f, -1.0f, -1.0f); m_vecVertex.push_back(v); 

	v.p = D3DXVECTOR3(-1.0f, -1.0f,  1.0f); m_vecVertex.push_back(v);  
	v.p = D3DXVECTOR3( 1.0f, -1.0f, -1.0f); m_vecVertex.push_back(v); 
	v.p = D3DXVECTOR3( 1.0f, -1.0f,  1.0f); m_vecVertex.push_back(v); 

	m_pEffect = LoadEffect("shaderXDiffuse.fx");
}

void cCubeP::Render()
{
	D3DXMATRIXA16 matWorld, matView, matProjection;
	D3DXMatrixIdentity(&matWorld);
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProjection);

	// 쉐이더 전역변수들을 설정
	m_pEffect->SetMatrix("matWorld", &matWorld);
	m_pEffect->SetMatrix("matView",  &matView);
	m_pEffect->SetMatrix("matProjection",  &matProjection);
	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		m_pEffect->SetVector("color", &D3DXVECTOR4(1, 0, 0, 1));
	}
	else
	{
		m_pEffect->SetVector("color", &D3DXVECTOR4(0, 0, 1, 1));
	}

	g_pD3DDevice->SetFVF(ST_P_VERTEX::FVF);

	UINT numPasses = 0;
	m_pEffect->Begin(&numPasses, NULL);

	for (UINT i = 0; i < numPasses; ++i )
	{
		m_pEffect->BeginPass(i);

		g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 
			m_vecVertex.size() / 3, 
			&m_vecVertex[0], 
			sizeof(ST_P_VERTEX));

		m_pEffect->EndPass();
	}

	m_pEffect->End();
}

LPD3DXEFFECT cCubeP::LoadEffect( const char* szFileName )
{
	LPD3DXEFFECT pEffect = NULL;

	// 셰이더 로딩
	LPD3DXBUFFER		pError = NULL;			//에러 버퍼 ( 셰이더를 컴파일할때 잘못 된 문법이나 오류정보를 리턴해주는 버퍼 )
	DWORD				dwShaderFlag = 0;		//셰이더 플레그 0 

#ifdef _DEBUG
	dwShaderFlag = dwShaderFlag | D3DXSHADER_DEBUG;		//셰이더를 디버그모드로 컴파일하겠다 ( 디버그모드로 해야 잘못된 컴파일 오류가 날때 Error 버퍼에 오류정보가 들어간다 ) 
#endif

	//fx 파일로 부터 셰이더 객체 생성
	D3DXCreateEffectFromFile(
		g_pD3DDevice,				// 디바이스
		szFileName,					// 불러올 셰이더 코드 파일이름
		NULL,						// 셰이더를 컴파일할때 추가로 사용할 #define 정의 ( 일단 NULL )
		NULL,						// 셰이더를 컴파일할때 #include 지시문을 처리할때 사용할 인터페이스 플레그 ( 일단 NULL )
		dwShaderFlag,				// 셰이더 컴파일 플레그
		NULL,						// 셰이더 매개변수를 공유할 메모리풀 ( 일단 NULL )
		&pEffect,					// 로딩될 셰이더 Effect 포인터
		&pError						// 셰이더를 로딩하고 컴파일할때 문제가 생기면 해당 버퍼에 에러메시지가 들어간다 ( 성공적으로 로딩되면 NULL 이 참조된다 )
		);

	//셰이더 파일로딩에문재가 있다면..
	if( pError != NULL || pEffect == NULL ){

		//문제의 내용이 뭔지 문자열로 확인
		int size = pError->GetBufferSize();
		char* str = new char[ size ];

		//str에 버퍼에있는 내용을 저장한다.
		sprintf_s( str, size,(const char*)pError->GetBufferPointer() );

		OutputDebugString(str);
		//오류내용을 출력했으니 오류버퍼 해제
		SAFE_RELEASE( pError );
		if (str)
		{
			delete [] str;
		}

		return NULL;
	}

	return pEffect;
}
