#pragma once
#include "cDeviceManager.h"
#define EPSILON 0.00001f

//a,b�� �������ΰ�
#define FLOATEQUAL(a,b)	(abs((a)-(b))<EPSILON)

//a�ǰ��� ���� 0�ϰ��
#define FLOATZERO(a) (abs((a))<EPSILON)

//���κ����ΰ�
#define VECTORZERO(v)  (FLOATZERO((v).x) && FLOATZERO((v).y) && FLOATZERO((v).z))

#define RS(a, b)		Device->SetRenderState((a), (b))

//������ ����ü
typedef struct tagRay {
	D3DXVECTOR3 origin;
	D3DXVECTOR3 direction;
}Ray, *LPRay;

namespace cUt
{
	POINT GetMousePos();
	void SetMousePos(int x, int y);

	float Clamp(float value, float min, float max);	//value ���� min max ���̷� ������
	float Clamp01(float value);						//value ���� 0�� 1���̷� ����
	float ClampMOnePOne(float value);				//value ���� -1�� 1���̷� ����

	//float ���� ����
	float Lerp(float from, float to, float t);

	//float ���� ���� �ݴ�
	float InverseLerp(float head, float rear, float value);

	//���� ���� ����
	D3DXVECTOR3 VecLerp(const D3DXVECTOR3& from, const D3DXVECTOR3& to, float t);

	//����
	void SetRandomSeed();
	float RandomFloatRange(float min, float max);
	int RandomIntRange(int min, int max);

	//������ �븻�� ������ִ� �Լ�
	void ComputeNormal(
		D3DXVECTOR3* pNorams,			//����� ���� �븻���� �迭 ������ ( �븻������ �迭�� ���� ���� �迭�� ���� ���� )
		const D3DXVECTOR3* pVertices,	//��꿡 ���� ������ġ �迭 ������
		int verticesNum,				//���� ����
		const DWORD* pIndices,			//�ε��� �迭������ ( �ϴ� �ε����� DWORD �� �뵿�ܰ� )
		int indicesNum					//�ε��� �迭������ ����
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

