#include "stdafx.h"
#include "CharacterClass.h"
#include "iMap.h"
#include "cAnimation.h"
#include "cAllocateHierarchy.h"
#include "Shadow.h"
#include "EffectClass.h"

#define RADIUS 0.3f
#define DELTA_Y 0.1f

CharacterClass::CharacterClass()
	: m_fSpeed(100.f)
	, m_vDirection1(1, 0, 0)
	, n(0)
	, a(0)
	, t(0)
	, r(0)
	, i(0)
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
	, d_1(0.f)
	, m_pEffectClass(NULL)
{

}
CharacterClass::~CharacterClass()
{
}

void CharacterClass::SetUp()
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&matR);
	D3DXMatrixIdentity(&matT);


	_Animation = new cAnimation("./Alice48/", "AliceW.x");
	_Knife = new cAnimation("./Knife/", "Kife.X");
	_Gun = new cAnimation("./Gun/", "Gun.X");
	_Hammor = new cAnimation("./weapon/", "Hammor.X");
	m_pEffectClass = new EffectClass;
	m_pEffectClass->SetUpEffectTexture();



	D3DXMatrixRotationY(&matR, 각도(180.0f));

	_Animation->SetAnimationIndex(21);
	_Gun->SetAnimationIndex(1);

	_Animation->SetPosition(D3DXVECTOR3(0, 0, 0));

	_Knife->SetPosition(D3DXVECTOR3(0, 0, 0));
	_Gun->SetPosition(D3DXVECTOR3(0, 0, 0));
	
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
	case E_STATE_TELPO:
		UpdateTelpo(pMap);
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
	case E_STATE_HAMMOR:
		UpdateHammor(pMap);
		break;
	case E_STATE_H1COMBO:
		UpdateH1combo(pMap);
		break;
	case E_STATE_H2COMBO:
		UpdateH2combo(pMap);
		break;
	case E_STATE_H3COMBO:
		UpdateH3combo(pMap);
		break;
	case E_STATE_GRUN:
		UpdateGunRunMotion(pMap);
		break;
	}
}

void CharacterClass::Controll(iMap* pMap, float CameraAngleX, float CameraAngleY /*= NULL*/)
{
	_Animation->GetAnimController(&pAC);

	MotionControllTower(pMap); // 캐릭터 모션 정리 함수

	isBlending();

	D3DXMatrixTranslation(&matT, m_vCubePos.x, m_vCubePos.y, m_vCubePos.z);

	if (GetKeyState('W') & 0x8000) {
		D3DXMatrixRotationY(&matR, 각도(-180.0f) + CameraAngleY);
		m_vCubeDir = D3DXVECTOR3(0, 0, -1);
	}
	if (GetKeyState('S') & 0x8000) {
		D3DXMatrixRotationY(&matR, 각도(-1.0f) + CameraAngleY);
		m_vCubeDir = D3DXVECTOR3(0, 0, 1);
	}
	if (GetKeyState('A') & 0x8000) {
		D3DXMatrixRotationY(&matR, 각도(90.0f) + CameraAngleY);
		m_vCubeDir = D3DXVECTOR3(0, 0, 1);
	}
	if (GetKeyState('D') & 0x8000) {
		D3DXMatrixRotationY(&matR, 각도(-90.0f) + CameraAngleY);
		m_vCubeDir = D3DXVECTOR3(0, 0, -1);
	}

	if ((GetKeyState('A') & 0x8000) && (GetKeyState('W') & 0x8000)) {
		D3DXMatrixRotationY(&matR, 각도(130.0f) + CameraAngleY);
		m_vCubeDir = D3DXVECTOR3(0, 0, -1);
	}
	if ((GetKeyState('W') & 0x8000) && (GetKeyState('D') & 0x8000)) {
		D3DXMatrixRotationY(&matR, 각도(-135.0f) + CameraAngleY);
	}

	if ((GetKeyState('A') & 0x8000) && (GetKeyState('S') & 0x8000)) {
		D3DXMatrixRotationY(&matR, 각도(45.0f) + CameraAngleY);
	}

	if ((GetKeyState('S') & 0x8000) && (GetKeyState('D') & 0x8000)) {
		D3DXMatrixRotationY(&matR, 각도(-45.0f) + CameraAngleY);
		m_vCubeDir = D3DXVECTOR3(0, 0, 1);
	}


	//==============================================================
	D3DXMATRIXA16 matS;
	D3DXMatrixScaling(&matS, 0.3, 0.3, 0.3);

	if (g_pkeyManager->isStayKeyDown(VK_LCONTROL)) {
		m_matWorld = matS * matR * matT;
	}
	else {
		m_matWorld = matR * matT;
	}

	//============================================================

	D3DXVec3TransformCoord(&m_vCubeDir, &m_vCubeDir, &matR);


}

void CharacterClass::Render()
{
	_Animation->UpdateAndRender(&m_matWorld);

	if (m_eState == E_STATE_JUMPDOUBLE) {

		m_pEffectClass->ToneRender(&m_matWorld);
	}

	if (m_eState == E_STATE_JUMPDOWN) {
		if (g_pkeyManager->isStayKeyDown(VK_SPACE)) {
			m_pEffectClass->Render(&m_matWorld);
		}
	}

	if (m_eState == E_STATE_HAMMOR  ||
		m_eState == E_STATE_H1COMBO ||
		m_eState == E_STATE_H2COMBO ||
		m_eState == E_STATE_H3COMBO) {
		D3DXFRAME_DERIVED * tempBone2 = _Animation->GetFineBONE("Bip01-Prop1");

		D3DXMATRIXA16 tempMat2 = tempBone2->CombinedTransformationMatrix;

		D3DXFRAME_DERIVED* Hframe = _Gun->m_pRootFrame;
		// 빈칼은 칼의 전체
		D3DXMATRIXA16 HLocalMat = Hframe->TransformationMatrix;
		//칼의로컬 매트릭는 칼의 변형 매트릭스
		D3DXMATRIXA16 roteX1, roteY1, matT;
		D3DXMatrixIdentity(&roteX1);
		D3DXMatrixIdentity(&roteY1);
		D3DXMatrixIdentity(&matT);
		// 칼을 초기화 하고 돌려
		D3DXMatrixRotationX(&roteY1, 각도(360.0f));
		HLocalMat = roteY1;
		//==================
		D3DXMATRIXA16 matFinal1 = HLocalMat * tempMat2;
		// 마지막메트릭스는 = 칼의 로컬 메트릭스랑 뼈의 메트릭스 곱하기
		_Hammor->UpdateAndRender(&matFinal1);

	}

	if (m_eState == E_STATE_GIDEL)
	{
		D3DXFRAME_DERIVED * tempBone1 = _Animation->GetFineBONE("Bip01-Prop2");

		D3DXMATRIXA16 tempMat1 = tempBone1->CombinedTransformationMatrix;

		D3DXFRAME_DERIVED* GunLocal = _Gun->m_pRootFrame;
		// 빈칼은 칼의 전체
		D3DXMATRIXA16 GunLocalMat = GunLocal->TransformationMatrix;
		//칼의로컬 매트릭는 칼의 변형 매트릭스
		D3DXMATRIXA16 roteX1, roteY1, matT;
		D3DXMatrixIdentity(&roteX1);
		D3DXMatrixIdentity(&roteY1);
		D3DXMatrixIdentity(&matT);
		// 칼을 초기화 하고 돌려
		D3DXMatrixRotationY(&roteY1, 각도(280.0f));
		D3DXMatrixRotationX(&roteX1, 각도(-85.0f));
		D3DXMatrixTranslation(&matT, 0.4, -2.4, 0);
		GunLocalMat = roteY1 * roteX1 * matT;
		//==================
		D3DXMATRIXA16 matFinal1 = GunLocalMat * tempMat1;
		// 마지막메트릭스는 = 칼의 로컬 메트릭스랑 뼈의 메트릭스 곱하기

		_Gun->UpdateAndRender(&matFinal1);

	}
	if (m_eState == E_STATE_GRUN)
	{
		D3DXFRAME_DERIVED * tempBone1 = _Animation->GetFineBONE("Bip01-Prop2");

		D3DXMATRIXA16 tempMat1 = tempBone1->CombinedTransformationMatrix;

		D3DXFRAME_DERIVED* GunLocal = _Gun->m_pRootFrame;
		// 빈칼은 칼의 전체
		D3DXMATRIXA16 GunLocalMat = GunLocal->TransformationMatrix;
		//칼의로컬 매트릭는 칼의 변형 매트릭스
		D3DXMATRIXA16 roteX1, roteY1, matT;
		D3DXMatrixIdentity(&roteX1);
		D3DXMatrixIdentity(&roteY1);
		D3DXMatrixIdentity(&matT);
		// 칼을 초기화 하고 돌려
		D3DXMatrixRotationX(&roteY1, 각도(270.f));
		//D3DXMatrixRotationX(&roteX1, 각도(-85.0f));
		D3DXMatrixTranslation(&matT, -2.1, -0.9, 0.6);
		GunLocalMat = roteY1 * roteX1 * matT;
		//==================
		D3DXMATRIXA16 matFinal1 = GunLocalMat * tempMat1;
		// 마지막메트릭스는 = 칼의 로컬 메트릭스랑 뼈의 메트릭스 곱하기

		_Gun->UpdateAndRender(&matFinal1);
		// 칼그리기
	}
	if (m_eState == E_STATE_IDEL ||
		m_eState == E_STATE_1COMBO ||
		m_eState == E_STATE_2COMBO ||
		m_eState == E_STATE_3COMBO)
	{
		D3DXFRAME_DERIVED* tempBone = _Animation->GetFineBONE("Bip01-Prop1");
		//뼈를 찾아서 빈뼈에 넣어라
		D3DXMATRIXA16 tempMat = tempBone->CombinedTransformationMatrix;
		// 빈뼈의 컨바인 메트릭스는 빈 메트릭스에 넣어라
		D3DXFRAME_DERIVED* knifeLocal = _Knife->m_pRootFrame;
		// 빈칼은 칼의 전체
		D3DXMATRIXA16 knifeLocalMat = knifeLocal->TransformationMatrix;
		//칼의로컬 매트릭는 칼의 변형 매트릭스
		D3DXMATRIXA16 roteX;
		D3DXMatrixIdentity(&roteX);
		// 칼을 초기화 하고 돌려라
		D3DXMatrixRotationX(&roteX, 각도(180.0f));

		knifeLocalMat = roteX;
		// 칼의 로컬 매트릭슨 = 로테이션에x 에 대한 각도
		D3DXMATRIXA16 matFinal = knifeLocalMat * tempMat;
		// 마지막메트릭스는 = 칼의 로컬 메트릭스랑 뼈의 메트릭스 곱하기
		_Knife->UpdateAndRender(&matFinal);		// 칼그리기
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
		SetintdexAnimationLep(21);

	if (g_pkeyManager->isOnceKeyDown(VK_SPACE)) {
		m_fVY = 0.6f;	//점프 높이
		m_eState = E_STATE_JUMPUP;
		_Animation->SetAnimationIndex(20);
		return;
	}
	if (GetKeyState('1') & 0x8000) {
		m_fVY = 0.5f;	//점프 높이
		m_eState = E_STATE_IDEL;
		SetintdexAnimationLep(13);
		return;
	}
	if (GetKeyState('2') & 0x8000) {
		m_eState = E_STATE_GIDEL;
		SetintdexAnimationLep(16);
		return;
	}
	if (GetKeyState('3') & 0x8000) {
		m_eState = E_STATE_HAMMOR;
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

void CharacterClass::UpdateJumpUp(iMap* pMap /*= NULL*/)
{
	if (g_pkeyManager->isOnceKeyDown(VK_SPACE)) {
		m_fVY = 0.5f;	//점프 높이
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
			m_fVY = 0.5f;	//점프 높
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
	if (g_pkeyManager->isStayKeyDown(VK_SPACE)) {
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

	if (pMap == NULL) {
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

		if (GetKeyState('W') & 0x8000)
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
	m_isBlending = false;
	m_isOnceLoop = true;
	if (g_pkeyManager->isOnceKeyDown(VK_LBUTTON)) {
		if (d.size() < 1)
			d.push_back(1);
		if (!d.empty()) {
			if (d.size() > d_1) {
				if (i < 2) {
					if (i == 1)
						combo.push(E_STATE_2COMBO);
					i++;
				}
			}
		}
	}
	SetintdexAnimationLep(16); //1타
}

void CharacterClass::Update2combo(iMap * pMap /*= NULL*/)
{
	m_isBlending = false;
	m_isOnceLoop = true;
	if (g_pkeyManager->isOnceKeyDown(VK_LBUTTON)) {
		if (d.size() < 1)
			d.push_back(1);
		if (!d.empty()) {
			if (d.size() > d_1) {
				if (i < 3) {
					if (i == 2)
						combo.push(E_STATE_3COMBO);
					i++;
				}
			}
		}
	}

	SetintdexAnimationLep(15); //1타
}

void CharacterClass::Update3combo(iMap * pMap /*= NULL*/)
{
	m_isBlending = false;
	m_isOnceLoop = true;
	SetintdexAnimationLep(14); //1타
}

void CharacterClass::UpdatefRunAndIdel(iMap * pMap)
{
	if (!d.empty())
		d_1 += 0.07;
	if (d.size() < d_1) {
		d.clear(); d_1 = 0;
	}
	if (d.empty()) {
		if (!combo.empty()) {
			m_eState = combo.front();
		}
	}

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
	if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
		m_fVY = 0.5f;	//점프 높이
		m_eState = E_STATE_JUMPUP;
		_Animation->SetAnimationIndex(20);
		return;
	}


	if (g_pkeyManager->isOnceKeyDown(VK_LBUTTON)) {
		if (d.size() < 1)
			d.push_back(1);
		if (!d.empty()) {
			if (d.size() > d_1) {
				if (i < 1) {
					if (i == 0)
						combo.push(E_STATE_1COMBO);
					//if (i == 1)
					//	combo.push(E_STATE_2COMBO);
					//if (i == 2)
					//	combo.push(E_STATE_3COMBO);
					i++;
				}
			}
		}
	}

	if (GetKeyState('4') & 0x8000) {
		m_eState = E_STATE_NORMAL;
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
		m_eState = E_STATE_GRUN;
	}
	else if (GetKeyState('S') & 0x8000) {
		m_eState = E_STATE_GRUN;
	}
	else if (GetKeyState('A') & 0x8000) {
		m_eState = E_STATE_GRUN;
	}
	else if (GetKeyState('D') & 0x8000) {
		m_eState = E_STATE_GRUN;
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
		m_fVY = 0.5f;	//점프 높이
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
		else {
			m_vCubePos = vTempPos;
		}
	}
	if (pMap == NULL) {
		m_vCubePos = vTempPos;
	}
}

void CharacterClass::UpdateGunRunMotion(iMap * pMap)
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
	else {
		SetintdexAnimationLep(6);
		m_eState = E_STATE_GIDEL;
	}

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		m_fVY = 0.5f;	//점프 높이
		m_eState = E_STATE_JUMPUP;
		_Animation->SetAnimationIndex(20);
		return;
	}
}

void CharacterClass::UpdateHammor(iMap * pMap)
{
	if (!d.empty())
		d_1 += 0.07;
	if (d.size() < d_1) {
		d.clear(); d_1 = 0;
	}
	if (d.empty()) {
		if (!combo.empty()) {
			m_eState = combo.front();
		}
	}
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
	else {
		SetintdexAnimationLep(4);
	}
	if (g_pkeyManager->isOnceKeyDown(VK_LBUTTON)) {
		if (d.size() < 1)
			d.push_back(1);
		if (!d.empty()) {
			if (d.size() > d_1) {
				if (i < 1) {
					if (i == 0)
						combo.push(E_STATE_H1COMBO);
					i++;
				}
			}
		}
	}


	if (GetKeyState('4') & 0x8000)
		m_eState = E_STATE_NORMAL;



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

void CharacterClass::UpdateH1combo(iMap * pMap)
{
	m_isBlending = false;
	m_isOnceLoop = true;
	if (g_pkeyManager->isOnceKeyDown(VK_LBUTTON)) {
		if (d.size() < 1)
			d.push_back(1);
		if (!d.empty()) {
			if (d.size() > d_1) {
				if (i < 2) {
					if (i == 1)
						combo.push(E_STATE_H2COMBO);
					i++;
				}
			}
		}
	}
	SetintdexAnimationLep(2); //1타
}

void CharacterClass::UpdateH2combo(iMap * pMap)
{
	m_isBlending = false;
	m_isOnceLoop = true;
	if (g_pkeyManager->isOnceKeyDown(VK_LBUTTON)) {
		if (d.size() < 1)
			d.push_back(1);
		if (!d.empty()) {
			if (d.size() > d_1) {
				if (i < 3) {
					if (i == 2)
						combo.push(E_STATE_H3COMBO);
					i++;
				}
			}
		}
	}

	SetintdexAnimationLep(1); //1타
}

void CharacterClass::UpdateH3combo(iMap * pMap)
{
	m_isBlending = false;
	m_isOnceLoop = true;
	SetintdexAnimationLep(0); //1타
}

void CharacterClass::Renderstate()
{

}

void CharacterClass::SetintdexAnimationLep(int intdex)
{
	if (m_isBlending) {
		return;
	}
	LPD3DXANIMATIONSET pPrevAnimSet = NULL;
	LPD3DXANIMATIONSET pCurrAnimSet = NULL;


	D3DXTRACK_DESC stTrackDesc;
	if (pCurrAnimSet == pPrevAnimSet) {


		// 기존 애니매이션을 1번 트랙에 세팅
		pAC->GetTrackAnimationSet(0, &pPrevAnimSet);
		pAC->SetTrackAnimationSet(1, pPrevAnimSet);

		pAC->GetTrackDesc(0, &stTrackDesc);
		pAC->SetTrackDesc(1, &stTrackDesc);

		// 새로운 애니매이션을 0번 트랙에 세팅
		pAC->GetAnimationSet(intdex, &pCurrAnimSet);
		pAC->SetTrackAnimationSet(0, pCurrAnimSet);

		// 트랙 가중치 설정
		pAC->SetTrackWeight(0, 0.0f);
		pAC->SetTrackWeight(1, 1.0f);

		m_fPassedBlendTime = 0.0f;

		m_isBlending = true;

		if (m_isOnceLoop) {
			if (r != 3) {
				pAC->SetTrackPosition(0, 0);
				pAC->SetTrackPosition(1, 0);
				r = 3;
			}
			pAC->GetTrackDesc(0, &stTrackDesc);

			double A = pCurrAnimSet->GetPeriod();

			double B = stTrackDesc.Position;

			double AB = B / A;

			if (AB >= 0.95) {
				r = 0;
				pAC->SetTrackPosition(0, 0);
				pAC->SetTrackPosition(1, 0);
				m_isOnceLoop = false;
				m_isBlending = false;
				pAC->SetTrackEnable(1, false);
				if (!combo.empty()) {
					combo.pop();
					if (!combo.empty())
						m_eState = combo.front();
				}
				if (combo.empty()) {
					if (m_eState == 5 || m_eState == 6 || m_eState == 7)
						m_eState = E_STATE_IDEL; i = 0;
					if (m_eState == 11 || m_eState == 12 || m_eState == 13)
						m_eState = E_STATE_HAMMOR; i = 0;

				}
			}
		}
		// 트랙 포지션 설정
		SAFE_RELEASE(pPrevAnimSet);
		SAFE_RELEASE(pCurrAnimSet);
	}
	else {
		pAC->SetTrackPosition(0, 0);
		pAC->SetTrackPosition(1, 0);
		SetintdexAnimationLep(intdex);
	}

}

void CharacterClass::isBlending()
{
	m_fPassedBlendTime += g_pTimeManager->GetDeltaTime() * 1.5;

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

