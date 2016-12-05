#include "StdAfx.h"
#include "cCrtController.h"
#include "iMap.h"
#include "cAnimation.h"


cCrtController::cCrtController(void)
	: m_vCubePos(5, 0, 5)
	, m_vDirection1(1, 0, 0)
	, m_vCubeDir(0, 0, 1)
	, m_fCubeSpeed(0.5f)
	, m_fAngle(0.0f)
	, m_fGravity(-0.02f)
	, m_fVY(0.0f)
	, DoubleJump(0)
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

	D3DXMATRIXA16 matR, matT;



	D3DXVECTOR3 vTempPos = m_vCubePos;
	if (GetKeyState('W') & 0x8000)
	{
		vTempPos = vTempPos + m_vCubeDir * m_fCubeSpeed;
	}
	else if (GetKeyState('S') & 0x8000)
	{
		vTempPos = vTempPos - m_vCubeDir * m_fCubeSpeed;
	}
	else if (GetKeyState('S') & 0x8000)
	{

	}



	D3DXMatrixTranslation(&matT, m_vCubePos.x, m_vCubePos.y, m_vCubePos.z);

	m_matWorld = matR * matT;
	D3DXVec3TransformCoord(&m_vCubeDir, &m_vCubeDir, &matR);
}

