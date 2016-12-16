#pragma once
#include "cMonster.h"
class cM_Nightmare :public cMonster
{
private:
	cBoundingSphere *m_pSphere;
	cMtMesh			*m_Mesh;
	//ù��° �� �����Ҷ� ����
	D3DXVECTOR3 m_PrevGo;
	D3DXVECTOR3 m_PrevPos;
	cCube *m_Cube;
	//�ι�° �� �����Ҷ� ����
	D3DXVECTOR3 m_NowGo;
	bool player_check;
	SYNTHESIZE(int, m_pHealth, Health);
	cOBB*		m_obb;
	cOBB*		attack_obb;

	//�Ǹ��ִϸ��̼� ���� 0��, ������1��, �հȱ� 2��, 3�� ������1, 
	//4�� ������2, 5�� �ǰ�1, 6�� �ǰ�2, ���7��, ���� 8��
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

