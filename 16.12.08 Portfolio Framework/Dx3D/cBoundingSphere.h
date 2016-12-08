#pragma once
class cBoundingSphere
{
private:
	LPD3DXMESH		m_pSphereMesh;

	SYNTHESIZE(D3DXVECTOR3, m_vecCenter, Center);
	SYNTHESIZE(float, m_fRadius, Radius);
	SYNTHESIZE(float, m_fOriginalY, OriginalY);

public:
	HRESULT Setup(D3DXVECTOR3* pCenter = &D3DXVECTOR3(0, 0, 0), float fRadius = 0,
		UINT nSlices = 20, UINT nStacks = 20);
	void Render();
	void Release();

public:
	cBoundingSphere();
	~cBoundingSphere();
};

