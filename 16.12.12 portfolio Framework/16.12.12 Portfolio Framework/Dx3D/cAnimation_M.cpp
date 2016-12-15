#include "StdAfx.h"
#include "cAnimation_M.h"
#include "cAnimation.h"

cAnimation_M::cAnimation_M(void)
{
}


cAnimation_M::~cAnimation_M(void)
{
}

void cAnimation_M::Destroy()
{
	for each(auto it in m_mapSkinnedMesh)
	{
		it.second->Destroy();
		SAFE_DELETE(it.second);
	}
}

cAnimation* cAnimation_M::GetSkinnedMesh(char* szFolder, char* szFilename)
{
	string sFullPath(szFolder);
	sFullPath += std::string(szFilename);

	if (m_mapSkinnedMesh.find(sFullPath) == m_mapSkinnedMesh.end())
	{
		cAnimation* pSkinnedMesh = new cAnimation();
		pSkinnedMesh->Load(szFolder, szFilename);
		m_mapSkinnedMesh[sFullPath] = pSkinnedMesh;
	}
	return m_mapSkinnedMesh[sFullPath];
}
