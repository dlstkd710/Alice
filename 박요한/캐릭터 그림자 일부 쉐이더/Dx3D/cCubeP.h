#pragma once

class cCubeP
{
public:
	cCubeP(void);
	~cCubeP(void);

private:
	std::vector<ST_P_VERTEX>	m_vecVertex;
	LPD3DXEFFECT				m_pEffect;

public:
	virtual void Setup();
	virtual void Render();

	LPD3DXEFFECT LoadEffect( const char* szFileName );
};

