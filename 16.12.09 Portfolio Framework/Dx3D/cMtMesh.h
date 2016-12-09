#pragma once

#include "cMtAllocate.h"
class cMtMesh :public cGameObject
{
private:
	ST_BONE*					m_pRootBone;
	LPD3DXANIMATIONCONTROLLER	m_pAnimController;
	float						m_fBlendTime;
	float						m_fPassedBlendTime;
	bool						m_isBlending;

public:
	cMtMesh(void);
	~cMtMesh(void);

	void Load(std::string sFolder, std::string sFile);
	void Update();
	void Render();
	void SetAnimationIndex(int n);

private:
	void Render(ST_BONE* pBone);
	void Update(ST_BONE* pBone, D3DXMATRIX* pParent);
	void UpdateSkinnedMesh(ST_BONE* pBone);
	void SetupBoneMatrixPtrs(ST_BONE* pBone);
};

