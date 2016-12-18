#pragma once
class Shadow
{
private:

	LPD3DXEFFECT gShader = NULL;
	LPD3DXEFFECT gCreateShadowShader = NULL;

	//메시 색상
	//그림자 렌더타겟
	LPDIRECT3DTEXTURE9 gShadowRenderTarget = NULL;
	LPDIRECT3DSURFACE9 gShadowDepthStencil = NULL;

	D3DXVECTOR4 gMeshColor;
	D3DXVECTOR4 gBackgroundColor;
	D3DXVECTOR4 gWorldLightPos;
	D3DXVECTOR4	gWorldCameraPos;

public:
	Shadow();
	~Shadow();
	void SetUp();
	bool InitShadowRenderTarget();
	void SetupMatrix(float aspect);
	LPD3DXEFFECT LoadShader(const char * filename);
	bool LoadAsset();
	void RenderScene(D3DXMATRIXA16 * matworld, D3DXVECTOR3 cubePos);
};

