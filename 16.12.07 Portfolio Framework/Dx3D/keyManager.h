#pragma once
#include <bitset>
using namespace std;
//==================================================================
//		## keyManager ## (키매니져)
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
	//키매니져 초기화
	HRESULT init(void);

	//키가 한번만 눌렸냐?
	bool isOnceKeyDown(int key);
	//키가 한번 눌렀다 띄었냐?
	bool isOnceKeyUp(int key);
	//키가 계속 눌려 있냐?
	bool isStayKeyDown(int key);
	//키가 토글이냐?
	bool isToggleKey(int key);
};

