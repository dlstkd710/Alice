#include "stdafx.h"
#include "logManager.h"
#include <Windows.h>
#include <MMSystem.h>
#include <stdio.h>
#include <iostream>
#include <time.h>


logManager::logManager()
	: m_hWndLog(NULL),
	m_hWndListBox(NULL),
	m_nTarget(LOG_NONE)
{
	m_szFileName[0] = NULL;
}


logManager::~logManager()
{
	Release();
}

HRESULT logManager::init(unsigned int nTarget, HWND hParentWnd, const char * pszFileName)
{
	m_nTarget = nTarget;


	//�α����Ϸ� �����Ѵٸ�...
	if (m_nTarget & LOG_FILE)
		strcpy_s(m_szFileName, MAX_PATH, pszFileName);

	else
		m_szFileName[0] = NULL;


	//�α� ��������...
	if (m_nTarget & LOG_WINDOW)
		CreateLogWindow(hParentWnd);

	AddLog("============== Begin Log ============== ");

	return S_OK;
}

VOID logManager::AddLog(char * strmsg, ...)
{
	char szBuff[1024];
	char szDate[128];		//��¥
	char szTime[128];		//�ð�

	_strdate_s(szDate, 128);
	_strtime_s(szTime, 128);
	vsprintf_s(szBuff, strmsg, (char*)(&strmsg + 1));

	//Console�� ����� ���
	if (m_nTarget & LOG_CONSOLE)
	{
		printf("(data[%s] time[%s] ) : %s\n", szDate, szTime, szBuff);
	}

	//FILE �� ��� �Ұ��
	if (m_nTarget & LOG_FILE)
	{
		LogFile("(data[%s] time[%s] ) : %s\n", szDate, szTime, szBuff);
	}

	//LOG �����쿡 ����� ��� ( ����� ��忡���� ����
#ifdef _DEBUG

	if (m_nTarget & LOG_WINDOW)
	{
		//������ ����Ʈ �ڽ��� �α� �߰�
		SendMessage(m_hWndListBox, LB_ADDSTRING, 0, (LPARAM)szBuff);

		//�߰��� ����Ʈ �ڽ��� ���� ��� (�������� �����ϸ� �ȴ� )
		UINT32 n = SendMessage(m_hWndListBox, LB_GETCOUNT, 0, 0) - 1;

		//���� Ŀ�� ����������
		SendMessage(m_hWndListBox, LB_SETCURSEL, (WPARAM)n, 0);
	}

#endif //_DEBUG
}

VOID logManager::LogFile(char * strmsg, ...)
{
	//���ð��� ���´�.
	SYSTEMTIME		st;
	GetLocalTime(&st);

	char szBuf[256];		//���� �̸�

	if (m_nTarget & LOG_FILE)
	{
		//wchar_t* _szBuf = NULL;
		//wchar_t* _txt = NULL;
		//cUt::CharToWChar(szBuf, _szBuf);
		//cUt::CharToWChar("%s.txt", _txt);
		wsprintf(szBuf, "%s.txt", m_szFileName);
	}

	// c�� ��Ÿ���� ��������� 
	FILE*		fp = NULL;
	//fp = fopen(szBuf,"a");		//������ ������ ������ �߰��� �ٿ� �ִ´�.
	fopen_s(&fp, szBuf, "a");

	// ���� ó�� 
	if (fp == NULL)
	{
		//wchar_t* _errorfile = NULL;
		//cUt::CharToWChar("���� ���Ϸ� ���� ����", _errorfile);
		MessageBox(0, "���� ���Ϸ� ���� ����", 0, 0);
		return;
	}

	else
	{
		char	buffer[256];
		va_list list;					// ����Ʈ ������ �����Ѵ�.

		va_start(list, strmsg);			// ���˿� ����
		vsprintf_s(buffer, strmsg, list);	// �°� �ڸ��� 
		va_end(list);					// ����Ʈ �� 
		fprintf(fp, "%s\n", buffer);
	}

	//���� �ݴ´�.
	if (fp)
	{
		fclose(fp);
		fp = NULL;
	}
}

VOID logManager::Release(VOID)
{
	if (m_hWndLog != NULL)
	{
		DestroyWindow(m_hWndLog);
	}
}

VOID logManager::CreateLogWindow(HWND hWnd)
{
	//
	// �α� ������ ����
	//
	int			x, y, cx, cy;
	WNDCLASS	wc;
	RECT		rc;

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC)logManager::WndLogProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	//wchar_t* _lpszClassName = NULL;
	//cUt::CharToWChar("LogWin", _lpszClassName);
	wc.lpszClassName = "LogWin";

	RegisterClass(&wc);


	//�θ� ������ �����ʿ� ��ġ�ϰ�....
	RECT	rcWin;
	GetWindowRect(hWnd, &rcWin);

	cx = LOG_WINDOW_CX;
	cy = LOG_WINDOW_CY;
	x = rcWin.right;
	y = rcWin.top;

	HWND		hParenthWnd = NULL;
	HINSTANCE	hInst = NULL;

	hParenthWnd = hWnd;
	hInst = GetModuleHandle(NULL);
	//wchar_t* _lpsz= NULL;
	//cUt::CharToWChar("LogWin", _lpsz);
	m_hWndLog = CreateWindow(
		"LogWin",
		LOG_WINDOW_TITLE,
		WS_POPUP | WS_CAPTION | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		x, y, cx, cy, hParenthWnd, NULL,
		hInst, NULL);


	//
	// �α� �����쿡 ����Ʈ �ڽ� ������ �ٿ� �ִ´�.
	//
	GetClientRect(m_hWndLog, &rc);
	//wchar_t* _listbox = NULL;
	//cUt::CharToWChar("listbox", _listbox);
	m_hWndListBox = CreateWindow("listbox", "", WS_CHILD | WS_VSCROLL, 0, 0, rc.right, rc.bottom, m_hWndLog, NULL, GetModuleHandle(NULL), NULL);
	ShowWindow(m_hWndLog, SW_SHOW);
	ShowWindow(m_hWndListBox, SW_SHOW);
}

LRESULT logManager::WndLogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}