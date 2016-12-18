#pragma once

#include "cObjectAllocate.h"

class cOBB;

class cObjectXfile : public cGameObject
{
private:
	ST_BONE*					m_pRootBone;
	LPD3DXANIMATIONCONTROLLER	m_pAnimController;
	SYNTHESIZE(D3DXVECTOR3, o_Min, oMin);
	SYNTHESIZE(D3DXVECTOR3, o_Max, oMax);
	SYNTHESIZE(bool, o_collision, collision);

	cOBB* o_OBB;
	cOBB* o_OBB2;
	cOBB* o_OBB3;
	cOBB* o_OBB4;
	cOBB* o_OBB5;

	D3DXMATRIXA16 obb4mat;
public:
	cObjectXfile(void);
	~cObjectXfile(void);

	void Load(std::string sFolder, std::string sFile, int Num);
	void Load(std::string sFolder, std::string sFile);
	void LoadObb(int PNum);
	void UpdateMatrix(D3DXMATRIXA16* pParent);
	void Update();
	void Render();
	void SetAnimationIndex(int n);

private:
	void Render(ST_BONE* pBone);
	void Render(ST_BONE * pBone, D3DXMATRIXA16 * trans);
	void Update(ST_BONE* pBone, D3DXMATRIX* pParent);
	void UpdateSkinnedMesh(ST_BONE* pBone);
	void SetupBoneMatrixPtrs(ST_BONE* pBone);
};
