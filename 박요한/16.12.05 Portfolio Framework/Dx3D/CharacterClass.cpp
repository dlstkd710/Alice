#include "stdafx.h"
#include "CharacterClass.h"
#include "iMap.h"
#include "cAnimation.h"
#include "cAllocateHierarchy.h"

#define RADIUS 0.6f
#define DELTA_Y 0.1f

CharacterClass::CharacterClass()
	: m_fSpeed(100.f)
	, m_fSpeedWalk(1.f / 5.7f)
	, m_dwCurrentTrack(0)
	, m_dTimeCurrent(0.0)
	, n(0)
	, m(0)
	, m_vCubePos(5, 0, 5)
	, m_vCubeDir(0, 0, 1)
	, m_fCubeSpeed(0.5f)
	, m_fAngle(0.0f)
	, m_eState(E_STATE_NORMAL)
	, m_fGravity(-0.02f)
	, m_fVY(0.0f)
	, AniOnOff(false)
	, OnKnife(false)
{
}

CharacterClass::~CharacterClass()
{
}

void CharacterClass::SetUp()
{
	_Animation = new cAnimation("./Alice46/", "AliceW.x");
	
	_Knife = new cAnimation("./Knife/", "Kife.X");

	_Gun = new cAnimation("./Gun/", "Gun.X");

	_Animation->SetAnimationIndex(16);

	_Gun->SetAnimationIndex(1);
	D3DXMatrixRotationY(&matR, ����(180.0f));

	
	_Knife->SetPosition(D3DXVECTOR3(0, 0, 0));

	_Animation->SetPosition(D3DXVECTOR3(5, 0, 5));

	_Gun->SetPosition(D3DXVECTOR3(0, 0, 0));
}

void CharacterClass::Release()
{
	_Animation->Release();
	_Knife->Release();
	_Gun->Release();
}

void CharacterClass::Controll(iMap* pMap /*= NULL*/)
{
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
	case E_STATE_1COMBO:
		Update1combo(pMap);
		break;
	case E_STATE_2COMBO:
		Update2combo(pMap);
		break;
	case E_STATE_3COMBO:
		Update3combo(pMap);
		break;
	case E_STATE_IDEL:
		UpdatefRunAndIdel(pMap);
		break;
	case E_STATE_GIDEL:
		UpdateGunIdel(pMap);
		break;
	}


	D3DXMatrixTranslation(&matT, m_vCubePos.x, m_vCubePos.y, m_vCubePos.z);

	if (GetKeyState('W') & 0x8000) {
		D3DXMatrixRotationY(&matR, ����(-180.0f));
		m_vCubeDir = D3DXVECTOR3(0, 0, -1);
	}
	if (GetKeyState('S') & 0x8000) {
		D3DXMatrixRotationY(&matR, ����(-1.0f));
		m_vCubeDir = D3DXVECTOR3(0, 0, 1);
	}
	if (GetKeyState('A') & 0x8000)
	{
		D3DXMatrixRotationY(&matR, ����(90.0f));
		m_vCubeDir = D3DXVECTOR3(0, 0, 1);
	}
	if (GetKeyState('D') & 0x8000)
	{
		D3DXMatrixRotationY(&matR, ����(-90.0f));
		m_vCubeDir = D3DXVECTOR3(0, 0, -1);
	}

	if ((GetKeyState('A') & 0x8000) && (GetKeyState('W') & 0x8000)) {
		D3DXMatrixRotationY(&matR, ����(130.0f));
		m_vCubeDir = D3DXVECTOR3(0, 0, -1);
	}
	if ((GetKeyState('W') & 0x8000) && (GetKeyState('D') & 0x8000))
		D3DXMatrixRotationY(&matR, ����(-135.0f));

	if ((GetKeyState('A') & 0x8000) && (GetKeyState('S') & 0x8000))
		D3DXMatrixRotationY(&matR, ����(45.0f));

	if ((GetKeyState('S') & 0x8000) && (GetKeyState('D') & 0x8000)) {
		D3DXMatrixRotationY(&matR, ����(-45.0f));
		m_vCubeDir = D3DXVECTOR3(0, 0, 1);
	}

	m_matWorld = matR * matT;

	D3DXVec3TransformCoord(&m_vCubeDir, &m_vCubeDir, &matR);
}

void CharacterClass::Render()
{
	_Animation->UpdateAndRender(&m_matWorld);

	// �����̽ÿ� �� ����
	if (m_eState == E_STATE_GFIRE ||
		m_eState == E_STATE_GIDEL)
	{
		D3DXFRAME_DERIVED * tempBone1 = _Animation->GetFineBONE("Bip01-Prop1");

		D3DXMATRIXA16 tempMat1 = tempBone1->CombinedTransformationMatrix;

		D3DXFRAME_DERIVED* GunLocal = _Gun->m_pRootFrame;
		// ��Į�� Į�� ��ü
		D3DXMATRIXA16 GunLocalMat = GunLocal->TransformationMatrix;
		//Į�Ƿ��� ��Ʈ���� Į�� ���� ��Ʈ����
		D3DXMATRIXA16 roteX1, roteY1;
		D3DXMatrixIdentity(&roteX1);
		D3DXMatrixIdentity(&roteY1);
		// Į�� �ʱ�ȭ �ϰ� ������
		//
		D3DXMatrixRotationX(&roteX1, ����(-70.0f));
		D3DXMatrixRotationY(&roteY1, ����(280.0f));

		GunLocalMat = roteY1 * roteX1 ;
		//==================
		D3DXMATRIXA16 matFinal1 = GunLocalMat * tempMat1;
		// ��������Ʈ������ = Į�� ���� ��Ʈ������ ���� ��Ʈ���� ���ϱ�

		_Gun->UpdateAndRender(&matFinal1);
		// Į�׸���

	}
	if (m_eState == E_STATE_IDEL ||
		m_eState == E_STATE_RUNIDEL ||
		m_eState == E_STATE_1COMBO ||
		m_eState == E_STATE_2COMBO ||
		m_eState == E_STATE_3COMBO)
	{
		D3DXFRAME_DERIVED* tempBone = _Animation->GetFineBONE("Bip01-Prop1");
		//���� ã�Ƽ� ����� �־��
		D3DXMATRIXA16 tempMat = tempBone->CombinedTransformationMatrix;
		// ����� ������ ��Ʈ������ �� ��Ʈ������ �־��
		D3DXFRAME_DERIVED* knifeLocal = _Knife->m_pRootFrame;
		// ��Į�� Į�� ��ü
		D3DXMATRIXA16 knifeLocalMat = knifeLocal->TransformationMatrix;
		//Į�Ƿ��� ��Ʈ���� Į�� ���� ��Ʈ����

		D3DXMATRIXA16 roteX;
		D3DXMatrixIdentity(&roteX);
		// Į�� �ʱ�ȭ �ϰ� ������
		//
		D3DXMatrixRotationX(&roteX, ����(180.0f));

		knifeLocalMat = roteX; 
		// Į�� ���� ��Ʈ���� = �����̼ǿ�x �� ���� ����

		D3DXMATRIXA16 matFinal = knifeLocalMat * tempMat;
		// ��������Ʈ������ = Į�� ���� ��Ʈ������ ���� ��Ʈ���� ���ϱ�

		_Knife->UpdateAndRender(&matFinal);
		// Į�׸���
	}
}

void CharacterClass::SetintdexAnimationLep(int intdex)
{

	dwNewTrack = (m_dwCurrentTrack == 0 ? 1 : 0); // Ʈ���� ���̰� 0  -  1

	_Animation->GetAnimController(&pAC);

	pAC->GetAnimationSet(intdex, &pAS);

	pAC->SetTrackAnimationSet(dwNewTrack, pAS);

	pAS->Release();

	double b = 0;
	b = pAS->GetPeriod();
	

	pAC->ResetTime();

	pAC->UnkeyAllTrackEvents(m_dwCurrentTrack);
	pAC->UnkeyAllTrackEvents(dwNewTrack);

	pAC->KeyTrackEnable(m_dwCurrentTrack, FALSE, g_pTimeManager->GetDeltaTime() + (b / 6));
	pAC->KeyTrackSpeed(m_dwCurrentTrack, 0.0f, g_pTimeManager->GetDeltaTime(), b / 4, D3DXTRANSITION_LINEAR);
	pAC->KeyTrackWeight(m_dwCurrentTrack, 0.0f, g_pTimeManager->GetDeltaTime(),  b / 4, D3DXTRANSITION_LINEAR);
	
	pAC->SetTrackEnable(dwNewTrack, TRUE);
	pAC->KeyTrackSpeed(dwNewTrack, 1.0f, g_pTimeManager->GetDeltaTime(),  b / 4, D3DXTRANSITION_LINEAR);
	pAC->KeyTrackWeight(dwNewTrack, 1.0f, g_pTimeManager->GetDeltaTime(),  b / 4, D3DXTRANSITION_LINEAR);

	m_dwCurrentTrack = dwNewTrack;

	pAC->ResetTime();


	pAC->Release();
}

void CharacterClass::UpdateNormal(iMap* pMap /*= NULL*/)
{
	SetintdexAnimationLep(16);

	D3DXVECTOR3 vTempPos = m_vCubePos;
	if (GetKeyState('W') & 0x8000) {
		SetintdexAnimationLep(12);
		vTempPos = vTempPos + m_vCubeDir * m_fCubeSpeed;
	}
	else if (GetKeyState('S') & 0x8000) {
		SetintdexAnimationLep(12);
		vTempPos = vTempPos - m_vCubeDir * m_fCubeSpeed;
	}
	else if (GetKeyState('A') & 0x8000) {
		SetintdexAnimationLep(12);
		vTempPos = vTempPos - m_vCubeDir * m_fCubeSpeed;
	}
	else if (GetKeyState('D') & 0x8000) {
		SetintdexAnimationLep(12);
		vTempPos = vTempPos + m_vCubeDir * m_fCubeSpeed;
	}
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		m_fVY = 0.5f;	//���� ����
		m_eState = E_STATE_JUMPUP;
		SetintdexAnimationLep(15);
		return;
	}
	if (GetKeyState('1') & 0x8000)
	{
		m_fVY = 0.5f;	//���� ����
		m_eState = E_STATE_IDEL;
		SetintdexAnimationLep(8);
		return;
	}
	if (GetKeyState('2') & 0x8000)
	{
		m_eState = E_STATE_GIDEL;
		SetintdexAnimationLep(1);
		return;
	}
	if (GetKeyState(VK_LBUTTON) & 0x8000)
	{
		m_eState = E_STATE_1COMBO;
		SetintdexAnimationLep(11);
		return;
	}
	if (pMap && pMap->GetHeight(vTempPos.x, vTempPos.y, vTempPos.z))
	{
		if (vTempPos.y - m_vCubePos.y > DELTA_Y) // ������ ���� ���� ����
		{
			m_vCubePos = vTempPos;
		}
		else if (m_vCubePos.y - vTempPos.y > DELTA_Y) // ������ ���� ���� ����
		{
			m_fVY = 0.0f;
			m_eState = E_STATE_JUMPDOWN;
			return;
		}
		else {
			m_vCubePos = vTempPos;
		}
	}
	if (pMap == NULL){
		m_vCubePos = vTempPos;
	}

}

void CharacterClass::UpdateJumpUp(iMap* pMap /*= NULL*/)
{
	if (GetAsyncKeyState(VK_SPACE) & 1){
		m_fVY = 0.5f;	//���� ����
		SetintdexAnimationLep(14);
		m_eState = E_STATE_JUMPDOUBLE;
		return;
	}

	if (m_fVY < 0)
	{
		SetintdexAnimationLep(15);
		m_eState = E_STATE_JUMPDOWN;
		return;
	}
	m_fVY += m_fGravity;

	D3DXVECTOR3 vTempPos = m_vCubePos;
	if (GetKeyState('W') & 0x8000) {
		vTempPos = vTempPos + m_vCubeDir * m_fCubeSpeed;
	}
	else if (GetKeyState('S') & 0x8000) {
		vTempPos = vTempPos - m_vCubeDir * m_fCubeSpeed;
	}
	else if (GetKeyState('A') & 0x8000) {
		vTempPos = vTempPos - m_vCubeDir * m_fCubeSpeed;
	}
	else if (GetKeyState('D') & 0x8000) {
		vTempPos = vTempPos + m_vCubeDir * m_fCubeSpeed;
	}

	if (pMap == NULL){
		m_vCubePos = vTempPos;
		return;
	}

	if (pMap->GetHeight(vTempPos.x, vTempPos.y, vTempPos.z)){
		if (vTempPos.y < m_vCubePos.y)
		{
			m_vCubePos.x = vTempPos.x;
			m_vCubePos.z = vTempPos.z;
		}
	}
	m_vCubePos.y += m_fVY;
}

void CharacterClass::UpdateJumpDown(iMap* pMap /*= NULL*/)
{
	if (GetAsyncKeyState(VK_SPACE) & 0x8001)
	{
		m_fVY -= -0.018f;
		SetintdexAnimationLep(13);
	}

	m_fVY += m_fGravity;

	D3DXVECTOR3 vTempPos = m_vCubePos;
	if (GetKeyState('W') & 0x8000) {
		vTempPos = vTempPos + m_vCubeDir * m_fCubeSpeed;
	}
	else if (GetKeyState('S') & 0x8000) {
		vTempPos = vTempPos - m_vCubeDir * m_fCubeSpeed;
	}
	else if (GetKeyState('A') & 0x8000) {
		vTempPos = vTempPos - m_vCubeDir * m_fCubeSpeed;
	}
	else if (GetKeyState('D') & 0x8000) {
		vTempPos = vTempPos + m_vCubeDir * m_fCubeSpeed;
	}
	if (pMap == NULL)
	{
		m_vCubePos = vTempPos;
		return;
	}

	if (pMap->GetHeight(vTempPos.x, vTempPos.y, vTempPos.z))
	{
		if (m_vCubePos.y > vTempPos.y) // ������ ���� �� �������
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
		else{
			m_vCubePos.y += m_fVY;

			pMap->GetHeight(m_vCubePos.x, vTempPos.y, m_vCubePos.z);

			if (m_vCubePos.y < vTempPos.y){
				m_eState = E_STATE_NORMAL;
				m_vCubePos.y = vTempPos.y;
			}
			return;
		}
	}
	else{
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

void CharacterClass::UpdateJumpUpUp(iMap * pMap /*= NULL*/)
{
	if (m_fVY < 0){
		m_eState = E_STATE_JUMPDOWN;
		SetintdexAnimationLep(15);
		return;
	}

	m_fVY += m_fGravity;

	D3DXVECTOR3 vTempPos = m_vCubePos;
	if (GetKeyState('W') & 0x8000) {
		vTempPos = vTempPos + m_vCubeDir * m_fCubeSpeed;
	}
	if (GetKeyState('S') & 0x8000) {
		vTempPos = vTempPos - m_vCubeDir * m_fCubeSpeed;
	}
	if (GetKeyState('A') & 0x8000) {
		vTempPos = vTempPos - m_vCubeDir * m_fCubeSpeed;
	}
	if (GetKeyState('D') & 0x8000) {
		vTempPos = vTempPos + m_vCubeDir * m_fCubeSpeed;
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
//
void CharacterClass::Update1combo(iMap * pMap /*= NULL*/)
{
	_Animation->GetAnimController(&pAC);

	pAC->GetAnimationSet(11, &pAS);

	pAC->SetTrackAnimationSet(0, pAS);


	double b = 0;
	b = pAS->GetPeriod();

	float a = b / g_pTimeManager->GetDeltaTime();

	if (a >= 0.95) {
	pAC->KeyTrackWeight(0, 0.0f, g_pTimeManager->GetDeltaTime(), b / 4, D3DXTRANSITION_LINEAR);
	m_eState = E_STATE_NORMAL;
	}


	
}

void CharacterClass::Update2combo(iMap * pMap /*= NULL*/)
{

}

void CharacterClass::Update3combo(iMap * pMap /*= NULL*/)
{

}

void CharacterClass::UpdatefRunAndIdel(iMap * pMap)
{
	SetintdexAnimationLep(8);

	D3DXMATRIXA16 matR, matT;
	D3DXMatrixRotationY(&matR, ����(180.0f));

	D3DXVECTOR3 vTempPos = m_vCubePos;
	if (GetKeyState('W') & 0x8000) {
		SetintdexAnimationLep(7);
		vTempPos = vTempPos + m_vCubeDir * m_fCubeSpeed;

	}
	else if (GetKeyState('S') & 0x8000) {
		SetintdexAnimationLep(7);
		vTempPos = vTempPos - m_vCubeDir * m_fCubeSpeed;
	}
	else if (GetKeyState('A') & 0x8000) {
		SetintdexAnimationLep(7);
		vTempPos = vTempPos - m_vCubeDir * m_fCubeSpeed;
	}
	else if (GetKeyState('D') & 0x8000) {
		SetintdexAnimationLep(7);
		vTempPos = vTempPos + m_vCubeDir * m_fCubeSpeed;
	}
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		m_fVY = 0.5f;	//���� ����
		m_eState = E_STATE_JUMPUP;
		SetintdexAnimationLep(15);
		return;
	}

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		SetintdexAnimationLep(11);
		return;
	}

	if (GetKeyState('3') & 0x8000)
	{
		m_eState = E_STATE_NORMAL;
	}

	if (pMap && pMap->GetHeight(vTempPos.x, vTempPos.y, vTempPos.z))
	{
		if (vTempPos.y - m_vCubePos.y > DELTA_Y) // ������ ���� ���� ����
		{
			m_vCubePos = vTempPos;
		}
		else if (m_vCubePos.y - vTempPos.y > DELTA_Y) // ������ ���� ���� ����
		{
			m_fVY = 0.0f;
			m_eState = E_STATE_JUMPDOWN;
			return;
		}
		else {
			m_vCubePos = vTempPos;
		}
	}
	if (pMap == NULL) {
		m_vCubePos = vTempPos;
	}

}

void CharacterClass::UpdateGunIdel(iMap * pMap)
{
	SetintdexAnimationLep(1);

	D3DXMATRIXA16 matR, matT;
	D3DXMatrixRotationY(&matR, ����(180.0f));

	D3DXVECTOR3 vTempPos = m_vCubePos;
	if (GetKeyState('W') & 0x8000) {
		SetintdexAnimationLep(6);
		vTempPos = vTempPos + m_vCubeDir * m_fCubeSpeed;
	}
	else if (GetKeyState('S') & 0x8000) {
		SetintdexAnimationLep(6);
		vTempPos = vTempPos - m_vCubeDir * m_fCubeSpeed;
	}
	else if (GetKeyState('A') & 0x8000) {
		SetintdexAnimationLep(6);
		vTempPos = vTempPos - m_vCubeDir * m_fCubeSpeed;
	}
	else if (GetKeyState('D') & 0x8000) {
		SetintdexAnimationLep(6);
		vTempPos = vTempPos + m_vCubeDir * m_fCubeSpeed;
	}
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		m_fVY = 0.5f;	//���� ����
		m_eState = E_STATE_JUMPUP;
		SetintdexAnimationLep(15);
		return;
	}

	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		_Animation->SetAnimationIndex(0);
		return;
	}

	if (GetKeyState('3') & 0x8000)
	{
		m_eState = E_STATE_NORMAL;
	}

	if (pMap && pMap->GetHeight(vTempPos.x, vTempPos.y, vTempPos.z))
	{
		if (vTempPos.y - m_vCubePos.y > DELTA_Y) // ������ ���� ���� ����
		{
			m_vCubePos = vTempPos;
		}
		else if (m_vCubePos.y - vTempPos.y > DELTA_Y) // ������ ���� ���� ����
		{
			m_fVY = 0.0f;
			m_eState = E_STATE_JUMPDOWN;
			return;
		}
		else {
			m_vCubePos = vTempPos;
		}
	}
	if (pMap == NULL) {
		m_vCubePos = vTempPos;
	}

}
