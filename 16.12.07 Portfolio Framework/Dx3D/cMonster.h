#pragma once
#include "cAnimation_M.h"
#include "cActionMove.h"
#include "cActionRepeat.h"
#include "cActionSeq.h"
class cMonster :public cGameObject
{
private:
	SYNTHESIZE(D3DXVECTOR3, m_Position, Position);
	SYNTHESIZE(D3DXVECTOR3, m_Direction, Direction);
	cBoundingSphere *m_pSphere;
	D3DXVECTOR3 m_PrevGo;
	D3DXVECTOR3 m_NowGo;
public:
	cMonster();
	~cMonster();
	virtual HRESULT Init();
	virtual HRESULT Init(D3DXVECTOR3 m_MtPt);
	virtual void Update();
	virtual void Render();
	virtual void Destroy();
	virtual void FindPlayer();

};

