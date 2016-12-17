#pragma once

#define g_pFxLoader cFxLoader::GetInstance()

class cFxLoader
{
private:
	SINGLETONE(cFxLoader);

public:
	LPD3DXEFFECT LoadResource(std::string filePath);
};