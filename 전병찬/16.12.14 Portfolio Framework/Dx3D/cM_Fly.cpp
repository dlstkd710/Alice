#include "stdafx.h"
#include "cM_Fly.h"

cM_Fly::cM_Fly()
	: m_pSphere(NULL)
	, player_check(false)
	, m_Cube(NULL)
	, m_CharacterPos(0, 0, 0)
	, m_Mesh(NULL)
	, m_pHealth(5)
	, m_obb(NULL)
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
	m_Mesh = new cMtMesh("./NPC/", "볼트플라이.X");
	m_Mesh->SetPosition(m_vPosition);
	m_Mesh->SetDirection(m_vDirection);

	///모기애니메이션인덱스 0번 공격, 1번 이동
	m_Mesh->SetAnimationIndex(1);
	m_Mesh->SetScale(D3DXVECTOR3(0.1f, 0.1f, 0.1f));

	m_obb = new cOBB;
	m_obb->Setup(m_Mesh->GetMin(), m_Mesh->GetMax());
	return S_OK;
}

void cM_Fly::Update(D3DXVECTOR3 _char_position, iMap* m_pMap)
{
	cGameObject::Update();
	if (m_Mesh)
	{
		m_Mesh->SetPosition(m_vPosition);
	}
	if (_char_position)
	{
		m_CharacterPos = _char_position;
	}
	///플레이어 찾기///////
	if (FindPlayer())
		MoveMt();
	//////////////////
	m_pSphere->SetCenter(m_vPosition);
	m_Cube->Update(&m_matWorld);

	m_obb->Update(&m_matWorld);
}

void cM_Fly::Render()
{
	m_pSphere->Render();
	m_Cube->Render();
	if (m_Mesh)
	{
		m_Mesh->UpdateAndRender(&m_matWorld);
	}
	m_obb->DebugRender(D3DCOLOR_XRGB(0, 0, 255));
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
		if (player_check)
		{
			m_PrevGo = m_NowGo;
			m_NowGo = m_CharacterPos;
		}
		else
		{
			player_check = true;
			m_PrevGo = m_vPosition;
			m_NowGo = m_CharacterPos;
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
	if (m_NowGo != m_vPosition)
	{
		float t = D3DXVec3Length(&(m_vPosition - m_NowGo));
		cActionMove *_cActionMove = new cActionMove;
		_cActionMove->SetFrom(m_vPosition);
		_cActionMove->SetpTo(m_PrevGo);
		_cActionMove->SetTo(m_NowGo);
		_cActionMove->SetActionTime(t*0.08f);
		_cActionMove->SetTarget((this));
		(this)->SetAction(_cActionMove);
		m_Mesh->SetAction(_cActionMove);
	}
}