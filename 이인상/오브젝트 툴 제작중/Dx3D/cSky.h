#pragma once

class cSky
{
private:
	

private:
	SYNTHESIZE_ADD_REF(LPDIRECT3DCUBETEXTURE9,	m_pSkyTexture,	SkyTexture);
	SYNTHESIZE_ADD_REF(LPD3DXMESH,				m_pSkyMesh,		SkyMesh);
	SYNTHESIZE_ADD_REF(LPD3DXEFFECT,			m_pSkyEffect,	SkyEffect);

public:
	cSky();
	~cSky();

	HRESULT Init();
	void Release();
	void Update();
	void Render(D3DXMATRIXA16* matWorld, D3DXMATRIXA16* matViewProj);
};