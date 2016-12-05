#pragma once
#include "cGameObject.h"

class cCamera;

struct D3DXFRAME_DERIVED;

class cAnimation : public cGameObject
{
	friend class cAnimation_M;

private:

	D3DXFRAME_DERIVED*			m_pRootFrame;
	DWORD						m_dwWorkingPaletteSize;
	D3DXMATRIX*					m_pmWorkingPalette;
	ST_SPHERE					m_stBoundingSphere;

	float blendTime;
	float fpassblendTime;
	bool  isbool;

public:
	LPD3DXANIMATIONCONTROLLER	m_pAnimController;
	LPD3DXEFFECT				m_pEffect;

	cAnimation(char* szFolder, char* szFilename);
	~cAnimation(void);
	void UpdateAndRender(D3DXMATRIXA16* mat = NULL);
	void SetAnimationIndex(int nIndex);
	void SetPosition(D3DXVECTOR3 v)
	{
		m_vPosition = v;
	}
	ST_SPHERE* GetBoundingSphere()
	{
		return &m_stBoundingSphere;
	}
	void				GetAnimController(LPD3DXANIMATIONCONTROLLER* ppAC);

	void SetRandomTrackPosition();

private:
	cAnimation();
	void				Load(char* szFolder, char* szFilename);
	LPD3DXEFFECT		LoadEffect(char* szFilename);
	void				Update(D3DXFRAME_DERIVED* pCurrent, D3DXMATRIXA16* pmatParent);
	void				Render(D3DXFRAME_DERIVED* pBone = NULL);
	void				SetupBoneMatrixPtrs(D3DXFRAME_DERIVED* pBone);
	void				Destroy();
	void				SetIdleKey(bool bResetPosition);
};
