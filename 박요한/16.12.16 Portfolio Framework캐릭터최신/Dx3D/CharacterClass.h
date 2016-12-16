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

	cAnimation*					_Animation;			//������ ĳ����
	cAnimation*					_Knife;				//weapon Į
	cAnimation*					_Gun;				//weapon ��
	cAnimation*					_Hammor;			//weapon �ظ�
	EffectClass*				m_pEffectClass;		//Effect Class 


	int							n, a, t, r, i, c, combo1, combo3;


	D3DXMATRIXA16				matR;

	eState						m_eState;

	float						m_fGravity;				//������ �߷°�
	float						m_fVY;					//������
	float						m_fSpeed;				//ĳ���� �̵��ӵ�
	float						m_fCubeSpeed;			//�̵��ӵ�
	float						m_fBlendTime;			//�ִϸ��̼� ����Ÿ��
	float						m_fPassedBlendTime;		//�ִϸ��̼� �����Ⱓ
	float						m_fWeight;				//�ִϸ��̼� ����ġ

	bool						m_isBlending;			//
	bool						m_Temp1;
	bool						m_isOnceLoop;			//�ִϸ��̼� �ѹ��� ������

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
	void UpdateJumpUp(iMap* pMap = NULL);			//1������
	void UpdateJumpDown(iMap* pMap = NULL);			//�����ϰ�
	void UpdateJumpUpUp(iMap* pMap = NULL);			//2������
	void UpdateTelpo(iMap *pMap = NULL);			//����

													//=== GUN ===//
	void UpdateGunIdel(iMap* pMap = NULL);			//�Ѵ�����
	void UpdateGunRunMotion(iMap* pMap = NULL);		//�ѵ�� �޸���

													//===KNIFE===//
	void UpdatefRunAndIdel(iMap* pMap = NULL);		//Į������
	void Update1combo(iMap* pMap = NULL);			//Į1�޺�
	void Update2combo(iMap* pMap = NULL);			//Į2�޺�
	void Update3combo(iMap* pMap = NULL);			//Į3�޺�

													//===HAMMOR===//
	void UpdateHammor(iMap* pMap = NULL);			//�ظӴ�����

	void UpdateH1combo(iMap* pMap = NULL);			//�ظ�1�޺�
	void UpdateH2combo(iMap* pMap = NULL);			//�ظ�2�޺�
	void UpdateH3combo(iMap* pMap = NULL);			//�ظ�3�޺�


	void Renderstate();


private:
	void MotionControllTower(iMap *pMap);
};

