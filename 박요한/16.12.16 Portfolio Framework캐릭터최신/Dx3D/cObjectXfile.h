#pragma once

#include "cObjectAllocate.h"

class cObjectXfile : public cGameObject
{
private:
	ST_BONE*					m_pRootBone;
	LPD3DXANIMATIONCONTROLLER	m_pAnimController;
	SYNTHESIZE(D3DXVECTOR3, pMin, Min);
	SYNTHESIZE(D3DXVECTOR3, pMax, Max);
public:
	cObjectXfile(void);
	~cObjectXfile(void);

	void Load(std::string sFolder, std::string sFile);
	void Update();
	void UpdateMatrix(D3DXMATRIXA16* pParent);
	void Render();
	void SetAnimationIndex(int n);

private:
	void Render(ST_BONE* pBone);
	void Render(ST_BONE * pBone, D3DXMATRIXA16 * trans);
	void Update(ST_BONE* pBone, D3DXMATRIX* pParent);
	void UpdateSkinnedMesh(ST_BONE* pBone);
	void SetupBoneMatrixPtrs(ST_BONE* pBone);
};
