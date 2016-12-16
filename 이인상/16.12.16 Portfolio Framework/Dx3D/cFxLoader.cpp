#include "stdafx.h"
#include "cFxLoader.h"

cFxLoader::cFxLoader()
{

}

cFxLoader::~cFxLoader()
{

}

LPD3DXEFFECT cFxLoader::LoadResource(std::string filePath)
{
	/* Output */
	LPD3DXEFFECT Output = NULL;

	/* Debug Error Buffer */
	LPD3DXBUFFER pError = NULL;

	/* Shader Flag */
	DWORD dwShaderFlag = 0;

#ifdef _DEBUG
	//���̴��� ����׸��� �������ϰڴ� 
	//����׸��� �ؾ� �߸��� ������ ������ ���� Error ���ۿ� ���������� ���� 
	dwShaderFlag = dwShaderFlag | D3DXSHADER_DEBUG;
#endif

	/* Create Effect */
	D3DXCreateEffectFromFile(
		g_pD3DDevice,
		filePath.c_str(),
		NULL,
		NULL,
		dwShaderFlag,
		NULL,
		&Output,
		&pError
		);

	if (pError != NULL)
	{
		//Error Buffer Release
		SAFE_RELEASE(pError);
		
		assert(pError != NULL && " Failed Shader Loading ");
	}

	return Output;
}
