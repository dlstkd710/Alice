#include "stdafx.h"
#include "cM_Fly.h"

cM_Fly::cM_Fly()
	: m_pSphere(NULL)
	, player_check(false)
	, m_Cube(NULL)
	, m_CharacterPos(0, 0, 0)
	, m_PrevPos(0,0,0)
	, m_Mesh(NULL)
	, m_pHealth(5)
	, m_obb(NULL)
	, attack_obb(NULL)
	, _countAttack(0)
{

}


cM_Fly::~cM_Fly()
{
}

HRESULT cM_Fly::Init()
{
	SetPosition(D3DXVECTOR3(-10, 0, -10));
	SetDirection(D3DXVECTOR3(0, 0, -1));

	m_pSphere = new cBoundingSphere;
	m_pSphere->Setup(&m_vPosition, 5);

	D3DXMATRIXA16 matS;
	D3DXMatrixScaling(&matS, 1.5f, 3.0f, 1.5f);
	m_Cube = new cCube;
	m_Cube->Setup(&matS);

	player_check = false;
	return S_OK;
}

HRESULT cM_Fly::Init(D3DXVECTOR3 m_MtPt)
{
	m_vPosition = m_MtPt;
	SetDirection(D3DXVECTOR3(0, 0, 1));

	m_pSphere = new cBoundingSphere;
	m_pSphere->Setup(&m_vPosition, 5);
	player_check = false;

	D3DXMATRIXA16 matS;
	D3DXMatrixScaling(&matS, 1.5f, 3.0f, 1.5f);
	m_Cube = new cCube;
	m_Cube->Setup(&matS);

	//m_Object = new cObjectXfile;
	//m_Object->Load("./Object/", "rock.X");
	//m_Object->SetPosition(D3DXVECTOR3(0, 0, 0));
	//m_Object->SetScale(D3DXVECTOR3(0.01f, 0.01f, 0.01f));
	m_Mesh = new cMtMesh("./NPC/", "볼트플라이.X");
	m_Mesh->SetPosition(m_vPosition);
	m_Mesh->SetDirection(m_vDirection);

	///모기애니메이션인덱스 0번 공격, 1번 이동
	m_Mesh->SetAnimationIndex(1);
	m_Mesh->SetScale(D3DXVECTOR3(0.1f, 0.1f, 0.1f));

	m_obb = new cOBB;
	m_obb->Setup(m_Mesh->GetMin(), m_Mesh->GetMax());

	D3DXVECTOR3 Minimun, maximum;
	Minimun = m_Mesh->GetMin();
	maximum = m_Mesh->GetMax();

	//Minimun.x = Minimun.x + (maximum.x - Minimun.x);
	Minimun.z = Minimun.z - (maximum.z - Minimun.z)*(3.0f / 4.0f);

	//maximum.x = maximum.x + (maximum.x - m_Mesh->GetMin().x);
	maximum.z = maximum.z - (maximum.z - m_Mesh->GetMin().z)*(3.0f / 4.0f);

	attack_obb = new cOBB;
	attack_obb->Setup(Minimun, maximum);

	m_Stastus = READY;

	return S_OK;
}

void cM_Fly::Update(D3DXVECTOR3 _char_position, iMap* m_pMap)
{
	cGameObject::Update();
	if (m_pMap)
	{
		D3DXVECTOR3 VecTemp = m_vPosition;
		if (m_pMap->GetHeight(VecTemp.x, VecTemp.y, VecTemp.z))
		{
			m_vPosition.y = VecTemp.y+10;
		}
	}

	if (_char_position)
	{
		m_CharacterPos = _char_position;
	}
	///플레이어 찾기///////
	switch (m_Stastus)
	{
	case READY:
		_countAttack = 0;
		if (FindPlayer())
		{
			m_Stastus = FORWARD;
			m_Mesh->SetAnimationIndex(1);
			MoveMt();
		}
		break;
	case FORWARD:
		_countAttack = 0;
		if (FindPlayer())
		{
			if (PlayerobbCollision())
			{
				m_NowGo = m_vPosition;
				m_Stastus = ATTACKREADY;
				m_Mesh->SetAnimationIndex(0);
				m_PrevGo = m_CharacterPos;
				MoveMt();
			}
			else
			{
				m_NowGo = m_CharacterPos;
				m_NowGo.y = m_CharacterPos.y + 15;
				MoveMt();
			}
		}
		else
		{
			m_Stastus = READY;
			m_Mesh->SetAnimationIndex(0);
			MoveMt();
		}
		break;
	case ATTACKREADY:
		if (PlayerobbCollision())
		{
			_countAttack++;
			if (_countAttack % 40 == 0)
			{
				m_NowGo = m_CharacterPos;
				m_NowGo.y = m_CharacterPos.y + 15;
				m_Stastus = ATTACK_FORWARD;
				m_Mesh->SetAnimationIndex(0);
				m_PrevPos = m_vPosition;
				_countAttack = 0;
				MoveMt();
			}
		}
		else
		{
			m_Stastus = FORWARD;
			m_Mesh->SetAnimationIndex(1);
			_countAttack = 0;
		}
		break;
	case ATTACK_FORWARD:
		_countAttack++;
		if (_countAttack % 120 == 0)
		{
			if (PlayerobbCollision())
			{
				m_Stastus = ATTACK_BLEED;
				m_NowGo = m_PrevPos;
				MoveMt();
			}
			else
			{
				m_Stastus = FORWARD;
				m_Mesh->SetAnimationIndex(1);
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
}

void cM_Fly::Render()
{
	g_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000000);
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
	//cOBB* Player_obb;
	//Player_obb = new cOBB;
	//Player_obb->Setup(D3DXVECTOR3(_CheckobbPos.x - 1.0f, _CheckobbPos.y + 5, _CheckobbPos.z - 1.0f),
	//	D3DXVECTOR3(_CheckobbPos.x + 1.0f, _CheckobbPos.y + 15, _CheckobbPos.z + 1.0f));
	//Player_obb->DebugRender(D3DCOLOR_XRGB(0, 0, 255));
}

void cM_Fly::Destroy()
{
	SAFE_DELETE(m_Mesh);
}

bool cM_Fly::FindPlayer()
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

void cM_Fly::MoveMt()
{
	float t = D3DXVec3Length(&(m_vPosition - m_NowGo));
	cActionMove *_cActionMove = new cActionMove;
	_cActionMove->SetFrom(m_vPosition);
	_cActionMove->SetpTo(m_PrevGo);
	_cActionMove->SetTo(m_NowGo);
	switch (m_Stastus)
	{
	case FORWARD:
		_cActionMove->SetActionTime(t*0.08f);
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

bool cM_Fly::PlayerobbCollision()
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
