#pragma once
#include "cMonster.h"
class cM_Nightmare :public cMonster
{
private:
	cBoundingSphere *m_pSphere;
	cMtMesh			*m_Mesh;
	//첫번째 점 보간할때 쓰임
	D3DXVECTOR3 m_PrevGo;
	D3DXVECTOR3 m_PrevPos;
	cCube *m_Cube;
	//두번째 점 보간할때 쓰임
	D3DXVECTOR3 m_NowGo;
	bool player_check;
	SYNTHESIZE(int, m_pHealth, Health);
	cOBB*		m_obb;
	cOBB*		attack_obb;

	//악몽애니메이션 왼쪽 0번, 오른쪽1번, 앞걷기 2번, 3번 뒤지기1, 
	//4번 뒤지기2, 5번 피격1, 6번 피격2, 대기7번, 공격 8번
	MtStatus	m_Stastus;
	UINT		_countAttack;
private:
	D3DXVECTOR3 m_CharacterPos;
	D3DXMATRIXA16 *m_CharacterMat;

	bool ActionCheck;

	void LeftrightChange();
public:
	cM_Nightmare();
	~cM_Nightmare();

	HRESULT Init();
	HRESULT Init(D3DXVECTOR3 m_MtPt);
	void Update(D3DXVECTOR3 _char_position, iMap* m_pMap);
	void Render();
	void Destroy();
	bool FindPlayer();
	bool PlayerobbCollision();
	void MoveMt();
	void AstarAlgorism();
};

