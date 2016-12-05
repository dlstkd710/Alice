#pragma once

#include "stdafx.h"

#include "iMap.h"

class cTerrain : public iMap
{
public:
	//터레인 정점 구조체
	typedef struct tagTERRAINVERTEX{
		D3DXVECTOR3 pos;
		D3DXVECTOR3  n;
		D3DXVECTOR2  t;
		enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL |  D3DFVF_TEX1 };
	}TERRAINVERTEX, *LPTERRAINVERTTEX;

	//터레인 인덱스 구조체 ( 폴리곤 하나의 인덱싱 )
	typedef struct tagTERRAININDEX{
		DWORD		_0;
		DWORD		_1;
		DWORD		_2;
	}TERRAININDEX, *LPTERRAININDEX;

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)

private:
	LPDIRECT3DVERTEXBUFFER9				terrainVb;
	LPDIRECT3DINDEXBUFFER9				terrainIb;
	LPDIRECT3DVERTEXDECLARATION9		terrainDecl;			//정점 구조체 선언

	float								fCellScale;				//셀의 간격

	int									nVerNumX;				//가로 정점수
	int									nVerNumZ;				//깊이 정점수
	int									nTotalVerNum;			//총 정점수

	int									nCellNumX;				//가로 셀수
	int									nCellNumZ;				//깊이 셀수
	int									nTotalCellNum;			//총 셀수
	int									nTotalTri;				//삼각형 갯수


	float								fTerrainSizeZ;			//터레인 가로 사이즈
	float								fTerrainSizeX;			//터레인 세로 사이즈

	D3DXVECTOR3*						vertexPoses;			//정점 위치 배열
	LPTERRAININDEX						pIndices;				//인덱스 정보
	LPTERRAINVERTTEX                    pVertex;
	D3DMATERIAL9						m_stMtl;

	LPDIRECT3DTEXTURE9                  m_pkTexture;
	LPDIRECT3DTEXTURE9                  m_pkTexBmp;
	LPDIRECT3DTEXTURE9					m_pTexDiffuse;

	DWORD     g_cxHeight;
	DWORD     g_czHeight;


	//LPDIRECT3DTEXTURE9       g_pTexHeightMap = nullptr;  // HeightMap에 사용할 텍스처

public:
	float y;
	vector<D3DXVECTOR3*> vVertex;
	vector<LPTERRAININDEX> vIndex;

	cTerrain(void);
	~cTerrain(void);


	HRESULT Heightmap(char *szTexFile, char *szTexFile1, float cellSize);

	//해제
	void Release();

	virtual bool GetHeight(IN float x, OUT float& y, IN float z) override;

	//bool readRawFile(string fileName);
	virtual float GetHeight2(IN float x, IN float z) override;
	virtual	void Render() override;

	//랜더
};

