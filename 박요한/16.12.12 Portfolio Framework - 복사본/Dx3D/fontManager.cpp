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
	//��Ʈ ��ü �����
	D3DXCreateFont(
		g_pD3DDevice,			//����̽�
		(INT)30,				//��Ʈ�� ����
		(UINT)0.f,				//��Ʈ�� ��(�� ���� 0�̸� �տ��� ������ ��Ʈ ���̿� ���)
		FW_BOLD,			//��Ʈ�� �β� ����(FW_BOLD ��Ʈ�� BOLD ��Ÿ�� ũ�⸦ ���´�)
		1,					//�Ӹ�(�׳� 1)
		FALSE,				//���ڸ� ����
		DEFAULT_CHARSET,	//ĳ���� set( �׳� DEFAULT_CHARSET )
		OUT_DEFAULT_PRECIS,	//�ܰ� ó����� ( �׳� OUT_DEFAULT_PRECIS )
		DEFAULT_QUALITY,	//����Ƽ ( �׳� DEFAULT_QUALITY )
		DEFAULT_PITCH | FF_DONTCARE,// �۾�ü �йи��� ����
		"Bitstream Vera Sans Mono",			//���� ��Ʈ �̸� 
		&m_pDefaultFont			//������ ��Ʈ��ü ���� ���� ������
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
		g_pD3DDevice,						//����̽�
		(INT)fontSize,						//��Ʈ�� ����
		(UINT)0.f,							//��Ʈ�� ��(�� ���� 0�̸� �տ��� ������ ��Ʈ ���̿� ���)
		FW_BOLD,						//��Ʈ�� �β� ����(FW_BOLD ��Ʈ�� BOLD ��Ÿ�� ũ�⸦ ���´�)
		1,								//�Ӹ�(�׳� 1)
		FALSE,							//���ڸ� ����
		DEFAULT_CHARSET,				//ĳ���� set( �׳� DEFAULT_CHARSET )
		OUT_DEFAULT_PRECIS,				//�ܰ� ó����� ( �׳� OUT_DEFAULT_PRECIS )
		DEFAULT_QUALITY,				//����Ƽ ( �׳� DEFAULT_QUALITY )
		DEFAULT_PITCH | FF_DONTCARE,	// �۾�ü �йи��� ����
		fontName.c_str(),				//���� ��Ʈ �̸� 
		&newFont						//������ ��Ʈ��ü ���� ���� ������
	);

	m_FontStyleMap.insert(std::make_pair(style, newFont));

}

void fontManager::PrintText(std::string str, int x, int y, DWORD color)
{
	//��� ����
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
	//������ ���� �׸���
	PrintText(str, x + 1, y + 1, shadow);

	PrintText(str, x, y, color);
}

void fontManager::PrintTextOutline(std::string str, int x, int y, DWORD color, DWORD outLine)
{
	//�ܰ� ��´�
	PrintText(str, x + 1, y, outLine);
	PrintText(str, x - 1, y, outLine);
	PrintText(str, x, y + 1, outLine);
	PrintText(str, x, y - 1, outLine);

	//���� Text ��´�
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
