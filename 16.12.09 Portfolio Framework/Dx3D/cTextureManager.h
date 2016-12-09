#pragma once

#define g_pTextureManager cTextureManager::GetInstance()

class cTextureManager
{
private:
	SINGLETONE(cTextureManager);

private:
	std::map<std::string, LPDIRECT3DTEXTURE9> m_mapTexture;

public:
	LPDIRECT3DTEXTURE9 GetTexture(char* szPath);
	LPDIRECT3DTEXTURE9 GetTexture(std::string sPath);
	LPDIRECT3DTEXTURE9 GetTexture(char* szPath, D3DXIMAGE_INFO *m_stImageInfo);
	LPDIRECT3DTEXTURE9 GetTexture(std::string sPath, D3DXIMAGE_INFO *m_stImageInfo);
	void Destroy();
};

