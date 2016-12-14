#pragma once
#include "stdafx.h"

#include "iMap.h"

class Terrain : public iMap
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
	int							nCol;
	int							nRow;
	vector<unsigned char>		vecData;
	std::vector<ST_PNT_VERTEX>  vecVertex;

public:
	Terrain();
	~Terrain();

	void Load(char* szFilename, char* szTexture, int nBytesPerPixel = 1);

	virtual bool GetHeight(IN float x, OUT float& y, IN float z) override;

	//bool readRawFile(string fileName);
	virtual float GetHeight2(IN float x, IN float z) override;
	virtual	void Render() override;

	int getHeightmapEntry(int row, int col);

	float Lerp(float a, float b, float t);

};

