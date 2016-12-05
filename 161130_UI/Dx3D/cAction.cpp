#include "StdAfx.h"
#include "cAction.h"


cAction::cAction(void)
	: m_fActionTime(0.0f)
	, m_fPassedActionTime(0.0f)
	, m_pDelegate(NULL)
{
}
sfsfsfafsaefa
afeaefaef
awefaefeaf
cAction::~cAction(void)
{
}

void cAction::Start()
{

}

void cAction::Update()
{
	m_fPassedActionTime += g_pTimeManager->GetDeltaTime();
}
