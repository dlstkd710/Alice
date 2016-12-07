#include "StdAfx.h"
#include "cCrtController.h"
#include "iMap.h"

#define DELTA_Y 0.1f

cCrtController::cCrtController(void)
	: m_vCubePos(5, 0, 5)
	, m_vDirection1(0, 1, 0)
	, m_vCubeDir(0, 0, 1)
	, m_fCubeSpeed(0.5f)
	, m_fAngle(0.0f)
	, m_eState(E_STATE_NORMAL)
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
	if (g_pkeyManager->isStayKeyDown('A'))
	{
		m_vCubePos.x -= 0.5f;
	}
	if (g_pkeyManager->isStayKeyDown('D'))
	{
		m_vCubePos.x += 0.5f;
	}
	switch (m_eState)
	{
	case E_STATE_NORMAL:
		UpdateNormal(pMap);
		break;
	case E_STATE_JUMPUP:
		UpdateJumpUp(pMap);
		break;
	case E_STATE_JUMPDOUBLE:
		UpdateJumpUpUp(pMap);
		break;
	case E_STATE_JUMPDOWN:
		UpdateJumpDown(pMap);
		break;
	}

	D3DXMATRIXA16 matR, matT;
	D3DXMatrixRotationY(&matR, m_fAngle);
	D3DXMatrixTranslation(&matT, m_vCubePos.x, m_vCubePos.y, m_vCubePos.z);

	m_matWorld = matR * matT;

	m_vCubeDir = D3DXVECTOR3(0, 0, -1);
	D3DXVec3TransformCoord(&m_vCubeDir, &m_vCubeDir, &matR);
}


void cCrtController::UpdateNormal(iMap* pMap /*= NULL*/)
{

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		m_fVY = 0.5f;	//점프 높이
		m_eState = E_STATE_JUMPUP;
		DoubleJump = 1;
		return;
	}
	D3DXVECTOR3 vTempPos = m_vCubePos;
	if (g_pkeyManager->isStayKeyDown('W'))
	{
		vTempPos = vTempPos - m_vCubeDir * m_fCubeSpeed;
	}
	if (g_pkeyManager->isStayKeyDown('S'))
	{
		vTempPos = vTempPos + m_vCubeDir * m_fCubeSpeed;
	}

	if (pMap && pMap->GetHeight(vTempPos.x, vTempPos.y, vTempPos.z))
	{
		if (vTempPos.y - m_vCubePos.y > DELTA_Y) // 가려는 곳이 많이 높다
		{
			m_vCubePos = vTempPos;
		}
		else if (m_vCubePos.y - vTempPos.y > DELTA_Y) // 가려는 곳이 많이 낮다
		{
			m_fVY = 0.0f;
			m_eState = E_STATE_JUMPDOWN;
			return;
		}
		else
		{
			m_vCubePos = vTempPos;
		}
	}

	if (pMap == NULL)
	{
		m_vCubePos = vTempPos;
	}
}

void cCrtController::UpdateJumpUp(iMap* pMap /*= NULL*/)
{

	if (GetAsyncKeyState(VK_SPACE) & 1)
	{
		m_fVY = 0.5f;	//점프 높이
		m_eState = E_STATE_JUMPDOUBLE;
		return;
	}
	if (m_fVY < 0)
	{
		m_eState = E_STATE_JUMPDOWN;
		return;
	}
	m_fVY += m_fGravity;

	D3DXVECTOR3 vTempPos = m_vCubePos;
	if (GetKeyState('W') & 0x8000)
	{
		vTempPos = vTempPos + m_vCubeDir * m_fCubeSpeed;
	}

	if (GetKeyState('S') & 0x8000)
	{
		vTempPos = vTempPos - m_vCubeDir * m_fCubeSpeed;
	}

	if (pMap == NULL)
	{
		m_vCubePos = vTempPos;
		return;
	}

	if (pMap->GetHeight(vTempPos.x, vTempPos.y, vTempPos.z))
	{
		if (vTempPos.y < m_vCubePos.y)
		{
			m_vCubePos.x = vTempPos.x;
			m_vCubePos.z = vTempPos.z;
		}
	}

	m_vCubePos.y += m_fVY;
}


void cCrtController::UpdateJumpDown(iMap* pMap /*= NULL*/)
{
	if (GetAsyncKeyState(VK_SPACE) & 0x8001)
	{
		m_fVY -= -0.018f;
	//	, m_fGravity(-0.02f)
	}

	m_fVY += m_fGravity;

	D3DXVECTOR3 vTempPos = m_vCubePos;
	if (GetKeyState('W') & 0x8000)
	{
		vTempPos = vTempPos + m_vCubeDir * m_fCubeSpeed;
	}

	if (GetKeyState('S') & 0x8000)
	{
		vTempPos = vTempPos - m_vCubeDir * m_fCubeSpeed;
	}

	if (pMap == NULL)
	{
		m_vCubePos = vTempPos;
		return;
	}

	if (pMap->GetHeight(vTempPos.x, vTempPos.y, vTempPos.z))
	{
		if (m_vCubePos.y > vTempPos.y) // 가려는 곳이 더 낮은경우
		{
			m_vCubePos.x = vTempPos.x;
			m_vCubePos.y += m_fVY;
			m_vCubePos.z = vTempPos.z;

			if (m_vCubePos.y < vTempPos.y)
			{
				m_eState = E_STATE_NORMAL;
				m_vCubePos.y = vTempPos.y;
			}
			return;
		}
		else
		{
			m_vCubePos.y += m_fVY;

			pMap->GetHeight(m_vCubePos.x, vTempPos.y, m_vCubePos.z);

			if (m_vCubePos.y < vTempPos.y)
			{
				m_eState = E_STATE_NORMAL;
				m_vCubePos.y = vTempPos.y;
			}
			return;
		}
	}
	else
	{
		pMap->GetHeight(m_vCubePos.x, vTempPos.y, m_vCubePos.z);
		m_vCubePos.y += m_fVY;

		if (m_vCubePos.y < vTempPos.y)
		{
			m_eState = E_STATE_NORMAL;
			m_vCubePos.y = vTempPos.y;
			return;
		}
	}
}

void cCrtController::UpdateJumpUpUp(iMap * pMap)
{
	if (m_fVY < 0)
	{
		m_eState = E_STATE_JUMPDOWN;
		return;
	}

	m_fVY += m_fGravity;

	D3DXVECTOR3 vTempPos = m_vCubePos;
	if (GetKeyState('W') & 0x8000)
	{
		vTempPos = vTempPos + m_vCubeDir * m_fCubeSpeed;
	}

	if (GetKeyState('S') & 0x8000)
	{
		vTempPos = vTempPos - m_vCubeDir * m_fCubeSpeed;
	}

	if (pMap == NULL)
	{
		m_vCubePos = vTempPos;
		return;
	}

	if (pMap->GetHeight(vTempPos.x, vTempPos.y, vTempPos.z))
	{
		if (vTempPos.y < m_vCubePos.y)
		{
			m_vCubePos.x = vTempPos.x;
			m_vCubePos.z = vTempPos.z;
		}
	}

	m_vCubePos.y += m_fVY;
}
