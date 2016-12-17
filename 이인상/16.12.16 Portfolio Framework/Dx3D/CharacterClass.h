#pragma once
#include "All_struct.h"

class iMap;
class cAnimation;


class CharacterClass
{

private:
	cAnimation*					_Animation;
	cAnimation*					_Knife;
	cAnimation*					_Gun;
	cAnimation*					_Hammor;
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
	int							n, m, a, t;
	std::vector<ST_PCT_VERTEX>	m_vecVertex1;

	float						m_fCubeSpeed;
	float						m_fAngle;

	D3DXMATRIXA16				m_matWorld;
	D3DXMATRIXA16				matR;
	D3DXMATRIXA16				matT;

	eState						m_eState;
	float						m_fGravity;
	float						m_fVY;

	bool						m_isOnceLoop;
	bool						m_isOnceLoop2;

	float						m_fBlendTime;
	float						m_fPassedBlendTime;
	bool						m_isBlending;


	SYNTHESIZE(D3DXVECTOR3, m_vCubePos, Position);
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vCubeDir, Direction);
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vDirection1, Direction1);

	int c;

public:
	CharacterClass();
	~CharacterClass();

	void SetUp();
	void Release();
	void Controll(iMap * pMap, float CameraAngleX, float CameraAngleY);
	void Render();
	void SetintdexAnimationLep(int intdex);
	void isBlending();
	void SetisOnceTimeAnimation(int index);
	D3DXMATRIXA16* GetWorldTM()
	{
		return &m_matWorld;
	}

private:
	void UpdateNormal(iMap* pMap = NULL);
	void UpdateJumpUp(iMap* pMap = NULL);
	void UpdateJumpDown(iMap* pMap = NULL);
	void UpdateJumpUpUp(iMap* pMap = NULL);
	void UpdateTelpo(iMap *pMap = NULL);
	void Update1combo(iMap* pMap = NULL);
	void Update2combo(iMap* pMap = NULL);
	void Update3combo(iMap* pMap = NULL);
	void UpdatefRunAndIdel(iMap* pMap = NULL);
	void UpdateGunIdel(iMap* pMap = NULL);
	void UpdateHammor(iMap* pMap = NULL);
	//void UpDateGunRunMotion(iMap * pMap = NULL);


	void Renderstate();


private:
	void MotionControllTower(iMap *pMap);
};

