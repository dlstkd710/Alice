#include "stdafx.h"
#include "Image.h"


Image::Image()
	:m_x(0)
	,m_y(0)
	,m_z(0)
	,m_pSprite(NULL)
	,m_pTexture(NULL)
	,m_height(0)
	,m_width(0)
{
}


Image::~Image()
{
	SAFE_RELEASE(m_pSprite);
	SAFE_RELEASE(m_pTexture);
}

void Image::Setup(char *strName, float x, float y, float z)
{
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);
	//D3DXCreateTextureFromFile(g_pD3DDevice, "인상이가 좋아하는 설현.png", &m_pTexture);
	m_pTexture = g_pTextureManager->GetTexture(strName, &m_stImageInfo);

	m_x = x;
	m_y = y;
	m_z = z;
	rext = cUt::RectMake(0, 0, m_stImageInfo.Width, m_stImageInfo.Height);
	//SetRect(&rext, 0, 0, m_stImageInfo.Width, m_stImageInfo.Height);

	//SetRect(&rext, x, y, 300, 300);
}

void Image::Render()
{
	//D3DXMATRIXA16 matWorld;
	//D3DXMatrixTranslation(&matWorld, m_x, m_y, m_z);
	//m_pSprite->SetWorldViewLH(NULL, &matWorld);
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	//m_pSprite->SetTransform(&matWorld);
	//m_pSprite->SetWorldViewLH(NULL, &matWorld);
	m_pSprite->Draw(m_pTexture,
		&rext,
		NULL,
		&D3DXVECTOR3(m_x, m_y, m_z),
		0xffffffff);
	m_pSprite->End();
}


void Image::Render(D3DXMATRIXA16 *m_pView)
{
	//m_pSprite->SetWorldViewLH(NULL, m_pView);
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	m_pSprite->SetTransform(m_pView);
	m_pSprite->Draw(m_pTexture, &rext, NULL, &D3DXVECTOR3(m_x, m_y, m_z), 0xffffffff);
	m_pSprite->End();
}

void Image::Destroy()
{

}
