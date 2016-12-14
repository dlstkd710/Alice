#pragma once
#include "cAnimation_M.h"
#include "cActionMove.h"
#include "cActionRepeat.h"
#include "cActionSeq.h"
#include "cCube.h"
#include "cMtMesh.h"
#include "iMap.h"

class cMonster :public cGameObject
{
private:
	cBoundingSphere *m_pSphere;
	cMtMesh			*m_Mesh;
	//첫번째 점 보간할때 쓰임
	D3DXVECTOR3 m_PrevGo;
	cCube *m_Cube;
	//두번째 점 보간할때 쓰임
	D3DXVECTOR3 m_NowGo;
	bool player_check;
	SYNTHESIZE(int, m_pHealth, Health);
	cOBB*		m_obb;
private:
	D3DXVECTOR3 m_CharacterPos;
public:
	cMonster();
	~cMonster();
	virtual HRESULT Init();
	virtual HRESULT Init(D3DXVECTOR3 m_MtPt);
	virtual void Update(D3DXVECTOR3 _char_position, iMap* m_pMap);
	virtual void Render();
	virtual void Destroy();
	virtual bool FindPlayer();
	virtual void MoveMt();

};

