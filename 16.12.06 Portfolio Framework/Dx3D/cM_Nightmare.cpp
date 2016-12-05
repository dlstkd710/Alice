#include "stdafx.h"
#include "cM_Nightmare.h"

cM_Nightmare::cM_Nightmare()
	:m_pSphere(NULL)
	, player_check(false)
{

}


cM_Nightmare::~cM_Nightmare()
{
}

HRESULT cM_Nightmare::Init()
{
	m_pSphere = new cBoundingSphere;
	m_pSphere->Setup(&m_Position, 10);
	player_check = false;
	return S_OK;
}

HRESULT cM_Nightmare::Init(D3DXVECTOR3 m_MtPt)
{
	m_Position = m_MtPt;
	m_pSphere = new cBoundingSphere;
	m_pSphere->Setup(&m_Position, 10);
	player_check = false;

	return S_OK;
}

void cM_Nightmare::Update()
{
	///플레이어 찾기///////
	_count++;
	if (_count % 50 == 0)
	{
		FindPlayer();
		if (player_check)
		{
			float t = D3DXVec3Length(&(m_Position - m_NowGo));
			cActionMove *_cActionMove = new cActionMove;
			_cActionMove->SetFrom(m_Position);
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




	m_pSphere->SetCenter(m_Position);
}

void cM_Nightmare::Render()
{
}

void cM_Nightmare::Destroy()
{

}

void cM_Nightmare::FindPlayer()
{
	D3DXVECTOR3 _player_position;
	float Distance = D3DXVec3Length(&(m_Position - _player_position));
	if (Distance <= 100)
	{
		if (player_check)
		{
			m_PrevGo = m_NowGo;
			m_NowGo = _player_position;
		}
		else
		{
			player_check = true;
			m_PrevGo = _player_position;
			m_NowGo = _player_position;
		}
	}
	else
	{
		player_check = false;
	}
}
