#include "stdafx.h"
#include "cUtill.h"


namespace cUt 
{

	POINT GetMousePos()
	{
		POINT mousePos;
		GetCursorPos(&mousePos); //���콺 Screen ���� ��ġ�� ����ش�.

								 //Screen ���� ��ġ�� Client ������ ������ �ٲ۴�
		ScreenToClient(g_hWnd, &mousePos);

		return mousePos;
	}

	void SetMousePos(int x, int y)
	{
		POINT setMousePos;
		setMousePos.x = x;
		setMousePos.y = y;

		//Client ���� ��ġ�� Screen ������ ������ �ٲ۴�
		ClientToScreen(g_hWnd, &setMousePos);

		//SetCursorPos �� Screen ���� ��ġ�� �����ؾ� �Ѵ�.
		SetCursorPos(setMousePos.x, setMousePos.y);
	}

	//value ���� min max ���̷� ������
	float Clamp(float value, float min, float max)
	{
		if (value < min)
			return min;
		else if (value > max)
			return max;

		return value;
	}

	//value ���� 0~1 ���̷� ������.
	float Clamp01(float value)
	{
		if (value < 0.0f)
			return 0.0f;
		if (value > 1.0f)
			return 1.0f;

		return value;
	}

	//value ���� -1 ~ 1 ���̷� ������.
	float ClampMOnePOne(float value)
	{
		if (value < -1.0f)
			return -1.0f;
		if (value > 1.0f)
			return 1.0f;

		return value;
	}

	//float ���� ����
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

	//���� ���� ����
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
		// 0 ~ 1 ������ �Ǽ�

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





	//������ �븻�� ������ִ� �Լ�
	void ComputeNormal(
		D3DXVECTOR3* pNorams,			//����� ���� �븻���� �迭 ������ ( �븻������ �迭�� ���� ���� �迭�� ���� ���� )
		const D3DXVECTOR3* pVertices,	//��꿡 ���� ������ġ �迭 ������
		int verticesNum,				//���� ����
		const DWORD* pIndices,			//�ε��� �迭������ ( �ϴ� �ε����� DWORD �� �뵿�ܰ� )
		int indicesNum					//�ε��� �迭������ ����
	)
	{
		//�븻 ��� �а� ����
		ZeroMemory(pNorams, sizeof(D3DXVECTOR3) * verticesNum);

		//�ﰢ�� ����
		DWORD triNum = indicesNum / 3;

		for (DWORD i = 0; i < triNum; i++)
		{
			//�ش� �ﰢ���� ���� �ε���
			DWORD i0 = pIndices[(i * 3) + 0];
			DWORD i1 = pIndices[(i * 3) + 1];
			DWORD i2 = pIndices[(i * 3) + 2];

			//���� 3��
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

			//���� ��Ų��.
			pNorams[i0] += normal;
			pNorams[i1] += normal;
			pNorams[i2] += normal;
		}

		//���� ������ Normalvector ����ȭ�Ѵ�.
		for (DWORD i = 0; i < (DWORD)verticesNum; i++)
		{
			D3DXVec3Normalize(&pNorams[i], &pNorams[i]);
		}
	}

	// char to LPCWSTR
	void CharToWChar(const char * pstrSrc, wchar_t pwstrDest[])
	{
		int nLen = (int)strlen(pstrSrc) + 1;
		mbstowcs(pwstrDest, pstrSrc, nLen);
	}

	//LPCWSTR to char
	void WCharToChar(const wchar_t * pwstrSrc, char pstrDest[])
	{
		int nLen = (int)wcslen(pwstrSrc);
		wcstombs(pstrDest, pwstrSrc, nLen + 1);
	}

}