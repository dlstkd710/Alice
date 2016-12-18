#pragma once

struct ST_BONE;

class cMtMesh : public cGameObject
{
	friend class cMtMeshManager;

private:

	//�ϳ��� ����
	ST_BONE*					m_pRootFrame;
	DWORD						m_dwWorkingPaletteSize;
	D3DXMATRIX*					m_pmWorkingPalette;
	LPD3DXEFFECT				m_pEffect;
	ST_SPHERE					m_stBoundingSphere;

	// ��ü���� ����
	LPD3DXANIMATIONCONTROLLER	m_pAnimController;
	//D3DXVECTOR3					m_vPosition;
	SYNTHESIZE(D3DXVECTOR3, m_Min, Min);
	SYNTHESIZE(D3DXVECTOR3, m_Max, Max);

	float Tweight0, Tweight1;

public:
	cMtMesh(char* szFolder, char* szFilename);
	~cMtMesh(void);

	void UpdateAndRender(D3DXMATRIXA16* pmat = NULL);
	void SetAnimationIndex(int nIndex);

	void SetRandomTrackPosition(); // �׽�Ʈ��
	void SetPosition(D3DXVECTOR3 v)
	{
		m_vPosition = v;
		m_stBoundingSphere.vCenter += v;
	}
	ST_SPHERE* GetBoundingSphere()
	{
		return &m_stBoundingSphere;
	}
private:
	cMtMesh();
	void Load(char* szFolder, char* szFilename);
	LPD3DXEFFECT LoadEffect(char* szFilename);
	void Update(ST_BONE* pCurrent, D3DXMATRIXA16* pmatParent);
	void Render(ST_BONE* pBone = NULL);
	void SetupBoneMatrixPtrs(ST_BONE* pBone);
	void Destroy();
	void SettingBlend();
	void UpdateAnimation();
};

