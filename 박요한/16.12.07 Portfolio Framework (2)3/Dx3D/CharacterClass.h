#pragma once
#include "All_struct.h"
#include <queue>

class iMap;
class cAnimation;
class EffectClass;

class CharacterClass
{

private:
	cAnimation*					_Animation;			//¿¤¸®½º Ä³¸¯ÅÍ
	cAnimation*					_Knife;				//weapon Ä®
	cAnimation*					_Gun;				//weapon ÃÑ
	cAnimation*					_Hammor;			//weapon ÇØ¸Ó
	EffectClass*				m_pEffectClass;

	LPD3DXANIMATIONCONTROLLER	pAC;

	float						m_fSpeed;
	int							n, a, t, r, i, c;



	float						m_fCubeSpeed;
	float						m_fAngle;


	D3DXMATRIXA16				matR;

	eState						m_eState;
	float						m_fGravity;
	float						m_fVY;

	bool						m_isOnceLoop;

	float						m_fBlendTime;
	float						m_fPassedBlendTime;
	bool						m_isBlending;
	bool						m_Temp1;

	float						m_fWeight;
	queue<eState>				combo;
	vector<int>                 d;
	float						d_1;



	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vCubePos, Position);
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vCubeDir, Direction);
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vDirection1, Direction1);
	SYNTHESIZE_PASS_BY_REF(D3DXMATRIXA16, m_matWorld, WorldTM);
	SYNTHESIZE_PASS_BY_REF(D3DXMATRIXA16, matT, matTm);

public:
	CharacterClass();
	~CharacterClass();

	void SetUp();
	void Release();
	void Controll(iMap * pMap, float CameraAngleX, float CameraAngleY);
	void Render();
	void SetintdexAnimationLep(int intdex);
	void isBlending();

private:
	void UpdateNormal(iMap* pMap = NULL);
	//=== JUMP ===//
	void UpdateJumpUp(iMap* pMap = NULL);
	void UpdateJumpDown(iMap* pMap = NULL);
	void UpdateJumpUpUp(iMap* pMap = NULL);
	void UpdateTelpo(iMap *pMap = NULL);

	//=== GUN ===//
	void UpdateGunIdel(iMap* pMap = NULL);

	void UpdateGunRunMotion(iMap* pMap = NULL);
	//===KNIFE===//
	void UpdatefRunAndIdel(iMap* pMap = NULL);

	void Update1combo(iMap* pMap = NULL);
	void Update2combo(iMap* pMap = NULL);
	void Update3combo(iMap* pMap = NULL);

	//===HAMMOR===//
	void UpdateHammor(iMap* pMap = NULL);

	void UpdateH1combo(iMap* pMap = NULL);
	void UpdateH2combo(iMap* pMap = NULL);
	void UpdateH3combo(iMap* pMap = NULL);


	void Renderstate();


private:
	void MotionControllTower(iMap *pMap);
};

