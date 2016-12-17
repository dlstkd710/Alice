#pragma once
class Shadow
{
private:
	std::vector<ST_PC_VERTEX>	m_vecVertex;

	LPD3DXMESH gMesh = NULL;
	LPD3DXMESH gBackgroundMesh = NULL;


	LPD3DXEFFECT gShader = NULL;
	LPD3DXEFFECT gCreateShadowShader = NULL;

	//메시 색상
	//그림자 렌더타겟
	LPDIRECT3DTEXTURE9 gShadowRenderTarget = NULL;
	LPDIRECT3DSURFACE9 gShadowDepthStencil = NULL;

	LPDIRECT3DTEXTURE9 gTexture = NULL;


	D3DXVECTOR4 gMeshColor;			
	D3DXVECTOR4 gBackgroundColor;	
	D3DXVECTOR4 gWorldLightPos;		
	D3DXVECTOR4	gWorldCameraPos;	


	int Xpos;
	int Ypos;

public:
	Shadow();
	~Shadow();
	void SetUp();
	void Update();
	bool InitShadowRenderTarget();
	void SetupMatrix(float aspect);
	LPD3DXEFFECT LoadShader(const char * filename);
	bool LoadAsset();
	void RenderScene(D3DXMATRIXA16 * matworld, D3DXVECTOR3 cubePos);
};

