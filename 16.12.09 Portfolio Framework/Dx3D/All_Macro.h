#pragma once
extern HWND	g_hWnd;
extern HINSTANCE hInst;
extern HDC g_hdc;



#define °¢µµ(a) (a * 0.017453f )

#define WINDOW_TITLE L"HeightMap-TList"
#define BMP_TERRAIN L"terrain.png"
#define BMP_HEIGHTMAP L"mountains.bmp"

#define SAFE_ADD_REF(p) if(p) { (p)->AddRef(); }
#define SAFE_RELEASE(p) if(p) { (p)->Release(); (p) = NULL; }
#define SAFE_DELETE(p) if(p) { delete (p); (p) = NULL; }
#define SAFE_DELETE_ARR(p) if (p) {delete [] p;}
#define SAFE_DELETE_ARRAY(p) if(p) { delete [] (p); }

#define IDLE_TRANSITION_TIME 0.125f
#define MOVE_TRANSITION_TIME 0.25f

#define WINSIZEX 1366
#define WINSIZEY 768
#define MSGBOX(string) MessageBox(NULL, string, "Message", MB_OK)
#define SINGLETONE(class_name) private:\
	class_name(void);\
	~class_name(void);\
public:\
	static class_name* GetInstance()\
	{\
		static class_name instance;\
		return &instance;\
	}

#define SYNTHESIZE(varType, varName, funName)\
protected: varType varName;\
public: inline varType Get##funName(void) const { return varName; }\
public: inline void Set##funName(varType var){ varName = var; }

#define SYNTHESIZE_PASS_BY_REF(varType, varName, funName)\
protected: varType varName;\
public: inline varType& Get##funName(void) { return varName; }\
public: inline void Set##funName(varType& var){ varName = var; }

#define SYNTHESIZE_ADD_REF(varType, varName, funName)    \
protected: varType varName; \
public: virtual varType Get##funName(void) const { return varName; } \
public: virtual void Set##funName(varType var){\
	if (varName != var) {\
	SAFE_ADD_REF(var);\
	SAFE_RELEASE(varName);\
	varName = var;\
	}\
}

