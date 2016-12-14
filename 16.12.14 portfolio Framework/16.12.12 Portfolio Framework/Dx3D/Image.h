#pragma once
class Image
{
private:
	SYNTHESIZE(float, m_x, image_x);
	SYNTHESIZE(float, m_y, image_y);
	SYNTHESIZE(float, m_z, image_z);
	SYNTHESIZE(float, m_width, image_width);
	SYNTHESIZE(float, m_height, image_height);
	SYNTHESIZE_PASS_BY_REF(D3DXIMAGE_INFO, m_stImageInfo,stImageInfo);
	RECT rext;
	SYNTHESIZE_PASS_BY_REF(LPDIRECT3DTEXTURE9, m_pTexture,pTexture);
	SYNTHESIZE_PASS_BY_REF(LPD3DXSPRITE, m_pSprite,pSprite);
public:
	Image();
	~Image();
	void Setup(char *strName,float x, float y, float z);
	void Render(int x, int y, int z);
	void Render();
	void Render(D3DXMATRIXA16 *m_pView);
	void Destroy();
};

