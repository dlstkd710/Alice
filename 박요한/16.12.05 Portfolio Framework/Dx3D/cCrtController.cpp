#include "StdAfx.h"
#include "cCrtController.h"
#include "iMap.h"

#define DELTA_Y 0.1f

cCrtController::cCrtController(void)
	: m_vCubePos(0, 0, 0)
	, m_vCubeDir(0, 0, 1)
	, m_fCubeSpeed(0.1f)
	, m_fAngle(0.0f)
{
}


cCrtController::~cCrtController(void)
{
}

void cCrtController::Setup()
{

}

void cCrtController::Update(iMap* pMap /*= NULL*/)
{
	if (g_pkeyManager->isStayKeyDown('A'))
	{
		m_fAngle -= 0.1f;
	}
	if (g_pkeyManager->isStayKeyDown('D'))
	{
		m_fAngle += 0.1f;
	}

	D3DXMATRIXA16 matR, matT;
	D3DXMatrixRotationY(&matR, m_fAngle);
	D3DXMatrixTranslation(&matT, m_vCubePos.x, m_vCubePos.y, m_vCubePos.z);

	m_matWorld = matR * matT;

	m_vCubeDir = D3DXVECTOR3(0, 0, -1);
	D3DXVec3TransformCoord(&m_vCubeDir, &m_vCubeDir, &matR);
}
