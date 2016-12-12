#pragma once

class cBoundingSphere;
class cFrustum
{
private:
	vector<D3DXVECTOR3>		m_vecV;
	vector<D3DXPLANE>		m_vecPlane;

public:
	void Setup();
	void Update();
	bool IsinFrustum(cBoundingSphere* pSphere);

public:
	cFrustum();
	~cFrustum();
};

