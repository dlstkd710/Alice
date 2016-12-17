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

	// ���̴� ������������ ����
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

	// ���̴� �ε�
	LPD3DXBUFFER		pError = NULL;			//���� ���� ( ���̴��� �������Ҷ� �߸� �� �����̳� ���������� �������ִ� ���� )
	DWORD				dwShaderFlag = 0;		//���̴� �÷��� 0 

#ifdef _DEBUG
	dwShaderFlag = dwShaderFlag | D3DXSHADER_DEBUG;		//���̴��� ����׸��� �������ϰڴ� ( ����׸��� �ؾ� �߸��� ������ ������ ���� Error ���ۿ� ���������� ���� ) 
#endif

	//fx ���Ϸ� ���� ���̴� ��ü ����
	D3DXCreateEffectFromFile(
		g_pD3DDevice,				// ����̽�
		szFileName,					// �ҷ��� ���̴� �ڵ� �����̸�
		NULL,						// ���̴��� �������Ҷ� �߰��� ����� #define ���� ( �ϴ� NULL )
		NULL,						// ���̴��� �������Ҷ� #include ���ù��� ó���Ҷ� ����� �������̽� �÷��� ( �ϴ� NULL )
		dwShaderFlag,				// ���̴� ������ �÷���
		NULL,						// ���̴� �Ű������� ������ �޸�Ǯ ( �ϴ� NULL )
		&pEffect,					// �ε��� ���̴� Effect ������
		&pError						// ���̴��� �ε��ϰ� �������Ҷ� ������ ����� �ش� ���ۿ� �����޽����� ���� ( ���������� �ε��Ǹ� NULL �� �����ȴ� )
		);

	//���̴� ���Ϸε������簡 �ִٸ�..
	if( pError != NULL || pEffect == NULL ){

		//������ ������ ���� ���ڿ��� Ȯ��
		int size = pError->GetBufferSize();
		char* str = new char[ size ];

		//str�� ���ۿ��ִ� ������ �����Ѵ�.
		sprintf_s( str, size,(const char*)pError->GetBufferPointer() );

		OutputDebugString(str);
		//���������� ��������� �������� ����
		SAFE_RELEASE( pError );
		if (str)
		{
			delete [] str;
		}

		return NULL;
	}

	return pEffect;
}
