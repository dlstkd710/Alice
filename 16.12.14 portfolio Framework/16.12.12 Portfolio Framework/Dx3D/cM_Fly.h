#pragma once
#include "cMonster.h"
class cM_Fly :public cMonster
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
private:
	D3DXVECTOR3 m_CharacterPos;
public:
	cM_Fly();
	~cM_Fly();

	HRESULT Init();
	HRESULT Init(D3DXVECTOR3 m_MtPt);
	void Update(D3DXVECTOR3 _char_position);
	void Render();
	void Destroy();
	void FindPlayer();
};

