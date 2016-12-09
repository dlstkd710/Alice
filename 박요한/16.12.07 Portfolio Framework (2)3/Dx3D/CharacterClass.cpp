#include "stdafx.h"
#include "CharacterClass.h"
#include "iMap.h"
#include "cAnimation.h"
#include "cAllocateHierarchy.h"

#define RADIUS 0.3f
#define DELTA_Y 0.1f

CharacterClass::CharacterClass()
	: m_fSpeed(100.f)
	, m_vDirection1(1, 0, 0)
	, m_dwCurrentTrack(0)
	, m_dTimeCurrent(0.0)
	, n(0)
	, m(0)
	, a(0)
	, t(0)
	, m_vCubePos(5, 0, 5)
	, m_vCubeDir(0, 0, 1)
	, m_fCubeSpeed(0.5f)
	, m_fAngle(0.0f)
	, m_eState(E_STATE_NORMAL)
	, m_fGravity(-0.02f)
	, m_fVY(0.0f)
	, m_fBlendTime(0.3f)
	, m_fPassedBlendTime(0.0f)
	, m_isBlending(false)
	, m_isOnceLoop(false)
	, c(0)
{
}
CharacterClass::~CharacterClass()
{
}
void CharacterClass::SetUp()
{
	_Animation = new cAnimation("./Alice48/", "AliceW.x");
	_Knife = new cAnimation("./Knife/", "Kife.X");
	_Gun = new cAnimation("./Gun/", "Gun.X");
	_Hammor = new cAnimation("./weapon/", "Hammor.X");

	D3DXMatrixRotationY(&matR, ����(180.0f));
	_Animation->SetAnimationIndex(21);
	_Gun->SetAnimationIndex(1);

	_Animation->SetPosition(D3DXVECTOR3(0, 0, 0));

	_Knife->SetPosition(D3DXVECTOR3(0, 0, 0));
	_Gun->SetPosition(D3DXVECTOR3(0, 0, 0));

	_Animation->GetAnimController(&pAC);
}
void CharacterClass::Release()
{
	_Animation->Release();
	_Knife->Release();
	_Gun->Release();
	_Hammor->Release();
}

void CharacterClass::MotionControllTower(iMap * pMap)
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
	case E_STATE_TELPO:
		UpdateTelpo(pMap);
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
	case E_STATE_HAMMOR:
		UpdateHammor(pMap);
		break;
	}
}

void CharacterClass::Controll(iMap* pMap , float CameraAngleX, float CameraAngleY /*= NULL*/)
{
	MotionControllTower(pMap); // ĳ���� ��� ���� �Լ�

	isBlending();

	D3DXMatrixTranslation(&matT, m_vCubePos.x, m_vCubePos.y, m_vCubePos.z);

	if (GetKeyState('W') & 0x8000) {
		D3DXMatrixRotationY(&matR, ����(-180.0f) + CameraAngleY);
		m_vCubeDir = D3DXVECTOR3(0, 0, -1);
	}
	if (GetKeyState('S') & 0x8000) {
		D3DXMatrixRotationY(&matR, ����(-1.0f) + CameraAngleY);
		m_vCubeDir = D3DXVECTOR3(0, 0, 1);
	}
	if (GetKeyState('A') & 0x8000) {
		D3DXMatrixRotationY(&matR, ����(90.0f) + CameraAngleY);
		m_vCubeDir = D3DXVECTOR3(0, 0, 1);
	}
	if (GetKeyState('D') & 0x8000) {
		D3DXMatrixRotationY(&matR, ����(-90.0f) + CameraAngleY);
		m_vCubeDir = D3DXVECTOR3(0, 0, -1);
	}

	if ((GetKeyState('A') & 0x8000) && (GetKeyState('W') & 0x8000)) {
		D3DXMatrixRotationY(&matR, ����(130.0f) + CameraAngleY);
		m_vCubeDir = D3DXVECTOR3(0, 0, -1);
	}
	if ((GetKeyState('W') & 0x8000) && (GetKeyState('D') & 0x8000)) {
		D3DXMatrixRotationY(&matR, ����(-135.0f)+ CameraAngleY);
	}

	if ((GetKeyState('A') & 0x8000) && (GetKeyState('S') & 0x8000)) {
		D3DXMatrixRotationY(&matR, ����(45.0f) + CameraAngleY);
	}

	if ((GetKeyState('S') & 0x8000) && (GetKeyState('D') & 0x8000)) {
		D3DXMatrixRotationY(&matR, ����(-45.0f) + CameraAngleY);
		m_vCubeDir = D3DXVECTOR3(0, 0, 1);
	}

	m_matWorld = matR * matT;

	D3DXVec3TransformCoord(&m_vCubeDir, &m_vCubeDir, &matR);

	m_vecVertex1.resize(6);

	D3DXVECTOR3 temp = m_vCubePos;

	D3DXMATRIXA16 matView;
	D3DXMatrixIdentity(&matView);
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMATRIXA16 matProjection;
	D3DXMatrixIdentity(&matProjection);

	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProjection);
	D3DXMATRIXA16 matViewProjection =  matView * matProjection;

	D3DXVECTOR3 temp2;
	D3DXVec3TransformCoord(&temp2, &temp, &matViewProjection);

	m_vecVertex1[0].p = D3DXVECTOR3(temp2.x - 20, temp2.y - 20 , temp2.z);
	m_vecVertex1[0].t = D3DXVECTOR2(0, 1);
	m_vecVertex1[0].c = D3DCOLOR_ARGB(128, 255, 255, 255);

	m_vecVertex1[1].p = D3DXVECTOR3(temp2.x - 20, temp2.y+ 20 ,temp2.z);
	m_vecVertex1[1].t = D3DXVECTOR2(0, 0);
	m_vecVertex1[1].c = D3DCOLOR_ARGB(128, 255, 255, 255);

	m_vecVertex1[2].p = D3DXVECTOR3(temp2.x + 20, temp2.y + 20, temp2.z);
	m_vecVertex1[2].t = D3DXVECTOR2(1, 0);
	m_vecVertex1[2].c = D3DCOLOR_ARGB(128, 255, 255, 255);

	m_vecVertex1[3].p = D3DXVECTOR3(temp2.x - 20, temp2.y - 20, temp2.z);
	m_vecVertex1[3].t = D3DXVECTOR2(0, 1);
	m_vecVertex1[3].c = D3DCOLOR_ARGB(128, 255, 255, 255);

	m_vecVertex1[4].p = D3DXVECTOR3(temp.x + 20, temp2.y + 20, temp2.z);
	m_vecVertex1[4].t = D3DXVECTOR2(1, 0);
	m_vecVertex1[4].c = D3DCOLOR_ARGB(128, 255, 255, 255);

	m_vecVertex1[5].p = D3DXVECTOR3(temp.x + 20, temp.y - 20,temp.z);
	m_vecVertex1[5].t = D3DXVECTOR2(1, 1);
	m_vecVertex1[5].c = D3DCOLOR_ARGB(128, 255, 255, 255);

}

void CharacterClass::Render()
{
	_Animation->UpdateAndRender(&m_matWorld);

	if (m_eState == E_STATE_HAMMOR) {
		D3DXFRAME_DERIVED * tempBone2 = _Animation->GetFineBONE("Bip01-Prop1");

		D3DXMATRIXA16 tempMat2 = tempBone2->CombinedTransformationMatrix;

		D3DXFRAME_DERIVED* Hframe = _Gun->m_pRootFrame;
		// ��Į�� Į�� ��ü
		D3DXMATRIXA16 HLocalMat = Hframe->TransformationMatrix;
		//Į�Ƿ��� ��Ʈ���� Į�� ���� ��Ʈ����
		D3DXMATRIXA16 roteX1, roteY1, matT;
		D3DXMatrixIdentity(&roteX1);
		D3DXMatrixIdentity(&roteY1);
		D3DXMatrixIdentity(&matT);
		// Į�� �ʱ�ȭ �ϰ� ����
		D3DXMatrixRotationX(&roteY1, ����(360.0f));
		HLocalMat = roteY1;
		//==================
		D3DXMATRIXA16 matFinal1 = HLocalMat * tempMat2;
		// ��������Ʈ������ = Į�� ���� ��Ʈ������ ���� ��Ʈ���� ���ϱ�
		_Hammor->UpdateAndRender(&matFinal1);

	}
	if (m_eState == E_STATE_GFIRE ||
		m_eState == E_STATE_GIDEL)
	{
		D3DXFRAME_DERIVED * tempBone1 = _Animation->GetFineBONE("Bip01-Prop2");

		D3DXMATRIXA16 tempMat1 = tempBone1->CombinedTransformationMatrix;

		D3DXFRAME_DERIVED* GunLocal = _Gun->m_pRootFrame;
		// ��Į�� Į�� ��ü
		D3DXMATRIXA16 GunLocalMat = GunLocal->TransformationMatrix;
		//Į�Ƿ��� ��Ʈ���� Į�� ���� ��Ʈ����
		D3DXMATRIXA16 roteX1, roteY1, matT;
		D3DXMatrixIdentity(&roteX1);
		D3DXMatrixIdentity(&roteY1);
		D3DXMatrixIdentity(&matT);
		// Į�� �ʱ�ȭ �ϰ� ����
		D3DXMatrixRotationY(&roteY1, ����(280.0f));
		D3DXMatrixRotationX(&roteX1, ����(-85.0f));
		D3DXMatrixTranslation(&matT, 0.4, -2.4 , 0);
		GunLocalMat = roteY1 * roteX1 * matT;
		//==================
		D3DXMATRIXA16 matFinal1 = GunLocalMat * tempMat1;
		// ��������Ʈ������ = Į�� ���� ��Ʈ������ ���� ��Ʈ���� ���ϱ�

		_Gun->UpdateAndRender(&matFinal1);
		// Į�׸���
	}
	if (m_eState == E_STATE_IDEL ||
		m_eState == E_STATE_RUNIDEL||
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
		D3DXMatrixRotationX(&roteX, ����(180.0f));

		knifeLocalMat = roteX;
		// Į�� ���� ��Ʈ���� = �����̼ǿ�x �� ���� ����
		D3DXMATRIXA16 matFinal = knifeLocalMat * tempMat;
		// ��������Ʈ������ = Į�� ���� ��Ʈ������ ���� ��Ʈ���� ���ϱ�
		_Knife->UpdateAndRender(&matFinal);		// Į�׸���
	}

	Renderstate();
}

void CharacterClass::UpdateNormal(iMap* pMap /*= NULL*/)
{

	D3DXVECTOR3 vTempPos = m_vCubePos;
	if (GetKeyState('W') & 0x8000) {
		SetintdexAnimationLep(17);
		vTempPos = vTempPos + m_vCubeDir * m_fCubeSpeed;
	}
	else if (GetKeyState('S') & 0x8000) {
		SetintdexAnimationLep(17);
		vTempPos = vTempPos - m_vCubeDir * m_fCubeSpeed;
	}
	else if (GetKeyState('A') & 0x8000) {
		SetintdexAnimationLep(17);
		vTempPos = vTempPos - m_vCubeDir * m_fCubeSpeed;
	}
	else if (GetKeyState('D') & 0x8000) {
		SetintdexAnimationLep(17);
		vTempPos = vTempPos + m_vCubeDir * m_fCubeSpeed;
	}
	else
	{
		SetintdexAnimationLep(21);
	}
	if (g_pkeyManager->isOnceKeyDown(VK_SPACE)){
		m_fVY = 0.6f;	//���� ����
		m_eState = E_STATE_JUMPUP;
		_Animation->SetAnimationIndex(20);
		return;
	}
	if (GetKeyState('1') & 0x8000){
		m_fVY = 0.5f;	//���� ����
		m_eState = E_STATE_IDEL;
		SetintdexAnimationLep(13);
		return;
	}
	if (GetKeyState('2') & 0x8000){
		m_eState = E_STATE_GIDEL;
		SetintdexAnimationLep(16);
		return;
	}
	if (GetKeyState('3') & 0x8000) {
		m_eState = E_STATE_HAMMOR ;
		SetintdexAnimationLep(4);
		return;
	}


	if (g_pkeyManager->isOnceKeyDown(VK_SHIFT)) {
		m_eState = E_STATE_TELPO;
		a = 2;
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

void CharacterClass::UpdateJumpUp(iMap* pMap /*= NULL*/)
{
	if (g_pkeyManager->isOnceKeyDown(VK_SPACE)) {
		m_fVY = 0.5f;	//���� ����
		t = 3;
		SetintdexAnimationLep(19);
		m_eState = E_STATE_JUMPDOUBLE;

		return;
	}

	if (m_fVY < 0)
	{
		SetintdexAnimationLep(20);
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

	if (pMap == NULL) {
		m_vCubePos = vTempPos;
		return;
	}

	if (pMap->GetHeight(vTempPos.x, vTempPos.y, vTempPos.z)) {
		if (vTempPos.y < m_vCubePos.y)
		{
			m_vCubePos.x = vTempPos.x;
			m_vCubePos.z = vTempPos.z;
		}
	}
	m_vCubePos.y += m_fVY;
}

void CharacterClass::UpdateJumpUpUp(iMap * pMap /*= NULL*/)
{
	if (t != 4) {
		if (g_pkeyManager->isOnceKeyDown(VK_SPACE)) {
			SetintdexAnimationLep(19);
			m_fVY = 0.5f;	//���� ��
			t = 4;
			return;
		}
	}
	if (m_fVY < 0) {
		SetintdexAnimationLep(20);
		m_eState = E_STATE_JUMPDOWN;
		return;
	}
	m_fVY += m_fGravity;

	D3DXVECTOR3 vTempPos = m_vCubePos;

	if (GetKeyState('W') & 0x8000)
		vTempPos = vTempPos + m_vCubeDir * m_fCubeSpeed;
	else if (GetKeyState('S') & 0x8000)
		vTempPos = vTempPos - m_vCubeDir * m_fCubeSpeed;
	else if (GetKeyState('A') & 0x8000)
		vTempPos = vTempPos - m_vCubeDir * m_fCubeSpeed;
	else if (GetKeyState('D') & 0x8000)
		vTempPos = vTempPos + m_vCubeDir * m_fCubeSpeed;

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

void CharacterClass::UpdateJumpDown(iMap* pMap /*= NULL*/)
{
	if (g_pkeyManager->isStayKeyDown(VK_SPACE)){
		m_fVY -= -0.018f;
		SetintdexAnimationLep(18);
	}
	if (g_pkeyManager->isOnceKeyDown(VK_SHIFT)) {
		m_eState = E_STATE_TELPO;
		m_fVY += m_fGravity;
		a = 2;
		SetintdexAnimationLep(18);
		return;
	}
	m_fVY += m_fGravity;

	D3DXVECTOR3 vTempPos = m_vCubePos;

	if (GetKeyState('W') & 0x8000)
		vTempPos = vTempPos + m_vCubeDir * m_fCubeSpeed;
	else if (GetKeyState('S') & 0x8000)
		vTempPos = vTempPos - m_vCubeDir * m_fCubeSpeed;
	else if (GetKeyState('A') & 0x8000)
		vTempPos = vTempPos - m_vCubeDir * m_fCubeSpeed;
	else if (GetKeyState('D') & 0x8000)
		vTempPos = vTempPos + m_vCubeDir * m_fCubeSpeed;
	
	if (pMap == NULL){
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
		else {
			m_vCubePos.y += m_fVY;

			pMap->GetHeight(m_vCubePos.x, vTempPos.y, m_vCubePos.z);

			if (m_vCubePos.y < vTempPos.y) {
				m_eState = E_STATE_NORMAL;
				m_vCubePos.y = vTempPos.y;
			}
			return;
		}
	}
	else {
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

void CharacterClass::UpdateTelpo(iMap * pMap)
{
	if (a != 3) {
		D3DXVECTOR3 vTempPos = m_vCubeDir;
		D3DXVECTOR3 TelPoPos = D3DXVECTOR3(30, 0, 30) + vTempPos;

		if(GetKeyState('W') & 0x8000)
		m_vCubePos.z += TelPoPos.z;
		else if (GetKeyState('S') & 0x8000)
		m_vCubePos.z -= TelPoPos.z;
		else if (GetKeyState('A') & 0x8000)
		m_vCubePos.x += TelPoPos.x;
		else if (GetKeyState('D') & 0x8000)
		m_vCubePos.x += TelPoPos.x;
		else
		m_vCubePos.z += TelPoPos.z;

		a = 3;
	}
	m_eState = E_STATE_NORMAL;
}

void CharacterClass::Update1combo(iMap * pMap /*= NULL*/)
{
}

void CharacterClass::Update2combo(iMap * pMap /*= NULL*/)
{
}

void CharacterClass::Update3combo(iMap * pMap /*= NULL*/)
{
}

void CharacterClass::UpdatefRunAndIdel(iMap * pMap)
{
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
	else {
		SetintdexAnimationLep(13);
	}
	if (GetAsyncKeyState(VK_SPACE) & 0x8000){ 
		m_fVY = 0.5f;	//���� ����
		m_eState = E_STATE_JUMPUP;
		SetintdexAnimationLep(20);
		return;
	}

	if (GetKeyState(VK_LBUTTON) & 0x8000){
		//m_isOnceLoop = true;
		SetintdexAnimationLep(16);
		return;
	}


	if (GetKeyState('4') & 0x8000){
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
	D3DXVECTOR3 vTempPos = m_vCubePos;
	if (GetKeyState('W') & 0x8000) {
		SetintdexAnimationLep(11);
		vTempPos = vTempPos + m_vCubeDir * m_fCubeSpeed;
	}
	else if (GetKeyState('S') & 0x8000) {
		SetintdexAnimationLep(11);
		vTempPos = vTempPos - m_vCubeDir * m_fCubeSpeed;
	}
	else if (GetKeyState('A') & 0x8000) {
		SetintdexAnimationLep(11);
		vTempPos = vTempPos - m_vCubeDir * m_fCubeSpeed;
	}
	else if (GetKeyState('D') & 0x8000) {
		SetintdexAnimationLep(11);
		vTempPos = vTempPos + m_vCubeDir * m_fCubeSpeed;
	}
	else if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		SetintdexAnimationLep(5);
		_Gun->SetAnimationIndex(2);
		return;
	}
	else {
		SetintdexAnimationLep(6);
		_Gun->SetAnimationIndex(1);
	}
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		m_fVY = 0.5f;	//���� ����
		m_eState = E_STATE_JUMPUP;
		_Animation->SetAnimationIndex(20);
		return;
	}


	if (GetKeyState('4') & 0x8000)
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

void CharacterClass::UpdateHammor(iMap * pMap)
{
	D3DXVECTOR3 vTempPos = m_vCubePos;
	if (GetKeyState('W') & 0x8000) {
		SetintdexAnimationLep(3);
		vTempPos = vTempPos + m_vCubeDir * m_fCubeSpeed;
	}
	else if (GetKeyState('S') & 0x8000) {
		SetintdexAnimationLep(3);
		vTempPos = vTempPos - m_vCubeDir * m_fCubeSpeed;
	}
	else if (GetKeyState('A') & 0x8000) {
		SetintdexAnimationLep(3);
		vTempPos = vTempPos - m_vCubeDir * m_fCubeSpeed;
	}
	else if (GetKeyState('D') & 0x8000) {
		SetintdexAnimationLep(3);
		vTempPos = vTempPos + m_vCubeDir * m_fCubeSpeed;
	}
	else 
		SetintdexAnimationLep(4);

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		m_fVY = 0.5f;	//���� ����
		m_eState = E_STATE_JUMPUP;
		_Animation->SetAnimationIndex(20);
		return;
	}
	if (GetKeyState(VK_LBUTTON) & 0x8000) {
		m_isOnceLoop = true;
	}
	if (m_isOnceLoop == true) {
		m_isBlending = false;
		SetintdexAnimationLep(0);
	}
	else {
		m_isOnceLoop = false;
		SetintdexAnimationLep(4);

	}

	if (GetKeyState('4') & 0x8000)
		m_eState = E_STATE_NORMAL;



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

void CharacterClass::Renderstate()
{
	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	// D3DBLENDOP_ADD (�⺻��)	: src + dest
	// D3DBLENDOP_SUBTRACT		: src - dest
	// D3DBLENDOP_REVSUBTRACT	: dest - src
	// D3DBLENDOP_MIN			: MIN(src, dest)
	// D3DBLENDOP_MAX			: MAX(src, dest)
	g_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	// D3DBLEND_ZERO : (0, 0, 0, 0)
	// D3DBLEND_ONE : (1, 1, 1, 1)
	// D3DBLEND_SRCCOLOR : (rs, gs, bs, as)
	// D3DBLEND_INVSRCCOLOR : (1-rs, 1-gs, 1-bs, 1-as)
	// D3DBLEND_SRCALPHA : (as, as, as, as) (Source Blend �⺻��)
	// D3DBLEND_INVSRCALPHA : (1-as, 1-as, 1-as, 1-as) (Destination Blend �⺻��)
	// D3DBLEND_DESTALPHA : (ad, ad, ad, ad)
	// D3DBLEND_INVDESTALPHA : (1-ad, 1-ad, 1-ad, 1-ad)
	// D3DBLEND_DESTCOLOR : (rd, gd, bd, ad)
	// D3DBLEND_INVDESTCCOLOR : (1-rd, 1-gd, 1-bd, 1-ad)
	// D3DBLEND_SRCALPHASAT : (f, f, f, 1); f=min(as, 1-ad)
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);


	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	D3DXMATRIXA16 matWorld, matView;
	D3DXMatrixIdentity(&matWorld);

	//LPDIRECT3DTEXTURE9 p = g_pTextureManager->GetTexture("alpha_tex.tga");
	g_pD3DDevice->SetTexture(0, g_pTextureManager->GetTexture("ThumbnailTexture_4.tga"));
	g_pD3DDevice->SetFVF(ST_PCT_VERTEX::FVF);

	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matWorld, 0, &matView);
	matWorld._41 = 0;
	matWorld._42 = 0;
	matWorld._43 = 1;
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &m_vecVertex1[0], sizeof(ST_PCT_VERTEX));

	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

void CharacterClass::SetintdexAnimationLep(int intdex)
{
	if (m_isBlending) {
		return;
	}

	LPD3DXANIMATIONSET pPrevAnimSet = NULL;
	LPD3DXANIMATIONSET pCurrAnimSet = NULL;
	// ���� �ִϸ��̼��� 1�� Ʈ���� ����
	pAC->GetTrackAnimationSet(0, &pPrevAnimSet);
	pAC->SetTrackAnimationSet(1, pPrevAnimSet);

	D3DXTRACK_DESC stTrackDesc;
	pAC->GetTrackDesc(0, &stTrackDesc);
	pAC->SetTrackDesc(1, &stTrackDesc);

	// ���ο� �ִϸ��̼��� 0�� Ʈ���� ����
	pAC->GetAnimationSet(intdex, &pCurrAnimSet);
	pAC->SetTrackAnimationSet(0, pCurrAnimSet);

	// Ʈ�� ����ġ ����
	pAC->SetTrackWeight(0, 0.0f);
	pAC->SetTrackWeight(1, 1.0f);

	m_fPassedBlendTime = 0.0f;

	m_isBlending = true;

	if (m_isOnceLoop || m_isOnceLoop2) {
		double A = pCurrAnimSet->GetPeriod();
		double B = stTrackDesc.Position;

		if (B / A > 0.9) {
			m_isOnceLoop = false;
			m_isOnceLoop2 = false;
			pAC->SetTrackPosition(0, 0);
			pAC->SetTrackPosition(1, 0);
			m_isBlending = true;
		}
	}
	// Ʈ�� ������ ����
	SAFE_RELEASE(pPrevAnimSet);
	SAFE_RELEASE(pCurrAnimSet);
}

void CharacterClass::isBlending()
{
		m_fPassedBlendTime += g_pTimeManager->GetDeltaTime() + m_fPassedBlendTime;

		if (m_fPassedBlendTime > m_fBlendTime)
		{
			m_isBlending = false;
			pAC->SetTrackWeight(0, 1.0f);
			pAC->SetTrackWeight(1, 0.0f);
			pAC->SetTrackEnable(1, false);

		}
		else
		{
			float fWeight = m_fPassedBlendTime / m_fBlendTime;
			pAC->SetTrackWeight(0, fWeight);
			pAC->SetTrackWeight(1, 1.0f - fWeight);
		}
}

