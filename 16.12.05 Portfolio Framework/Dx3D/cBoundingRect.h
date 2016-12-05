#pragma once
class cBoundingRect
{
private:
	LPD3DXMESH		m_pBoxMesh;

	SYNTHESIZE(D3DXVECTOR3, m_vMin, vMin);
	SYNTHESIZE(D3DXVECTOR3, m_vMax, vMax);

public:
	HRESULT Setup(D3DXVECTOR3* pMin, D3DXVECTOR3* pMax);
	void Render();
	void Release();

public:
	cBoundingRect();
	~cBoundingRect();
};

