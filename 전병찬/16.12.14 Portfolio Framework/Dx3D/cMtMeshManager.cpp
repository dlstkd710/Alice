#include "StdAfx.h"
#include "cMtMeshManager.h"
#include "cMtMesh.h"

cMtMeshManager::cMtMeshManager(void)
{
}

cMtMeshManager::~cMtMeshManager(void)
{
}

void cMtMeshManager::Destroy()
{
	for each(auto it in m_mapSkinnedMesh)
	{
		it.second->Destroy();
		SAFE_DELETE(it.second);
	}
}

cMtMesh* cMtMeshManager::GetSkinnedMesh(char* szFolder, char* szFilename)
{
	std::string sFullPath(szFolder);
	sFullPath += std::string(szFilename);

	if (m_mapSkinnedMesh.find(sFullPath) == m_mapSkinnedMesh.end())
	{
		cMtMesh* pSkinnedMesh = new cMtMesh();
		pSkinnedMesh->Load(szFolder, szFilename);
		m_mapSkinnedMesh[sFullPath] = pSkinnedMesh;
	}
	return m_mapSkinnedMesh[sFullPath];
}
