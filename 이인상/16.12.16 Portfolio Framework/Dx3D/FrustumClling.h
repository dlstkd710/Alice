#pragma once

#define ang 0.5f

class FrustumClling
{
protected:

	vector<D3DXVECTOR3> m_VecDot;
	D3DXVECTOR3	m_vPos;	
	D3DXPLANE	m_plane[6];


public:
	FrustumClling();
	~FrustumClling();

	BOOL Make(D3DXMATRIXA16 * pmatViewProj);
	BOOL	IsInSphere(D3DXVECTOR3* pv, float radius);
	D3DXVECTOR3* GetPos() { return &m_vPos; }

};


