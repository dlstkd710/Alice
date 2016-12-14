#pragma once
#include "cM_Nightmare.h"
#include "cM_Fly.h"
class CharacterClass;

using namespace std;
class cMonsterManager
{
private:
	typedef vector<cMonster*> vMonster;
	typedef vector<cMonster*>::iterator viMonster;
private:
	vMonster _vMonster;
	viMonster _viMonster;

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

