#pragma once
#include "cAnimation_M.h"
#include "cActionMove.h"
#include "cActionRepeat.h"
#include "cActionSeq.h"
#include "cCube.h"

class cMonster :public cGameObject
{
private:
	cBoundingSphere *m_pSphere;

	//ù��° �� �����Ҷ� ����
	D3DXVECTOR3 m_PrevGo;
	cCube *m_Cube;
	//�ι�° �� �����Ҷ� ����
	D3DXVECTOR3 m_NowGo;
	bool player_check;
	unsigned int _count;
private:
	D3DXVECTOR3 m_CharacterPos;
public:
	cMonster();
	~cMonster();
	virtual HRESULT Init();
	virtual HRESULT Init(D3DXVECTOR3 m_MtPt);
	virtual void Update();
	virtual void Update(D3DXVECTOR3 _char_position);
	virtual void Render();
	virtual void Destroy();
	virtual void FindPlayer();

};

