#pragma once

class iMap;

class cCrtController
{


private:

	float						m_fCubeSpeed;
	float						m_fAngle;

	D3DXMATRIXA16				m_matWorld;
	D3DXMATRIXA16				matR;

	float						m_fGravity;
	float						m_fVY;

	int							DoubleJump;

	SYNTHESIZE(D3DXVECTOR3, m_vCubePos, Position);
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vCubeDir, Direction);
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vDirection1, Direction1);
public:


	cCrtController(void);
	~cCrtController(void);

	void Setup();
	void Update(iMap* pMap = NULL);

	D3DXMATRIXA16* GetWorldTM()
	{
		return &m_matWorld;
	}
};

