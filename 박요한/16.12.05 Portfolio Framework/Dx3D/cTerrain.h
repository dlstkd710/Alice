#pragma once

#include "stdafx.h"

#include "iMap.h"

class cTerrain : public iMap
{
public:
	//�ͷ��� ���� ����ü
	typedef struct tagTERRAINVERTEX{
		D3DXVECTOR3 pos;
		D3DXVECTOR3  n;
		D3DXVECTOR2  t;
		enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL |  D3DFVF_TEX1 };
	}TERRAINVERTEX, *LPTERRAINVERTTEX;

	//�ͷ��� �ε��� ����ü ( ������ �ϳ��� �ε��� )
	typedef struct tagTERRAININDEX{
		DWORD		_0;
		DWORD		_1;
		DWORD		_2;
	}TERRAININDEX, *LPTERRAININDEX;

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)

private:
	LPDIRECT3DVERTEXBUFFER9				terrainVb;
	LPDIRECT3DINDEXBUFFER9				terrainIb;
	LPDIRECT3DVERTEXDECLARATION9		terrainDecl;			//���� ����ü ����

	float								fCellScale;				//���� ����

	int									nVerNumX;				//���� ������
	int									nVerNumZ;				//���� ������
	int									nTotalVerNum;			//�� ������

	int									nCellNumX;				//���� ����
	int									nCellNumZ;				//���� ����
	int									nTotalCellNum;			//�� ����
	int									nTotalTri;				//�ﰢ�� ����


	float								fTerrainSizeZ;			//�ͷ��� ���� ������
	float								fTerrainSizeX;			//�ͷ��� ���� ������

	D3DXVECTOR3*						vertexPoses;			//���� ��ġ �迭
	LPTERRAININDEX						pIndices;				//�ε��� ����
	LPTERRAINVERTTEX                    pVertex;
	D3DMATERIAL9						m_stMtl;

	LPDIRECT3DTEXTURE9                  m_pkTexture;
	LPDIRECT3DTEXTURE9                  m_pkTexBmp;
	LPDIRECT3DTEXTURE9					m_pTexDiffuse;

	DWORD     g_cxHeight;
	DWORD     g_czHeight;


	//LPDIRECT3DTEXTURE9       g_pTexHeightMap = nullptr;  // HeightMap�� ����� �ؽ�ó

public:
	float y;
	vector<D3DXVECTOR3*> vVertex;
	vector<LPTERRAININDEX> vIndex;

	cTerrain(void);
	~cTerrain(void);


	HRESULT Heightmap(char *szTexFile, char *szTexFile1, float cellSize);

	//����
	void Release();

	virtual bool GetHeight(IN float x, OUT float& y, IN float z) override;

	//bool readRawFile(string fileName);
	virtual float GetHeight2(IN float x, IN float z) override;
	virtual	void Render() override;

	//����
};

