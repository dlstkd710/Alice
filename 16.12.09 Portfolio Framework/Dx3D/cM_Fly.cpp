#include "stdafx.h"
#include "cM_Fly.h"

cM_Fly::cM_Fly()
	: m_pSphere(NULL)
	, player_check(false)
	, m_Cube(NULL)
	, m_CharacterPos(0, 0, 0)
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
	D3DXMatrixScaling(&matS, 0.5f, 1.5f, 0.5f);
	m_Cube = new cCube;
	m_Cube->Setup(&matS);

	player_check = false;
	return S_OK;
}

HRESULT cM_Fly::Init(D3DXVECTOR3 m_MtPt)
{
	m_vPosition = m_MtPt;
	SetDirection(D3DXVECTOR3(0, 0, -1));

	m_pSphere = new cBoundingSphere;
	m_pSphere->Setup(&m_vPosition, 5);
	player_check = false;

	D3DXMATRIXA16 matS;
	D3DXMatrixScaling(&matS, 0.5f, 0.5f, 1.5f);
	m_Cube = new cCube;
	m_Cube->Setup(&matS);

	return S_OK;
}

void cM_Fly::Update()
{
	cGameObject::Update();
	///플레이어 찾기///////
	_count++;
	if (_count % 50 == 0)
	{
		FindPlayer();
		if (player_check)
		{
			float t = D3DXVec3Length(&(m_vPosition - m_NowGo));
			cActionMove *_cActionMove = new cActionMove;
			_cActionMove->SetFrom(m_vPosition);
			_cActionMove->SetpTo(m_PrevGo);
			_cActionMove->SetTo(m_NowGo);
			_cActionMove->SetActionTime(t*0.1);
			_cActionMove->SetTarget((this));

			(this)->SetAction(_cActionMove);
		}
	}
	if (_count >= INT16_MAX)
		_count = 0;
	//////////////////




	m_pSphere->SetCenter(m_vPosition);
	m_Cube->Update(&m_matWorld);
}

void cM_Fly::Update(D3DXVECTOR3 _char_position)
{
	cGameObject::Update();
	if (_char_position)
	{
		m_CharacterPos = _char_position;
		m_CharacterPos.y = _char_position.y + 15;
	}
	///플레이어 찾기///////
	_count++;
	if (_count % 30 == 0)
	{
		FindPlayer();
		if (player_check)
		{
			float Distance = D3DXVec3Length(&(m_PrevGo - m_NowGo));
			if (fabsf(Distance) >= 5.0f)
			{
				float t = D3DXVec3Length(&(m_vPosition - m_NowGo));
				cActionMove *_cActionMove = new cActionMove;
				_cActionMove->SetFrom(m_vPosition);
				_cActionMove->SetpTo(m_PrevGo);
				_cActionMove->SetTo(m_NowGo);
				_cActionMove->SetActionTime(t*0.1);
				_cActionMove->SetTarget((this));
				(this)->SetAction(_cActionMove);
			}
		}
	}
	if (_count >= INT16_MAX)
		_count = 0;
	//////////////////




	m_pSphere->SetCenter(m_vPosition);
	m_Cube->Update(&m_matWorld);
}

void cM_Fly::Render()
{
	m_pSphere->Render();
	m_Cube->Render();
}

void cM_Fly::Destroy()
{

}

void cM_Fly::FindPlayer()
{
	D3DXVECTOR3 _player_position = m_CharacterPos;
	_player_position.y = m_CharacterPos.y;
	float Distance = D3DXVec3Length(&(m_vPosition - m_CharacterPos));
	if (Distance <= 100)
	{
		if (player_check)
		{
			m_PrevGo = m_NowGo;
			m_NowGo = m_CharacterPos;
		}
		else
		{
			player_check = true;
			m_PrevGo = m_CharacterPos;
			m_NowGo = m_CharacterPos;
		}
	}
	else
	{
		player_check = false;
	}
	//if (player_check)
	//{
	//	m_PrevGo = m_NowGo;
	//	m_NowGo = _player_position;
	//}
	//else
	//{
	//	player_check = true;
	//	m_PrevGo = _player_position;
	//	m_NowGo = _player_position;
	//
	//	float t = D3DXVec3Length(&(m_vPosition - m_NowGo));
	//	cActionMove *_cActionMove = new cActionMove;
	//	_cActionMove->SetFrom(m_vPosition);
	//	_cActionMove->SetpTo(m_PrevGo);
	//	_cActionMove->SetTo(m_NowGo);
	//	_cActionMove->SetActionTime(t*0.1);
	//	_cActionMove->SetTarget((this));
	//
	//	(this)->SetAction(_cActionMove);
	//}
}
