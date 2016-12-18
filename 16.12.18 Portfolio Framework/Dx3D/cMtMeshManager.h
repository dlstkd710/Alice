#pragma once

#define g_pMtMeshManager cMtMeshManager::GetInstance()

class cMtMesh;

class cMtMeshManager
{
private:
	std::map<std::string, cMtMesh*> m_mapSkinnedMesh;

public:
	SINGLETONE(cMtMeshManager);

	cMtMesh* GetSkinnedMesh(char* szFolder, char* szFilename);

	void Destroy();
};

