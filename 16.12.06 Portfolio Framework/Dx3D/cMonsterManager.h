#pragma once
#include "cM_Nightmare.h"

class CharacterClass;

using namespace std;
class cMonsterManager
{
private:
	typedef map<int, cMonster*> m_Monster;
	typedef map<int, cMonster*>::iterator mi_Monster;
	typedef vector<cMonster*> vMonster;
	typedef vector<cMonster*>::iterator viMonster;
private:
	vMonster _vMonster;
	viMonster _viMonster;
	m_Monster _mMonster;
	mi_Monster _miMonster;
private:
	CharacterClass* _char;
public:
	cMonsterManager();
	~cMonsterManager();
	HRESULT init();
	HRESULT init(MonsterNum MtNum, D3DXVECTOR3 MtPos);
	void charLink(CharacterClass* Char) { _char = Char; }
	void update();
	void render();
};

