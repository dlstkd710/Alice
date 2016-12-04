#pragma once

class iMap;

class cCrtController
{
	enum eState
	{
		E_STATE_NORMAL,
		E_STATE_JUMPUP,
		E_STATE_JUMPDOUBLE,
		E_STATE_JUMPDOWN
	};

private:
	float						m_fCubeSpeed;
	float						m_fAngle;
	//D3DXVECTOR3					m_vCubePos;
	D3DXMATRIXA16				m_matWorld;
	D3DXMATRIXA16				matR;

	eState			m_eState;
	float			m_fGravity;
	float			m_fVY;

	int				DoubleJump;
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

private:
	void UpdateNormal(iMap* pMap = NULL);
	void UpdateJumpUp(iMap* pMap = NULL);
	void UpdateJumpDown(iMap* pMap = NULL);
	void UpdateJumpUpUp(iMap* pMap = NULL);
};

