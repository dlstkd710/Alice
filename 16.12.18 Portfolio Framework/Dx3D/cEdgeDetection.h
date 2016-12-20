#pragma once

class cEdgeDetection
{
private:
	SYNTHESIZE(LPD3DXMESH,			m_TargetMesh,		TargetMesh);
	SYNTHESIZE(LPD3DXEFFECT,		m_EdgeEffect,		EdgeEffect);

public:
	cEdgeDetection();
	~cEdgeDetection();

	void Setup();
	void Release();
	void Update(/* IN Object Ref */);
	void Render(D3DXMATRIXA16* matViewProj, D3DXMATRIXA16* matWorld);
};

