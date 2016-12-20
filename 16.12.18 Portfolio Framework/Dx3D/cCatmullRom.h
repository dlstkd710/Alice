#pragma once

class cCatmullRom
{
private:
	SYNTHESIZE(int, m_UpdateCnt, UpdateCnt);

	SYNTHESIZE(vector<D3DXVECTOR3>,			m_vPos,	vPos);

	SYNTHESIZE_ADD_REF(LPDIRECT3DTEXTURE9,	m_pTexture, Texture);
	
public:
	cCatmullRom();
	~cCatmullRom();

	void Setup(IN const D3DXVECTOR3 UpPos, const IN D3DXVECTOR3 DownPos);
	void Release();
	void Update();
	void Render();

	void SetQueue(IN const D3DXVECTOR3 UpPos, const IN D3DXVECTOR3 DownPos);
	void CalcCatmullRom();
};