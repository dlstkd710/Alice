#pragma once

class iMap;
class cAnimation;

class CharacterClass
{
	enum eState
	{
		E_STATE_NORMAL,
		E_STATE_JUMPUP,
		E_STATE_JUMPDOUBLE,
		E_STATE_JUMPDOWN,
		E_STATE_1COMBO,
		E_STATE_2COMBO,
		E_STATE_3COMBO,
		E_STATE_4COMBO,
		E_STATE_IDEL,
		E_STATE_RUNIDEL
	};
private:
	cAnimation*					_Animation;
	cAnimation*					_Knife;
	DWORD						dwNewTrack;
	LPD3DXANIMATIONCONTROLLER	pAC;
	LPD3DXANIMATIONSET			pAS;
	DWORD						m_dwCurrentTrack;
	DWORD						m_dwAnimIdxLoiter;
	DWORD						m_dwAnimIdxWalk;
	DWORD						m_dwAnimIdxJog;
	double						m_dTimeCurrent;
	float						m_fSpeed;
	float						m_fSpeedWalk;
	int							n, m;


	float						m_fCubeSpeed;
	float						m_fAngle;

	D3DXMATRIXA16				m_matWorld;
	D3DXMATRIXA16				matR;

	eState						m_eState;
	float						m_fGravity;
	float						m_fVY;

	int							DoubleJump;

	bool						AniOnOff;
	bool						OnKnife;


	SYNTHESIZE(D3DXVECTOR3, m_vCubePos, Position);
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vCubeDir, Direction);
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vDirection1, Direction1);

public:
	CharacterClass();
	~CharacterClass();
	void SetupMom(cAnimation * ANI, 
		char* foderName,
		char * fileName,
		int intdex, 
		D3DXVECTOR3 pos);
	void SetUp();
	void Controll(iMap* pMap = NULL);
	void Render();
	void SetintdexAnimationLep(int intdex);

	//void SetAnimation(LPD3DXANIMATIONSET animSet);
	//
	//void Play(int animIndex, float crossFadeTime);
	//
	//void PlayOneShot(std::string animName, float inCrossFadeTime, float outCrossFadeTime);
	//
	//void SetPlaySpeed(float speed);

private:
	void UpdateNormal(iMap* pMap = NULL);
	void UpdateJumpUp(iMap* pMap = NULL);
	void UpdateJumpDown(iMap* pMap = NULL);
	void UpdateJumpUpUp(iMap* pMap = NULL);
	void Update1combo(iMap* pMap = NULL);
	void Update2combo(iMap* pMap = NULL);
	void Update3combo(iMap* pMap = NULL);
	//void Update4combo(iMap* pMap = NULL);
	//void Updatefibel(iMap* pMap = NULL);
	void UpdatefRunAndIdel(iMap* pMap = NULL);
};

