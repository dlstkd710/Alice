#include "stdafx.h"
#include "cM_Nightmare.h"

cM_Nightmare::cM_Nightmare()
	: m_pSphere(NULL)
	, player_check(false)
	, m_Cube(NULL)
	, m_CharacterPos(0, 0, 0)
	, m_Mesh(NULL)
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

	m_Mesh = new cMtMesh;
	m_Mesh->Load("./NPC/.", "그런트.X");

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

	//m_Object = new cObjectXfile;
	//m_Object->Load("./Object/", "rock.X");
	//m_Object->SetPosition(D3DXVECTOR3(0, 0, 0));
	//m_Object->SetScale(D3DXVECTOR3(0.01f, 0.01f, 0.01f));
	m_Mesh = new cMtMesh;
	m_Mesh->Load("./NPC/.", "그런트.X");
	m_Mesh->SetPosition(m_vPosition);
	m_Mesh->SetDirection(m_vDirection);
	//m_Mesh->SetAnimationIndex(rand() % 5);
	m_Mesh->SetScale(D3DXVECTOR3(0.1f, 0.1f, 0.1f));

	return S_OK;
}

void cM_Nightmare::Update()
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
	if (m_Mesh)
	{
		m_Mesh->Update();
	}



	m_pSphere->SetCenter(m_vPosition);
	m_Cube->Update(&m_matWorld);
}

void cM_Nightmare::Update(D3DXVECTOR3 _char_position)
{
	cGameObject::Update();
	if (m_Mesh)
	{
		m_Mesh->SetPosition(m_vPosition);
		m_Mesh->Update();
	}
	if (_char_position)
		m_CharacterPos = _char_position;
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
				m_Mesh->SetAction(_cActionMove);
			}
		}
	}
	if (_count >= INT16_MAX)
		_count = 0;
	//////////////////


	D3DXMATRIXA16 WorldMat;
	D3DXMatrixIdentity(&WorldMat);
	//D3DXMatrixTransformation(&WorldMat,m_vPosition,)
	m_pSphere->SetCenter(m_vPosition);
	m_Cube->Update(&m_matWorld);
}

void cM_Nightmare::Render()
{
	m_pSphere->Render();
	m_Cube->Render();
	m_Mesh->Render();
}

void cM_Nightmare::Destroy()
{
	SAFE_DELETE(m_Mesh);
}

void cM_Nightmare::FindPlayer()
{
	D3DXVECTOR3 _player_position = m_CharacterPos;
	float Distance = D3DXVec3Length(&(m_vPosition - _player_position));
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
