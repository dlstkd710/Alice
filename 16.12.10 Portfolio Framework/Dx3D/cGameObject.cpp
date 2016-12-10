#include "StdAfx.h"
#include "cGameObject.h"


cGameObject::cGameObject(void)
	: m_vPosition(0, 0, 0)
	, m_vDirection(0, 0, 1)
	, m_pAction(NULL)
	, m_vPrevPosition(0, 0, 0)
	, m_vScale(0.1f, 0.1f, 0.1f)
	, m_vRotateX(0.0f)
	, m_vRotateY(0.0f)
	, m_vRotateZ(0.0f)
{
	D3DXMatrixIdentity(&m_matWorld);
}


cGameObject::~cGameObject(void)
{
	SAFE_RELEASE(m_pAction);
}

void cGameObject::Update()
{
	if (m_pAction)
		m_pAction->Update();

	//m_vDirection = m_vPosition - m_vPrevPosition;

	D3DXMATRIXA16 matS, matR, matT;
	D3DXMatrixLookAtLH(&matR,
		&D3DXVECTOR3(0, 0, 0),
		&m_vDirection,
		&D3DXVECTOR3(0, 1, 0));
	D3DXMatrixScaling(&matS, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranspose(&matR, &matR);
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	m_matWorld = matS * matR;
	m_matWorld = m_matWorld* matT;
	//m_vPrevPosition = m_vPosition;
}

void cGameObject::Render()
{

}
