#pragma once
#include <bitset>
using namespace std;
//==================================================================
//		## keyManager ## (Ű�Ŵ���)
//==================================================================

#pragma once

#define g_pkeyManager keyManager::GetInstance()

#define KEYMAX 256

class keyManager
{
private:
	SINGLETONE(keyManager);
private:
	bitset<KEYMAX> _keyUp;
	bitset<KEYMAX> _keyDown;

public:
	//Ű�Ŵ��� �ʱ�ȭ
	HRESULT init(void);

	//Ű�� �ѹ��� ���ȳ�?
	bool isOnceKeyDown(int key);
	//Ű�� �ѹ� ������ �����?
	bool isOnceKeyUp(int key);
	//Ű�� ��� ���� �ֳ�?
	bool isStayKeyDown(int key);
	//Ű�� ����̳�?
	bool isToggleKey(int key);
};

