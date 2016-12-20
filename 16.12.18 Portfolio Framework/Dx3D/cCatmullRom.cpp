#include "stdafx.h"
#include "cCatmullRom.h"


cCatmullRom::cCatmullRom()
	: m_UpdateCnt(0)
{
}

cCatmullRom::~cCatmullRom()
{
}

void cCatmullRom::Setup(IN const D3DXVECTOR3 UpPos, IN const D3DXVECTOR3 DownPos)
{
	SetQueue(UpPos, DownPos);
	
	m_pTexture = g_pTextureManager->GetTexture("FilePath.tga");
}

void cCatmullRom::Release()
{
	if (m_pTexture)
		m_pTexture->Release();
}

void cCatmullRom::Update()
{
	m_UpdateCnt++;
	if (m_UpdateCnt % 5 == 0)
	{
		// TODO : Position Value 넣어야 함
		//SetQueue()
	}

}

void cCatmullRom::Render()
{
	//// Clear the backbuffer to a blue color
	//g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	//
	//// Begin the scene
	//if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	//{
	//
	//	g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	//
	//	g_pd3dDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, LINESIZE - 1, S_RomLine, sizeof(CUSTOMVERTEX));
	//
	//	// End the scene
	//	g_pd3dDevice->EndScene();
	//}
	//
	//// Present the backbuffer contents to the display
	//g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}

void cCatmullRom::SetQueue(IN const D3DXVECTOR3 UpPos, const IN D3DXVECTOR3 DownPos)
{
	m_vPos.push_back(UpPos);
	m_vPos.push_back(DownPos);
}

void cCatmullRom::CalcCatmullRom()
{
	
}

/* 
1. 4 * 2 개의 Vertex
2. 보간 갯수 설정 하기
3. 
*/