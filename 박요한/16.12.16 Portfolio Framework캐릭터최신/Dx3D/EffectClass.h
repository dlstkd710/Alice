#pragma once
#include "All_struct.h"

class cObjectXfile;
class CharacterClass;

class EffectClass
{
private:
	cObjectXfile * Rollmesh;
	cObjectXfile * Rollmesh2;
	cObjectXfile * Rollmesh3;
	cObjectXfile * Rollmesh4;
	cObjectXfile * Rollmesh5;
	cObjectXfile * Rollmesh6;
	cObjectXfile * Rollmesh7;
	cObjectXfile * Rollmesh8;
	cObjectXfile * Rollmesh9;
	cObjectXfile * Rollmesh10;
	CharacterClass * m_pCharacter;
	D3DXVECTOR3		Temp;
	D3DXMATRIXA16  matrixtemp;
	float plus;
	float plus2;
	float plus3;
	float plus4;

public:
	EffectClass();
	~EffectClass();

	void SetUpEffectTexture();


	void Render(D3DXMATRIXA16 * trans);

	void ToneRender(D3DXMATRIXA16 * trans);

	void HammerCombo3(D3DXMATRIXA16 * trans);
	void HammerCombo1(D3DXMATRIXA16 * trans);

	void EffectKnife(D3DXVECTOR3 * Position);
};

