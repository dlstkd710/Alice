#pragma once
class cBoundingSphere;
class cRay
{
private:
	SYNTHESIZE(D3DXVECTOR3, m_vOri, Ori);
	SYNTHESIZE(D3DXVECTOR3, m_vDir, Dir);

public:
	static cRay CalcViewSpace(int _x, int _y);
	static cRay CalcWorldSpace(int _x, int _y);
	static cRay CalcViewSpace(POINT _pos);
	static cRay CalcWorldSpace(POINT _pos);
	cRay		WorldToLocal(D3DXVECTOR3 v);
	bool		IsPick(cBoundingSphere* sphere);

public:
	cRay();
	~cRay();
};

