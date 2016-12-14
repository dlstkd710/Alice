#pragma once
#include "cMonster.h"
class cM_Nightmare :public cMonster
{
private:
	cBoundingSphere *m_pSphere;
	cMtMesh			*m_Mesh;
	//ù��° �� �����Ҷ� ����
	D3DXVECTOR3 m_PrevGo;
	cCube *m_Cube;
	//�ι�° �� �����Ҷ� ����
	D3DXVECTOR3 m_NowGo;
	bool player_check;
	SYNTHESIZE(int, m_pHealth, Health);
private:
	D3DXVECTOR3 m_CharacterPos;
public:
	cM_Nightmare();
	~cM_Nightmare();

	HRESULT Init();
	HRESULT Init(D3DXVECTOR3 m_MtPt);
	void Update(D3DXVECTOR3 _char_position);
	void Render();
	void Destroy();
	void FindPlayer();
};

