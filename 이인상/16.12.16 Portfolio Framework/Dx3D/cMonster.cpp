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


void cMonster::Update(D3DXVECTOR3 _char_position)
{
	cGameObject::Update();
}

void cMonster::Render()
{
}

void cMonster::Destroy()
{
}

void cMonster::FindPlayer()
{
}
