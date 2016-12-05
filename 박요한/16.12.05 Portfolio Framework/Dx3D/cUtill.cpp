#include "stdafx.h"
#include "cUtill.h"


namespace cUt {

	POINT GetMousePos()
	{
		POINT mousePos;
		GetCursorPos(&mousePos); //마우스 Screen 상의 위치를 얻어준다.

								 //Screen 상의 위치를 Client 영역의 범위로 바꾼다
		ScreenToClient(g_hWnd, &mousePos);

		return mousePos;
	}

	void SetMousePos(int x, int y)
	{
		POINT setMousePos;
		setMousePos.x = x;
		setMousePos.y = y;

		//Client 상의 위치를 Screen 영역의 범위로 바꾼다
		ClientToScreen(g_hWnd, &setMousePos);

		//SetCursorPos 는 Screen 상의 위치로 셋팅해야 한다.
		SetCursorPos(setMousePos.x, setMousePos.y);
	}

	//value 값을 min max 사이로 맞춰줌
	float Clamp(float value, float min, float max)
	{
		if (value < min)
			return min;
		else if (value > max)
			return max;

		return value;
	}

	//value 값을 0~1 사이로 맞춰줌.
	float Clamp01(float value)
	{
		if (value < 0.0f)
			return 0.0f;
		if (value > 1.0f)
			return 1.0f;

		return value;
	}

	//value 값을 -1 ~ 1 사이로 맞춰줌.
	float ClampMOnePOne(float value)
	{
		if (value < -1.0f)
			return -1.0f;
		if (value > 1.0f)
			return 1.0f;

		return value;
	}

	//float 선형 보간
	float Lerp(float from, float to, float t)
	{
		t = Clamp01(t);

		float delta = to - from;

		return (delta * t) + from;
	}

	float InverseLerp(float head, float rear, float value)
	{
		float delta = rear - head;
		float inValue = value - head;

		if (inValue < 0)
			inValue = 0;

		return inValue / delta;
	}

	//벡터 선형 보간
	D3DXVECTOR3 VecLerp(const D3DXVECTOR3& from, const D3DXVECTOR3& to, float t)
	{
		D3DXVECTOR3 result(
			Lerp(from.x, to.x, t),
			Lerp(from.y, to.y, t),
			Lerp(from.z, to.z, t));

		return result;
	}


	void SetRandomSeed()
	{
		srand(GetTickCount());
	}

	float RandomFloatRange(float min, float max)
	{
		// 0 ~ 99999
		// 0 ~ 1 까지의 실수

		float factor = (rand() % 1001) * 0.001f;

		float delta = (max - min);

		float result = (delta * factor) + min;

		return result;

	}

	int RandomIntRange(int min, int max)
	{
		int delta = max - min;

		return (rand() % delta) + min;
	}





	//정점의 노말을 계산해주는 함수
	void ComputeNormal(
		D3DXVECTOR3* pNorams,			//결과로 받을 노말백터 배열 포인터 ( 노말벡터의 배열의 수는 정점 배열의 수와 같다 )
		const D3DXVECTOR3* pVertices,	//계산에 사용된 정점위치 배열 포인터
		int verticesNum,				//정점 갯수
		const DWORD* pIndices,			//인덱스 배열포인터 ( 일단 인덱스는 DWORD 로 대동단결 )
		int indicesNum					//인덱스 배열포인터 갯수
	)
	{
		//노말 결과 털고 시작
		ZeroMemory(pNorams, sizeof(D3DXVECTOR3) * verticesNum);

		//삼각형 갯수
		DWORD triNum = indicesNum / 3;

		for (DWORD i = 0; i < triNum; i++)
		{
			//해당 삼각형의 정점 인덱스
			DWORD i0 = pIndices[(i * 3) + 0];
			DWORD i1 = pIndices[(i * 3) + 1];
			DWORD i2 = pIndices[(i * 3) + 2];

			//정점 3개
			D3DXVECTOR3 v0 = pVertices[i0];
			D3DXVECTOR3 v1 = pVertices[i1];
			D3DXVECTOR3 v2 = pVertices[i2];

			//Edge
			D3DXVECTOR3 edge1 = v1 - v0;
			D3DXVECTOR3 edge2 = v2 - v0;

			//Cross
			D3DXVECTOR3 cross;
			D3DXVec3Cross(&cross, &edge1, &edge2);

			//Normal
			D3DXVECTOR3 normal;
			D3DXVec3Normalize(&normal, &cross);

			//증가 시킨다.
			pNorams[i0] += normal;
			pNorams[i1] += normal;
			pNorams[i2] += normal;
		}

		//최종 적으로 Normalvector 정규화한다.
		for (DWORD i = 0; i < (DWORD)verticesNum; i++)
		{
			D3DXVec3Normalize(&pNorams[i], &pNorams[i]);
		}
	}



}
