#include "StdAfx.h"
#include "cActionMove.h"


cActionMove::cActionMove(void)
	: m_vFrom(0, 0, 0)
	, m_vTo(0, 0, 0)
	, m_pTo(0, 0, 0)
	, m_Check(false)
	, m_Count(0)
{
}

cActionMove::~cActionMove(void)
{
}

void cActionMove::Start()
{
	m_fPassedActionTime = 0.0f;
	if (m_pTarget)
	{
		m_pTarget->SetPosition(m_vFrom);
	}
}

void cActionMove::Update()
{
	float t = 0.0f;
	D3DXVECTOR3 p(0, 0, 0);
	D3DXVECTOR3 p1(0, 0, 0);
	D3DXVECTOR3 p2(0, 0, 0);
	switch (m_Status)
	{
	case FORWARD:
		m_fPassedActionTime += g_pTimeManager->GetDeltaTime();
		t = m_fPassedActionTime / m_fActionTime;

		if (t > 1.0f)
		{
			t = 1.0f;
			if (m_pDelegate)
			{
				m_pDelegate->OnActionFinish(this);
			}
		}

		D3DXVec3Lerp(&p, &m_vFrom, &m_vTo, t);
		if (m_pTarget)
		{
			if (m_vTo != p)
				m_pTarget->SetPosition(p);
			m_pTarget->SetDirection(m_vFrom - m_vTo);
		}
		break;
	case LEFT:
		m_fPassedActionTime += g_pTimeManager->GetDeltaTime();
		t = m_fPassedActionTime / m_fActionTime;

		if (t > 1.0f)
		{
			t = 1.0f;
			if (m_pDelegate)
			{
				m_pDelegate->OnActionFinish(this);
			}
		}

		D3DXVec3Lerp(&p, &m_vFrom, &m_vTo, t);
		D3DXVec3Lerp(&p1, &m_pTo, &m_vTo, t);
		D3DXVec3Lerp(&p2, &p, &p1, t);
		if (m_pTarget)
		{
			if (m_vTo != p2)
				m_pTarget->SetPosition(p2);
		}
		break;
	case RIGHT:
		m_fPassedActionTime += g_pTimeManager->GetDeltaTime();
		t = m_fPassedActionTime / m_fActionTime;

		if (t > 1.0f)
		{
			t = 1.0f;
			if (m_pDelegate)
			{
				m_pDelegate->OnActionFinish(this);
			}
		}

		D3DXVec3Lerp(&p, &m_vFrom, &m_vTo, t);
		D3DXVec3Lerp(&p1, &m_pTo, &m_vTo, t);
		D3DXVec3Lerp(&p2, &p, &p1, t);
		if (m_pTarget)
		{
			if (m_vTo != p2)
				m_pTarget->SetPosition(p2);
		}
		break;
	case ATTACK_FORWARD:
		m_Count++;
		if (m_Count > 5 && m_Count < 25)
		{
			m_fPassedActionTime += g_pTimeManager->GetDeltaTime();
			t = m_fPassedActionTime / m_fActionTime;

			if (t > 1.0f)
			{
				t = 1.0f;
				if (m_pDelegate)
				{
					m_pDelegate->OnActionFinish(this);
				}
			}

			D3DXVec3Lerp(&p, &m_vFrom, &m_vTo, t);
			if (m_pTarget)
			{
				if (m_vTo != p)
					m_pTarget->SetPosition(p);
				m_pTarget->SetDirection(m_vFrom - m_vTo);
			}
		}
		break;
	case ATTACK_BACKWARD:
		m_Count++;
		if (m_Count > 5 && m_Count < 25)
		{
			m_fPassedActionTime += g_pTimeManager->GetDeltaTime();
			t = m_fPassedActionTime / m_fActionTime;

			if (t > 1.0f)
			{
				t = 1.0f;
				if (m_pDelegate)
				{
					m_pDelegate->OnActionFinish(this);
				}
			}

			D3DXVec3Lerp(&p, &m_vFrom, &m_vTo, t);
			if (m_pTarget)
			{
				if (m_vTo != p)
					m_pTarget->SetPosition(p);
				//m_pTarget->SetDirection(m_vFrom - m_vTo);
			}
		}
		break;
	case ATTACK_FLY:
		m_fPassedActionTime += g_pTimeManager->GetDeltaTime();
		t = m_fPassedActionTime / m_fActionTime;

		if (t > 2.0f)
		{
			t = 2.0f;
			if (m_pDelegate)
			{
				m_pDelegate->OnActionFinish(this);
			}
		}

		D3DXVec3Lerp(&p, &m_vFrom, &m_vTo, t);
		if (m_pTarget)
		{
			if (m_vTo != p)
				m_pTarget->SetPosition(p);
			m_pTarget->SetDirection(m_vFrom - m_vTo);
		}
		break;
	default:
		break;
	}
}

void cActionMove::MtUpdate()
{
	m_fPassedActionTime += g_pTimeManager->GetDeltaTime();
	float t = m_fPassedActionTime / m_fActionTime;

	if (t > 1.0f)
	{
		t = 1.0f;
		if (m_pDelegate)
		{
			m_pDelegate->OnActionFinish(this);
		}
	}

	D3DXVECTOR3 p(0, 0, 0);
	D3DXVECTOR3 p1(0, 0, 0);
	D3DXVECTOR3 p2(0, 0, 0);
	D3DXVec3Lerp(&p, &m_vFrom, &m_pTo, t);
	D3DXVec3Lerp(&p1, &m_pTo, &m_vTo, t);
	D3DXVec3Lerp(&p2, &p, &p1, t);
	if (m_pTarget)
	{
		m_pTarget->SetPosition(p2);
		m_pTarget->SetDirection(p - p1);

	}
}
