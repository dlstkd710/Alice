#pragma once
#include "All_struct.h"
#include <queue>

class iMap;
class cAnimation;
class EffectClass;

class CharacterClass
{
private:
	LPD3DXANIMATIONCONTROLLER	pAC;				//Animation Control

	cAnimation*					_Animation;			//엘리스 캐릭터
	cAnimation*					_Knife;				//weapon 칼
	cAnimation*					_Gun;				//weapon 총
	cAnimation*					_Hammor;			//weapon 해머
	EffectClass*				m_pEffectClass;		//Effect Class 


	int							n, a, t, r, i, c, combo1, combo3;


	D3DXMATRIXA16				matR;

	eState						m_eState;

	float						m_fGravity;				//점프력 중력값
	float						m_fVY;					//점프력
	float						m_fSpeed;				//캐릭터 이동속도
	float						m_fCubeSpeed;			//이동속도
	float						m_fBlendTime;			//애니메이션 블렌딩타임
	float						m_fPassedBlendTime;		//애니메이션 블렌딩기간
	float						m_fWeight;				//애니메이션 가중치

	bool						m_isBlending;			//
	bool						m_Temp1;
	bool						m_isOnceLoop;			//애니메이션 한번만 돌리기

	queue<eState>				combo;
	vector<int>                 d;
	float						d_1;

private:

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
	void UpdateJumpUp(iMap* pMap = NULL);			//1단점프
	void UpdateJumpDown(iMap* pMap = NULL);			//점프하강
	void UpdateJumpUpUp(iMap* pMap = NULL);			//2단점프
	void UpdateTelpo(iMap *pMap = NULL);			//텔포

													//=== GUN ===//
	void UpdateGunIdel(iMap* pMap = NULL);			//총대기상태
	void UpdateGunRunMotion(iMap* pMap = NULL);		//총들고 달리기

													//===KNIFE===//
	void UpdatefRunAndIdel(iMap* pMap = NULL);		//칼대기상태
	void Update1combo(iMap* pMap = NULL);			//칼1콤보
	void Update2combo(iMap* pMap = NULL);			//칼2콤보
	void Update3combo(iMap* pMap = NULL);			//칼3콤보

													//===HAMMOR===//
	void UpdateHammor(iMap* pMap = NULL);			//해머대기상태

	void UpdateH1combo(iMap* pMap = NULL);			//해머1콤보
	void UpdateH2combo(iMap* pMap = NULL);			//해머2콤보
	void UpdateH3combo(iMap* pMap = NULL);			//해머3콤보


	void Renderstate();


private:
	void MotionControllTower(iMap *pMap);
};

