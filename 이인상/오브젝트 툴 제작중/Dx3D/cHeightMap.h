#pragma once

#include "iMap.h"

class cHeightMap : public iMap
{
protected:
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;
	int							m_nNumTri;
	int							m_nNumVertex;
	int							m_nNumTile;
	D3DMATERIAL9				m_stMtl;
	LPDIRECT3DTEXTURE9			m_pTexture;
	std::vector<D3DXVECTOR3>	m_vecVertex;
public:
	cHeightMap(void);
	virtual ~cHeightMap(void);

	void Load(char* szFilename, char* szTexture, int nBytesPerPixel = 1);
	
	virtual bool GetHeight(IN float x, OUT float& y, IN float z) override;
	virtual void Render() override;
};

