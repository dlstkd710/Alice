#pragma once
#include "cDeviceManager.h"
#define EPSILON 0.00001f

//a,b가 같은값인가
#define FLOATEQUAL(a,b)	(abs((a)-(b))<EPSILON)

//a의값이 거의 0일경우
#define FLOATZERO(a) (abs((a))<EPSILON)

//제로벡터인가
#define VECTORZERO(v)  (FLOATZERO((v).x) && FLOATZERO((v).y) && FLOATZERO((v).z))

#define RS(a, b)		Device->SetRenderState((a), (b))

//반직선 구조체
typedef struct tagRay {
	D3DXVECTOR3 origin;
	D3DXVECTOR3 direction;
}Ray, *LPRay;

namespace cUt
{
	POINT GetMousePos();
	void SetMousePos(int x, int y);

	float Clamp(float value, float min, float max);	//value 값을 min max 사이로 맞춰줌
	float Clamp01(float value);						//value 값을 0과 1사이로 맞춤
	float ClampMOnePOne(float value);				//value 값을 -1과 1사이로 맞춤

	//float 선형 보간
	float Lerp(float from, float to, float t);

	//float 선형 보간 반대
	float InverseLerp(float head, float rear, float value);

	//벡터 선형 보간
	D3DXVECTOR3 VecLerp(const D3DXVECTOR3& from, const D3DXVECTOR3& to, float t);

	//랜덤
	void SetRandomSeed();
	float RandomFloatRange(float min, float max);
	int RandomIntRange(int min, int max);

	//정점의 노말을 계산해주는 함수
	void ComputeNormal(
		D3DXVECTOR3* pNorams,			//결과로 받을 노말백터 배열 포인터 ( 노말벡터의 배열의 수는 정점 배열의 수와 같다 )
		const D3DXVECTOR3* pVertices,	//계산에 사용된 정점위치 배열 포인터
		int verticesNum,				//정점 갯수
		const DWORD* pIndices,			//인덱스 배열포인터 ( 일단 인덱스는 DWORD 로 대동단결 )
		int indicesNum					//인덱스 배열포인터 갯수
	);
	
	void CharToWChar(const char* pstrSrc, wchar_t pwstrDest[]);

	void WCharToChar(const wchar_t* pwstrSrc, char pstrDest[]);

	inline POINT PointMake(int x, int y)
	{
		POINT pt = { x,y };
		return pt;
	}

	inline RECT RectMake(int x, int y, int width, int height)
	{
		RECT rc = { x,y,x + width,y + height };
		return rc;
	}
	inline RECT RectMakeCenter(int x, int y, int width, int height)
	{
		RECT rc = { x - width / 2, y - height / 2, x + width / 2, y + height / 2 };
		return rc;
	}
	inline LPD3DXMESH SphereMake(float Radius, int Slices, int Stacks)
	{
		LPD3DXMESH pOut = NULL;
		FLOAT R = Radius;
		UINT Sl = Slices;
		UINT St = Stacks;
		D3DXCreateSphere(g_pD3DDevice, R, Sl, St, &pOut, NULL);
		return pOut;
	}
}

