#pragma once

class cCube
{
private:
	std::vector<ST_PC_VERTEX>	m_vecVertex;
	D3DXMATRIXA16				m_matWorld;
	D3DXVECTOR3					m_vPosition;
	D3DXVECTOR3					m_vDirection;
	float						m_fAngle;
	float						m_fSpeed;

public:
	cCube(void);
	~cCube(void);

	void Setup();
	void Update();
	void Render();
	D3DXVECTOR3* GetPosition();
};

