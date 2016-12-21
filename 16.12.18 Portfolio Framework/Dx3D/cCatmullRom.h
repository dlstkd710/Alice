#pragma once

#define LINENUM		100 * 3

class cCatmullRom
{
	enum tagUpDown
	{
		Up,
		Down,
	};

private:
	SYNTHESIZE(int, m_UpdateCnt, UpdateCnt);

	SYNTHESIZE(vector<D3DXVECTOR3>, m_vPos, vPos);

	SYNTHESIZE_ADD_REF(LPDIRECT3DTEXTURE9, m_pTexture, Texture);

	vector<ST_PC_VERTEX> VertexBuffer;

public:
	cCatmullRom();
	~cCatmullRom();

	void Setup(IN const D3DXVECTOR3 UpPos, const IN D3DXVECTOR3 DownPos);
	void Release();
	void Update(IN const D3DXVECTOR3 UpPos, const IN D3DXVECTOR3 DownPos);
	void Render();

	void CalcCatmullRom();
	void Reset(IN const D3DXVECTOR3 UpPos, IN const D3DXVECTOR3 DownPos);
	void PosPull();
	void VertexPull();
};