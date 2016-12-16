#pragma once
#include "stdafx.h"

class cAnimation;

using namespace std;
class cAnimation_M
{
private:

	map<string, cAnimation*> m_mapSkinnedMesh;

public:
	SINGLETONE(cAnimation_M);

	cAnimation* GetSkinnedMesh(char* szFolder, char* szFilename);

	void Destroy();

};

#define Animation_M cAnimation_M::GetInstance()


