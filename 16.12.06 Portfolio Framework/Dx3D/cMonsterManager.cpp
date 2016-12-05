#include "stdafx.h"
#include "cMonsterManager.h"


cMonsterManager::cMonsterManager()
{
}


cMonsterManager::~cMonsterManager()
{
}

HRESULT cMonsterManager::init()
{
	for each(auto p in _vMonster)
	{
		p->Init();
	}
	return S_OK;
}

void cMonsterManager::update()
{
	for each(auto p in _vMonster)
	{
		p->Update();
	}
}

void cMonsterManager::render()
{
	for each(auto p in _vMonster)
	{
		p->Render();
	}
}
