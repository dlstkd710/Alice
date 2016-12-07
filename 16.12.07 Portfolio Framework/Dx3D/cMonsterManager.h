#pragma once
#include "cM_Nightmare.h"

class cMonsterManager
{
private:
	typedef vector<cMonster*> vMonster;
	typedef vector<cMonster*>::iterator viMonster;
private:
	vMonster _vMonster;
	viMonster _viMonster;
public:
	cMonsterManager();
	~cMonsterManager();
	HRESULT init();
	void update();
	void render();
};

