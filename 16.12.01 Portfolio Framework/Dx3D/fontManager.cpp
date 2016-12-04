#include "stdafx.h"
#include "fontManager.h"


fontManager::fontManager()
	:m_pDefaultFont(NULL)
{
}


fontManager::~fontManager()
{
}

void fontManager::Setup()
{
	//폰트 객체 만든다
	D3DXCreateFont(
		g_pD3DDevice,			//디바이스
		(INT)30,				//폰트의 높이
		(UINT)0.f,				//폰트의 폭(이 값이 0이면 앞에서 설정한 폰트 높이에 비례)
		FW_BOLD,			//폰트의 두께 설정(FW_BOLD 폰트의 BOLD 스타일 크기를 갖는다)
		1,					//밉맵(그냥 1)
		FALSE,				//이텔릭 여부
		DEFAULT_CHARSET,	//캐릭터 set( 그냥 DEFAULT_CHARSET )
		OUT_DEFAULT_PRECIS,	//외각 처리방식 ( 그냥 OUT_DEFAULT_PRECIS )
		DEFAULT_QUALITY,	//퀄리티 ( 그냥 DEFAULT_QUALITY )
		DEFAULT_PITCH | FF_DONTCARE,// 글씨체 패밀리셋 설정
		"Bitstream Vera Sans Mono",			//사용될 폰트 이름 
		&m_pDefaultFont			//생성된 폰트객체 받을 더블 포인터
	);
}

void fontManager::Release()
{
	SAFE_RELEASE(m_pDefaultFont);

	MAP_FONT_ITER iter;
	for (iter = m_FontStyleMap.begin(); iter != m_FontStyleMap.end(); ++iter)
	{
		SAFE_RELEASE(iter->second);
	}
}

void fontManager::addStyle(std::string style, std::string fontName, float fontSize)
{
	MAP_FONT_ITER find = m_FontStyleMap.find(style);
	if (find != m_FontStyleMap.end())
		return;
	
	LPD3DXFONT newFont;
	D3DXCreateFont(
		g_pD3DDevice,						//디바이스
		(INT)fontSize,						//폰트의 높이
		(UINT)0.f,							//폰트의 폭(이 값이 0이면 앞에서 설정한 폰트 높이에 비례)
		FW_BOLD,						//폰트의 두께 설정(FW_BOLD 폰트의 BOLD 스타일 크기를 갖는다)
		1,								//밉맵(그냥 1)
		FALSE,							//이텔릭 여부
		DEFAULT_CHARSET,				//캐릭터 set( 그냥 DEFAULT_CHARSET )
		OUT_DEFAULT_PRECIS,				//외각 처리방식 ( 그냥 OUT_DEFAULT_PRECIS )
		DEFAULT_QUALITY,				//퀄리티 ( 그냥 DEFAULT_QUALITY )
		DEFAULT_PITCH | FF_DONTCARE,	// 글씨체 패밀리셋 설정
		fontName.c_str(),				//사용될 폰트 이름 
		&newFont						//생성된 폰트객체 받을 더블 포인터
	);

	m_FontStyleMap.insert(std::make_pair(style, newFont));

}

void fontManager::PrintText(std::string str, int x, int y, DWORD color)
{
	//출력 영역
	RECT rc = { x, y, 0, 0 };

	m_pDefaultFont->DrawText(
		NULL,
		str.c_str(),
		-1,
		&rc,
		DT_LEFT | DT_NOCLIP,
		color);
}

void fontManager::PrintTextShadow(std::string str, int x, int y, DWORD color, DWORD shadow)
{
	//쉐도우 먼저 그린다
	PrintText(str, x + 1, y + 1, shadow);

	PrintText(str, x, y, color);
}

void fontManager::PrintTextOutline(std::string str, int x, int y, DWORD color, DWORD outLine)
{
	//외각 찍는다
	PrintText(str, x + 1, y, outLine);
	PrintText(str, x - 1, y, outLine);
	PrintText(str, x, y + 1, outLine);
	PrintText(str, x, y - 1, outLine);

	//본래 Text 찍는다
	PrintText(str, x, y, color);
}

void fontManager::PrintStyleText(std::string style, std::string str, int x, int y, DWORD color)
{
	LPD3DXFONT pFont = NULL;

	MAP_FONT_ITER find = m_FontStyleMap.find(style);

	if (find == m_FontStyleMap.end()) {
		pFont = m_pDefaultFont;
	}
	else {
		pFont = find->second;
	}

	RECT rc = { x, y, 0, 0 };

	pFont->DrawText(
		NULL,
		str.c_str(),
		-1,
		&rc,
		DT_LEFT | DT_NOCLIP,
		color);
}

void fontManager::PrintStyleTextShadow(std::string style, std::string str, int x, int y, DWORD color, DWORD shadow)
{
	PrintStyleText(style, str, x + 1, y + 1, shadow);

	PrintStyleText(style, str, x, y, color);
}

void fontManager::PrintStyleTextOutline(std::string style, std::string str, int x, int y, DWORD color, DWORD outLine)
{
	PrintStyleText(style, str, x + 1, y, outLine);
	PrintStyleText(style, str, x - 1, y, outLine);
	PrintStyleText(style, str, x, y + 1, outLine);
	PrintStyleText(style, str, x, y - 1, outLine);

	PrintStyleText(style, str, x, y, color);
}
