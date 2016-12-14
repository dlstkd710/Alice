#pragma once

#include "cAction.h"

class cGameObject : public cObject
{
protected:
	D3DXVECTOR3	m_vPrevPosition;
	SYNTHESIZE_ADD_REF(cAction*, m_pAction, Action);
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vPosition, Position);
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vDirection, Direction);
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vScale, Scale);
	SYNTHESIZE(float, m_vRotateX, RotateX);
	SYNTHESIZE(float, m_vRotateY, RotateY);
	SYNTHESIZE(float, m_vRotateZ, RotateZ);
	SYNTHESIZE_PASS_BY_REF(D3DXMATRIXA16, m_matWorld, WorldTM);
public:
	cGameObject(void);
	virtual ~cGameObject(void);

	virtual void Update();
	virtual void Render();
};

