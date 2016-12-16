#include "stdafx.h"
#include "cMonsterManager.h"
#include "CharacterClass.h"
#include "iMap.h"

cMonsterManager::cMonsterManager()
	:_vMonster(NULL)
{
}


cMonsterManager::~cMonsterManager()
{
}

HRESULT cMonsterManager::init()
{
	return S_OK;
}

HRESULT cMonsterManager::init(MonsterNum MtNum, D3DXVECTOR3 MtPos)
{
	switch (MtNum)
	{
	case 0:
		cMonster *_Nightmare;
		_Nightmare = new cM_Nightmare;
		_Nightmare->Init(MtPos);
		_vMonster.push_back(_Nightmare);
		break;
	case 1:
		cMonster *_Fly;
		_Fly = new cM_Fly;
		_Fly->Init(MtPos);
		_vMonster.push_back(_Fly);
		break;
	case 2:
		break;
	}
	return S_OK;
}

void cMonsterManager::update()
{
	
}

void cMonsterManager::update(iMap * m_pMap)
{
	for each(auto p in _vMonster)
	{
		p->Update(_char->GetPosition(), m_pMap);
	}
}

void cMonsterManager::render()
{
	for each(auto p in _vMonster)
	{
		p->Render();
	}
}
