#pragma once

#include <string>
#include <map>
#define g_pFontManager fontManager::GetInstance()

class fontManager
{
private:
	SINGLETONE(fontManager);
private:
	typedef std::map<std::string, LPD3DXFONT>	MAP_FONT;
	typedef MAP_FONT::iterator					MAP_FONT_ITER;
private:
	LPD3DXFONT	m_pDefaultFont;	//����Ʈ ��Ʈ
	MAP_FONT	m_FontStyleMap;	//��Ʈ ��Ÿ�ϸ�
public:
	void Setup();

	void Release();

	void addStyle(std::string style, std::string fontName, float fontSize);

	void PrintText(std::string str, int x, int y, DWORD color);
	void PrintTextShadow(std::string str, int x, int y, DWORD color, DWORD shadow = 0xff000000);
	void PrintTextOutline(std::string str, int x, int y, DWORD color, DWORD outLine = 0xff000000);

	void PrintStyleText(std::string style, std::string str, int x, int y, DWORD color );
	void PrintStyleTextShadow(std::string style, std::string str, int x, int y, DWORD color, DWORD shadow = 0xff000000);
	void PrintStyleTextOutline(std::string style, std::string str, int x, int y, DWORD color, DWORD outLine = 0xff000000);
};


