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

	stD3DPP.AutoDepthStencilFormat = D3DFMT_D24S8;				//���� ���ۿ� ���׽� ���� ũ�� ���� 24bit ���ٽ� ���� 8 ��Ʈ
	stD3DPP.BackBufferCount = 1;						//����۴� �Ѱ�
	stD3DPP.BackBufferFormat = D3DFMT_A8R8G8B8;			//����� ���� ���� ����8��Ʈ,����8��Ʈ,�׸�8��Ʈ,���8��Ʈ
	stD3DPP.BackBufferHeight = WINSIZEY;				//����� �ȼ� ���� ũ��
	stD3DPP.BackBufferWidth = WINSIZEX;				//����� �ȼ� ���� ũ��
	stD3DPP.EnableAutoDepthStencil = true;						//�ڵ� ���̹��� ��� ���� ( �׳� true )
	stD3DPP.Flags = 0;						//�ϴ� 0
	stD3DPP.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//ȭ�� �ֻ��� ( �׳� D3DPRESENT_RATE_DEFAULT ����� �ֻ����� ���Ͻ� )
	stD3DPP.hDeviceWindow = g_hWnd;					//Device �� ��µ� ������ �ڵ�
	stD3DPP.MultiSampleQuality = 0;						//��Ƽ ���ø� �� 
	stD3DPP.MultiSampleType = D3DMULTISAMPLE_NONE;		//��Ƽ ���ø� Ÿ�� ( D3DMULTISAMPLE_NONE ��Ƽ�˸��ƽ� ������ ) 
	stD3DPP.PresentationInterval = D3DPRESENT_INTERVAL_ONE;	//ȭ�� ���� ���� ( �׳� D3DPRESENT_INTERVAL_ONE ����� �ֻ����� ���Ͻ� )
	stD3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;	//ȭ�� ���� ü�� ��� ( D3DSWAPEFFECT_DISCARD )
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
	//assert(ul == 0 && "COM��ü �޸� ��");
	//SAFE_RELEASE(m_pD3DDevice);
}
