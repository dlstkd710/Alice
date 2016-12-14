#include "StdAfx.h"
#include "cDeviceManager.h"


cDeviceManager::cDeviceManager(void)
	: m_pD3D(NULL)
	, m_pD3DDevice(NULL)
{
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	D3DCAPS9	stCaps;
	int			nVertexProcessing;
	m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &stCaps);
	if(stCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		nVertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		nVertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	D3DPRESENT_PARAMETERS stD3DPP;
	ZeroMemory(&stD3DPP, sizeof(D3DPRESENT_PARAMETERS));
	//stD3DPP.SwapEffect				= D3DSWAPEFFECT_DISCARD;
	//stD3DPP.Windowed				= TRUE;
	//stD3DPP.BackBufferFormat		= D3DFMT_UNKNOWN;
	//stD3DPP.EnableAutoDepthStencil	= TRUE;
	//stD3DPP.AutoDepthStencilFormat	= D3DFMT_D24X8;

	stD3DPP.AutoDepthStencilFormat = D3DFMT_D24S8;				//뎁스 버퍼와 스테실 버퍼 크기 뎁스 24bit 스텐실 버퍼 8 비트
	stD3DPP.BackBufferCount = 1;						//백버퍼는 한개
	stD3DPP.BackBufferFormat = D3DFMT_A8R8G8B8;			//백버퍼 색상 포맷 알파8비트,레드8비트,그린8비트,블루8비트
	stD3DPP.BackBufferHeight = WINSIZEY;				//백버퍼 픽셀 높이 크기
	stD3DPP.BackBufferWidth = WINSIZEX;				//백버퍼 픽셀 가로 크기
	stD3DPP.EnableAutoDepthStencil = true;						//자동 깊이버퍼 사용 여부 ( 그냥 true )
	stD3DPP.Flags = 0;						//일단 0
	stD3DPP.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//화면 주사율 ( 그냥 D3DPRESENT_RATE_DEFAULT 모니터 주사율과 동일시 )
	stD3DPP.hDeviceWindow = g_hWnd;					//Device 가 출력될 윈도우 핸들
	stD3DPP.MultiSampleQuality = 0;						//멀티 샘플링 질 
	stD3DPP.MultiSampleType = D3DMULTISAMPLE_NONE;		//멀티 샘플링 타입 ( D3DMULTISAMPLE_NONE 안티알리아싱 사용안함 ) 
	stD3DPP.PresentationInterval = D3DPRESENT_INTERVAL_ONE;	//화면 전송 간격 ( 그냥 D3DPRESENT_INTERVAL_ONE 모니터 주사율과 동일시 )
	stD3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;	//화면 스왑 체인 방식 ( D3DSWAPEFFECT_DISCARD )
	stD3DPP.Windowed = true;


	m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
		D3DDEVTYPE_HAL,
		g_hWnd,
		nVertexProcessing,
		&stD3DPP,
		&m_pD3DDevice);
}

cDeviceManager::~cDeviceManager(void)
{
}

LPDIRECT3DDEVICE9 cDeviceManager::GetDevice()
{
	return m_pD3DDevice;
}

void cDeviceManager::Destroy()
{
	SAFE_RELEASE(m_pD3D);
	ULONG ul = m_pD3DDevice->Release();
	//assert(ul == 0 && "COM객체 메모리 릭");
	//SAFE_RELEASE(m_pD3DDevice);
}
