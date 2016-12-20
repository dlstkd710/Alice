#include "stdafx.h"
#include "cEdgeDetection.h"


cEdgeDetection::cEdgeDetection()
{
}

cEdgeDetection::~cEdgeDetection()
{
}

void
cEdgeDetection::Setup()
{
	m_TargetMesh	= NULL;
	m_EdgeEffect	= g_pFxLoader->LoadResource("FxResource/EdgeDetection.fx");
}

void cEdgeDetection::Release()
{
	if (m_TargetMesh)
		m_TargetMesh->Release();

	if (m_EdgeEffect)
		m_EdgeEffect->Release();
}

void cEdgeDetection::Update(/* IN Object Ref*/)
{
	/*
	m_TargetMesh->CloneMeshFVF(Object Ref->GetMesh()->GetOptions(),
							   Object Ref->GetMesh()->GetFVF(),
							   g_pD3DDevice,
							   &m_TargetMesh);
	*/
}

void cEdgeDetection::Render(D3DXMATRIXA16* matViewProj, D3DXMATRIXA16* matWorld)
{
	if (m_TargetMesh)
	{
		m_EdgeEffect->SetMatrix("matViewProjection", matViewProj);
		m_EdgeEffect->SetMatrix("matWorld", matWorld);

		UINT numPass;
		m_EdgeEffect->Begin(&numPass, 0);

		for (UINT i = 0; i < numPass; ++i)
		{
			m_EdgeEffect->BeginPass(i);

			m_TargetMesh->DrawSubset(0);

			m_EdgeEffect->EndPass();
		}

		m_EdgeEffect->End();
	}
}
