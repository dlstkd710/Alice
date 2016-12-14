#include "stdafx.h"
#include "cMonster.h"


cMonster::cMonster()
{
}


cMonster::~cMonster()
{
}

HRESULT cMonster::Init()
{
	return S_OK;
}

HRESULT cMonster::Init(D3DXVECTOR3 m_MtPt)
{
	return S_OK;
}


void cMonster::Update(D3DXVECTOR3 _char_position, iMap* m_pMap)
{
	cGameObject::Update();
}

void cMonster::Render()
{
}

void cMonster::Destroy()
{
}

bool cMonster::FindPlayer()
{
	return false;
}

void cMonster::MoveMt()
{
}
