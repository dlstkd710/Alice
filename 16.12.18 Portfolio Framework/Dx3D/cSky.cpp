#include "stdafx.h"
#include "cSky.h"

cSky::cSky()
	: m_pSkyTexture(NULL), m_pSkyMesh(NULL), m_pSkyEffect(NULL)
{
}

cSky::~cSky()
{
}

HRESULT cSky::Init()
{
	/* Texture Load */
	D3DXCreateCubeTextureFromFile(g_pD3DDevice,
								  "FxResource/CubeMap_BlackNight.dds",
								  &m_pSkyTexture);

	if (m_pSkyTexture == NULL)
	{
		assert(m_pSkyTexture != NULL && " Failed Sky Texture load ");
	}

	/* Create Mesh */
	D3DXCreateSphere(g_pD3DDevice,
					 100.f,
					 (UINT)50,
					 (UINT)50,
					 &m_pSkyMesh,
					 NULL);

	if (m_pSkyMesh == NULL)
	{
		assert(m_pSkyMesh != NULL && " Failed Sky Texture load ");
	}

	/* Effect Load */
	m_pSkyEffect = g_pFxLoader->LoadResource("FxResource/EnvironmentCUBE.fx");

	return S_OK;
}

void cSky::Release()
{
	if (m_pSkyTexture)
		m_pSkyTexture->Release();
	if (m_pSkyMesh)
		m_pSkyMesh->Release();
	if (m_pSkyEffect)
		m_pSkyEffect->Release();
}

void cSky::Update()
{

}

void cSky::Render(D3DXMATRIXA16* matWorld, D3DXMATRIXA16* matViewProj)
{
	if (m_pSkyMesh && m_pSkyTexture)
	{
		m_pSkyEffect->SetTexture("MyCube_Tex", m_pSkyTexture);
	}

	D3DXMATRIXA16 matWVP, matW;
	matW = *matWorld;
	matWVP = *matWorld * *matViewProj;
	
	m_pSkyEffect->SetMatrix("matWVP", &matWVP);
	m_pSkyEffect->SetMatrix("matWorld", &matW);
	
	/* Render */
	UINT numPass;
	m_pSkyEffect->Begin(&numPass, 0);			// Number of pass
	
	for (UINT i = 0; i < numPass; ++i)
	{
		m_pSkyEffect->BeginPass(i);
		
		m_pSkyMesh->DrawSubset(0);
	
		m_pSkyEffect->EndPass();
	}
	m_pSkyEffect->End();
}
