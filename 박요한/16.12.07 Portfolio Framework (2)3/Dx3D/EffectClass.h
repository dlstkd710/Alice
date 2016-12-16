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

	CharacterClass * m_pCharacter;
	D3DXVECTOR3		Temp;
	D3DXMATRIXA16  matrixtemp;
	float plus;
	float plus2;
	float plus3;

public:
	EffectClass();
	~EffectClass();

	void SetUpEffectTexture();


	void Render(D3DXMATRIXA16 * trans);

	void ToneRender(D3DXMATRIXA16 * trans);
};

