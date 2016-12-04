#include "StdAfx.h"
#include "cActionSeq.h"


cActionSeq::cActionSeq(void)
	: m_nCurrIndex(0)
{
}


cActionSeq::~cActionSeq(void)
{
	for each(auto p in m_vecAction)
	{
		SAFE_RELEASE(p);
	}
}

void cActionSeq::AddAction( cAction* pAction )
{
	if(pAction)
	{
		SAFE_ADD_REF(pAction);
		m_vecAction.push_back(pAction);
	}
}

void cActionSeq::Start()
{
	m_nCurrIndex = 0;
	if((size_t)m_nCurrIndex < m_vecAction.size())
	{
		m_vecAction[m_nCurrIndex]->Start();
	}
}

void cActionSeq::Update()
{
	if ((size_t)m_nCurrIndex < m_vecAction.size())
	{
		m_vecAction[m_nCurrIndex]->Update();
	}
}

void cActionSeq::OnActionFinish( cAction* pAction )
{
	++m_nCurrIndex;
	if ((size_t)m_nCurrIndex < m_vecAction.size())
	{
		m_vecAction[m_nCurrIndex]->Start();
	}
	else if (m_pDelegate)
	{
		m_pDelegate->OnActionFinish(this);
	}
}
