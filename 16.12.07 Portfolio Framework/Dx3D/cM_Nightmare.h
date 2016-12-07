#pragma once
#include "cMonster.h"
class cM_Nightmare :public cMonster
{
private:
	SYNTHESIZE(D3DXVECTOR3, m_Position, Position);
	SYNTHESIZE(D3DXVECTOR3, m_Direction, Direction);
	cBoundingSphere *m_pSphere;

	//ù��° �� �����Ҷ� ����
	D3DXVECTOR3 m_PrevGo;

	//�ι�° �� �����Ҷ� ����
	D3DXVECTOR3 m_NowGo;
	bool player_check;
	unsigned int _count;
public:
	cM_Nightmare();
	~cM_Nightmare();

	HRESULT Init();
	HRESULT Init(D3DXVECTOR3 m_MtPt);
	void Update();
	void Render();
	void Destroy();
	void FindPlayer();
};

