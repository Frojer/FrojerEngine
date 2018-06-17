#include "FEDXRenderer.h"
#include <FEDefine.h>

FEDXRenderer::FEDXRenderer()
	: _pDevice(nullptr), _pDXDC(nullptr), _pSwapChain(nullptr), _pRTView(nullptr), _featureLevels(D3D_FEATURE_LEVEL_11_0)
{
}


FEDXRenderer::~FEDXRenderer()
{
	Release();
}


bool FEDXRenderer::Create(void* i_phWnd)
{
	HWND hWnd = *(HWND*)i_phWnd;
	//----------------------------------------
	// 1�ܰ� : ������ ��ġ ���� �ʼ� ����
	//----------------------------------------
	// D3D ������ ��ġ Device �� ����ü�� Swap Chain ����. 
	CreateDeviceSwapChain(hWnd);

	// ��ġ-����ü���� ����Ÿ��(�����) ȹ��
	CreateRenderTarget();
	
	
	// ��ġ ��º��ձ�(Output Merger) �� ���͸� Ÿ�� �� ����-���ٽ� ���� ���.
	_pDXDC->OMSetRenderTargets(
				1,				// ����Ÿ�� ����.(max: D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)
				&_pRTView,		// ����Ÿ��("�����") ���.	
				nullptr
				);
	
	// ����Ʈ ����.
	SetViewPort();

	return true;
}


void FEDXRenderer::Release()
{
	//��ġ ���� ���� : ���� ���� �ʱ�ȭ�� �ؾ� �մϴ�. (�޸� ���� ����)
	if (_pDXDC) _pDXDC->ClearState();
	//if (g_pSwapChain) g_pSwapChain->SetFullscreenState(false, NULL);


	SAFE_RELEASE(_pRTView);			//����Ÿ�� ����.
	SAFE_RELEASE(_pSwapChain);			//����ü�� ����.
	SAFE_RELEASE(_pDXDC);
	SAFE_RELEASE(_pDevice);			//����̽� ����. �� ���߿� �����մϴ�.
}


/////////////////////////////////////////////////////////////////////////////
//
//  D3D ������ '��ġ(Device)' �� ����ü�� ����. 
//
HRESULT FEDXRenderer::CreateDeviceSwapChain(HWND i_hWnd)
{
	HRESULT hr = S_OK;

	// ��ġ (Device) �� ����ü��(SwapChain) ���� ����.
	// ����ü���� ���߹��۸� �ý����� ���ϸ�
	// �������� '�ø���Flipping' ü�ΰ� ������ �ǹ��Դϴ�.  
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.Windowed = TRUE;					// Ǯ��ũ�� �Ǵ� â��� ����.
	sd.OutputWindow = i_hWnd;			// ����� ������ �ڵ�.
	sd.BufferCount = 1;					// ����� ����.
	sd.BufferDesc.Width = m_Setting.uiWindowWidth;	// �ػ� ����.(����� ũ��)
	sd.BufferDesc.Height = m_Setting.uiWindowHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		// ����� ����԰� (A8R8G8B8) â��忡���� ���� ���� 
	sd.BufferDesc.RefreshRate.Numerator = 0;			    // ���� ������.(��������ȭ VSync Ȱ��ȭ�� ǥ�ذ����� ���� : 60hz)
	//sd.BufferDesc.RefreshRate.Numerator = 0;				// ���� ������.(��������ȭ VSync Off)
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		// �뵵 ����: '����Ÿ��' 
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	//sd.Flags = 0;
	sd.SampleDesc.Count = 1;	// AA ����
	sd.SampleDesc.Quality = 0;



	// D3D ������ '��ġ(Device)' �� ����ü�� ����. 
	//
	hr = D3D11CreateDeviceAndSwapChain(
		NULL,						// ���� ��� ������ (�⺻��ġ�� NULL)
		D3D_DRIVER_TYPE_HARDWARE,	// HW ���� 
		NULL,						// SW Rasterizer DLL �ڵ�.  HW ���ӽÿ��� NULL.
		0,							// ����̽� ���� �÷���.(�⺻��)
		// D3D11_CREATE_DEVICE_DEBUG,//����̽� ���� �÷���.(�����)
		&_featureLevels,			// (������) ����̽� ��� ����(Feature Level) �迭
		1,							// (������) ����̽� ��� ����(Feature Level) �迭 ũ��.
		D3D11_SDK_VERSION,			// DX SDK ����.
		&sd,						// ����̽� ���� �� �ɼ�.
		&_pSwapChain,				// [���] ����ü�� �������̽� ���.
		&_pDevice,					// [���] ����̽� �������̽� ���.
		NULL,						// [���] (������) ����̽� ��� ����. �ʿ���ٸ� NULL ����. 
		&_pDXDC						// [���] ����̽� ���ؽ�Ʈ ���.
	);

	if (FAILED(hr))
	{
		// ����̽� / ����ü�� ���� ����
		return hr;
	}

	return hr;
}


/////////////////////////////////////////////////////////////////////////////
//
// ��ġ-����ü���� ����Ÿ��(�����) ȹ��
//
// ���ҽ���(Resource View)  
// DX11 �� ���ҽ��� "����" �������� �����ϰ� ����Ǿ� �ֽ��ϴ�. 
// (�޸� ������ ȿ���� ���� �� �ٿ뵵 Ȱ���� ����..)
// ���� ����� �� �뵵�� �´� ���ٹ��(�������̽�)�� ȹ��(����)�ؾ� �մϴ�.
// �� �۾��� "���ҽ� �� Resource View" �� ���� ó���մϴ�.
//                     
// ����ü�ο� ����(Bind)�� ������ ��¹��� - "����� BackBuffer" �� 2D ��� ���ҽ�(�ؽ�ó) �̸�
// "����Ÿ�� RenderTarget" ������ ���ҽ��並 ����, �ش� ���۸� ����/��� �ϰڽ��ϴ�.
//
HRESULT FEDXRenderer::CreateRenderTarget()
{
	HRESULT hr = S_OK;

	// ����� ȹ��.
	ID3D11Texture2D *pBackBuffer;
	hr = _pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);	// ����� ���, 2D �ؽ��� ����, ȭ����¿�
	if (FAILED(hr))	return hr;

	// ȹ���� ����ۿ� ����Ÿ�� �� ����.(����Ÿ��'��'���� ������)
	hr = _pDevice->CreateRenderTargetView(pBackBuffer, NULL, &_pRTView);		// '����Ÿ�ٺ�'�� ����.

	if (FAILED(hr))
	{
		// �����-����Ÿ�ٺ� ���� ����
	}

	//���ҽ� �� ���� ��, ���ʿ��� DX �ڵ��� �����ؾ� �մϴ�.(�޸� ���� ����)
	SAFE_RELEASE(pBackBuffer);

	return hr;
}


void FEDXRenderer::SetViewPort()
{
	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = (FLOAT)m_Setting.uiWindowWidth;
	vp.Height = (FLOAT)m_Setting.uiWindowHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	_pDXDC->RSSetViewports(1, &vp);
}


void FEDXRenderer::SetVertexBuffers(UINT StartSlot, UINT NumBuffers, LPVERTEXBUFFER const* ppVertexBuffers, const UINT* pStrides, const UINT* pOffsets)
{

}


void FEDXRenderer::SetIndexBuffer(LPINDEXBUFFER pIndexBuffer, FEGI_FORMAT Format, UINT Offset)
{

}


void FEDXRenderer::SetPrimitiveTopology(FE_PRIMITIVE_TOPOLOGY Topology)
{

}


void FEDXRenderer::ClearBackBuffer(const FEVector4& i_color)
{
	_pDXDC->ClearRenderTargetView(_pRTView, (float*)&i_color);			//����Ÿ�� �����.
}


void FEDXRenderer::Flip()
{
	_pSwapChain->Present(0, 0);			//ȭ����� : Flip!
}