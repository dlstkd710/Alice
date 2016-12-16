#include "stdafx.h"
#include "cM_Nightmare.h"

void cM_Nightmare::LeftrightChange()
{
	int _leftright = rand() % 2;
	switch (_leftright)
	{
	case 0:
		m_Stastus = LEFT;
		m_Mesh->SetAnimationIndex(0);
		break;
	case 1:
		m_Stastus = RIGHT;
		m_Mesh->SetAnimationIndex(1);
		break;
	}
	D3DXMATRIXA16 matT;
	D3DXMATRIXA16 mat_Night;
	D3DXMatrixIdentity(&matT); D3DXMatrixIdentity(&mat_Night);
	matT = m_matWorld; mat_Night = m_matWorld;
	switch (m_Stastus)
	{
	case LEFT:
		D3DXMatrixTranslation(&matT, m_CharacterPos.x, m_CharacterPos.y, m_CharacterPos.z);
		D3DXMatrixTranslation(&mat_Night, m_vDirection.x, m_vDirection.y, m_vDirection.z);
		D3DXMatrixRotationY(&mat_Night, D3DX_PI / 6.0f);
		m_PrevGo = D3DXVECTOR3(matT._41,matT._42, matT._43);
		m_NowGo = D3DXVECTOR3(mat_Night._41, mat_Night._42, mat_Night._43);
		MoveMt();
		break;
	case RIGHT:
		D3DXMatrixTranslation(&matT, 50.0f, 0.0f, 0.0f);
		D3DXMatrixTranslation(&mat_Night, 0.0f, 0.0f, 50.0f);
		D3DXMatrixRotationY(&mat_Night, D3DX_PI / 6.0f);
		m_PrevGo = D3DXVECTOR3(matT._41, matT._42, matT._43);
		m_NowGo = D3DXVECTOR3(mat_Night._41, mat_Night._42, mat_Night._43);
		MoveMt();
		break;
	}
	_countAttack = 0;
}

cM_Nightmare::cM_Nightmare()
	: m_pSphere(NULL)
	, player_check(false)
	, m_Cube(NULL)
	, m_CharacterPos(0, 0, 0)
	, m_PrevPos(0,0,0)
	, m_Mesh(NULL)
	, m_obb(NULL)
	, attack_obb(NULL)
	, m_CharacterMat(NULL)
	, ActionCheck(false)
	, _countAttack(0)
	, m_pHealth(10)
{

}


cM_Nightmare::~cM_Nightmare()
{
}

HRESULT cM_Nightmare::Init()
{
	SetPosition(D3DXVECTOR3(-10, 0, -10));
	SetDirection(D3DXVECTOR3(0, 0, -1));

	m_pSphere = new cBoundingSphere;
	m_pSphere->Setup(&m_vPosition, 5);

	D3DXMATRIXA16 matS;
	D3DXMatrixScaling(&matS, 1.5f, 3.0f, 1.5f);
	m_Cube = new cCube;
	m_Cube->Setup(&matS);
	D3DXMatrixIdentity(m_CharacterMat);

	player_check = false;
	return S_OK;
}

HRESULT cM_Nightmare::Init(D3DXVECTOR3 m_MtPt)
{
	m_vPosition = m_MtPt;
	SetDirection(D3DXVECTOR3(0, 0, -1));

	m_pSphere = new cBoundingSphere;
	m_pSphere->Setup(&m_vPosition, 5);
	player_check = false;

	D3DXMATRIXA16 matS;
	D3DXMatrixScaling(&matS, 1.5f, 3.0f, 1.5f);
	m_Cube = new cCube;
	m_Cube->Setup(&matS);

	m_Mesh = new cMtMesh("./NPC/", "그런트.X");
	m_Mesh->SetPosition(m_vPosition);
	m_Mesh->SetDirection(m_vDirection);
	m_Mesh->SetAnimationIndex(7);
	m_Mesh->SetScale(D3DXVECTOR3(0.1f, 0.1f, 0.1f));

	m_obb = new cOBB;
	m_obb->Setup(m_Mesh->GetMin(), m_Mesh->GetMax());

	D3DXVECTOR3 Minimun, maximum;
	Minimun = m_Mesh->GetMin();
	maximum = m_Mesh->GetMax();

	//Minimun.x = Minimun.x + (maximum.x - Minimun.x);
	Minimun.z = Minimun.z - (maximum.z - Minimun.z)*(3.0f/4.0f);

	//maximum.x = maximum.x + (maximum.x - m_Mesh->GetMin().x);
	maximum.z = maximum.z - (maximum.z - m_Mesh->GetMin().z)*(3.0f / 4.0f);

	attack_obb = new cOBB;
	attack_obb->Setup(Minimun, maximum);

	m_Stastus = READY;

	return S_OK;
}


void cM_Nightmare::Update(D3DXVECTOR3 _char_position, iMap* m_pMap)
{
	cGameObject::Update();
	if (m_pMap)
	{
		D3DXVECTOR3 VecTemp = m_vPosition;
		if (m_pMap->GetHeight(VecTemp.x, VecTemp.y, VecTemp.z))
		{
			m_vPosition.y = VecTemp.y;
		}
	}
	if (_char_position)
	{
		m_CharacterPos = _char_position;
	}

	///플레이어 찾기///////
	switch (m_Stastus)
	{
	case LEFT:
		_countAttack++;
		if (FindPlayer())
		{
			if (PlayerobbCollision())
			{
				ActionCheck = true;
				m_NowGo = m_vPosition;
				m_Stastus = ATTACKREADY;
				m_Mesh->SetAnimationIndex(7);
				m_PrevGo = m_CharacterPos;
				MoveMt();
			}
			else
			{
				if (_countAttack > 100)
				{
					ActionCheck = false;
					m_Stastus = FORWARD;
					m_Mesh->SetAnimationIndex(2);
					MoveMt();
				}
			}
		}
		else
		{
			m_Stastus = READY;
			m_Mesh->SetAnimationIndex(7);
			MoveMt();
		}
		break;
	case RIGHT:
		_countAttack++;
		if (FindPlayer())
		{
			if (PlayerobbCollision())
			{
				ActionCheck = true;
				m_NowGo = m_vPosition;
				m_Stastus = ATTACKREADY;
				m_Mesh->SetAnimationIndex(7);
				m_PrevGo = m_CharacterPos;
				MoveMt();
			}
			else
			{
				if (_countAttack > 100)
				{
					ActionCheck = false;
					m_Stastus = FORWARD;
					m_Mesh->SetAnimationIndex(2);
					MoveMt();
				}
			}
		}
		else
		{
			m_Stastus = READY;
			m_Mesh->SetAnimationIndex(7);
			MoveMt();
		}
		break;
	case READY:
		_countAttack = 0;
		if (FindPlayer())
		{
			ActionCheck = false;
			m_Stastus = FORWARD;
			m_Mesh->SetAnimationIndex(2);
			MoveMt();
		}
		break;
	case FORWARD:
		_countAttack = 0;
		if (FindPlayer())
		{
			if (PlayerobbCollision())
			{
				ActionCheck = true;
				m_NowGo = m_vPosition;
				m_Stastus = ATTACKREADY;
				m_Mesh->SetAnimationIndex(7);
				m_PrevGo = m_CharacterPos;
				MoveMt();
			}
			else
			{
				ActionCheck = false;
				m_NowGo = m_CharacterPos;
				MoveMt();
			}
		}
		else
		{
			m_Stastus = READY;
			m_Mesh->SetAnimationIndex(7);
			MoveMt();
		}
		break;
	case ATTACKREADY:
		if (PlayerobbCollision())
		{
			_countAttack++;
			if (_countAttack % 20 == 0)
			{
				m_NowGo = m_CharacterPos;
				m_Stastus = ATTACK_FORWARD;
				m_Mesh->SetAnimationIndex(8);
				m_PrevPos = m_vPosition;
				_countAttack = 0;
				MoveMt();
			}
		}
		else
		{
			//LeftrightChange();
			m_Stastus = FORWARD;
			m_Mesh->SetAnimationIndex(2);
			_countAttack = 0;
		}
		break;
	case ATTACK_FORWARD:
		_countAttack++;
		if (_countAttack % 120 == 0)
		{
			if (PlayerobbCollision())
			{
				m_Stastus = ATTACK_BACKWARD;
				m_NowGo = m_PrevPos;
				MoveMt();
			}
			else
			{
				//LeftrightChange();
				m_Stastus = FORWARD;
				m_Mesh->SetAnimationIndex(2);
			}
			_countAttack = 0;
		}
		break;
	case ATTACK_BACKWARD:
		_countAttack++;
		if (_countAttack % 180 == 0)
		{
			if (PlayerobbCollision())                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
			{
				m_NowGo = m_CharacterPos;
				m_Stastus = ATTACK_FORWARD;
				m_Mesh->SetAnimationIndex(8);
				m_PrevPos = m_vPosition;
				_countAttack = 0;
				MoveMt();
			}
			else
			{
				//LeftrightChange();
				m_Stastus = FORWARD;
				m_Mesh->SetAnimationIndex(2);

			}
			_countAttack = 0;
		}
		break;
	}
	//////////////////
	m_pSphere->SetCenter(m_vPosition);
	m_Cube->Update(&m_matWorld);
	if (m_Mesh)
	{
		m_Mesh->SetPosition(m_vPosition);
	}

	m_obb->Update(&m_matWorld);
	attack_obb->Update(&m_matWorld);
}

void cM_Nightmare::Render()
{
	m_pSphere->Render();
	m_Cube->Render();
	if (m_Mesh)
	{
		m_Mesh->UpdateAndRender(&m_matWorld);
	}
	m_obb->DebugRender(D3DCOLOR_XRGB(0, 0, 255));
	if (PlayerobbCollision())
	{
		attack_obb->DebugRender(D3DCOLOR_XRGB(255, 0, 0));
	}
	else
	{
		attack_obb->DebugRender(D3DCOLOR_XRGB(0, 255, 0));
	}

	D3DXVECTOR3 _CheckobbPos = m_CharacterPos;
	cOBB* Player_obb;
	Player_obb = new cOBB;
	Player_obb->Setup(D3DXVECTOR3(_CheckobbPos.x - 1.0f, _CheckobbPos.y + 5, _CheckobbPos.z - 1.0f),
					  D3DXVECTOR3(_CheckobbPos.x + 1.0f, _CheckobbPos.y + 15, _CheckobbPos.z + 1.0f));
	Player_obb->DebugRender(D3DCOLOR_XRGB(0,0,255));
}

void cM_Nightmare::Destroy()
{
	SAFE_DELETE(m_Mesh);
}

bool cM_Nightmare::FindPlayer()
{
	float Distance = D3DXVec3Length(&(m_vPosition - m_CharacterPos));
	if (Distance <= 100)
	{
		if (player_check);
		else
		{
			player_check = true;
		}
		return true;
	}
	else
	{
		player_check = false;
		return false;
	}

}

bool cM_Nightmare::PlayerobbCollision()
{
	D3DXVECTOR3 _CheckobbPos = m_CharacterPos;
	cOBB* Player_obb;
	Player_obb = new cOBB;
	Player_obb->Setup(D3DXVECTOR3(_CheckobbPos.x - 1.0f, _CheckobbPos.y + 5, _CheckobbPos.z - 1.0f),
		D3DXVECTOR3(_CheckobbPos.x + 1.0f, _CheckobbPos.y + 15, _CheckobbPos.z + 1.0f));
	if (attack_obb->IsCollision(attack_obb, Player_obb))
	{
		return true;
	}
	return false;
}

void cM_Nightmare::MoveMt()
{
	float t = D3DXVec3Length(&(m_vPosition - m_NowGo));
	cActionMove *_cActionMove = new cActionMove;
	_cActionMove->SetFrom(m_vPosition);
	_cActionMove->SetpTo(m_PrevGo);
	_cActionMove->SetTo(m_NowGo);
	switch (m_Stastus)
	{
	case FORWARD:
		_cActionMove->SetActionTime(t*0.06f);
		break;
	case LEFT:
		_cActionMove->SetActionTime(t*0.05f);
		break;
	case RIGHT:
		_cActionMove->SetActionTime(t*0.05f);
		break;
	case ATTACK_BACKWARD:
		_cActionMove->SetActionTime(t*0.04f);
		break;
	case ATTACK_FORWARD:
		_cActionMove->SetActionTime(t*0.04f);
		break;
	}
	_cActionMove->SetpStatus(m_Stastus);
	_cActionMove->SetpCount(_countAttack);
	_cActionMove->SetTarget((this));
	(this)->SetAction(_cActionMove);
}

void cM_Nightmare::AstarAlgorism()
{
}
