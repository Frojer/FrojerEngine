#include "FEDX11Renderer.h"
#ifdef FE_DX11
#include <FEDefine.h>
#include "FEDX11Buffer.h"

FEDX11Renderer::FEDX11Renderer()
	: _pDevice(nullptr), _pDXDC(nullptr), _pSwapChain(nullptr), _pRTView(nullptr), _featureLevels(D3D_FEATURE_LEVEL_11_0), _pDS(nullptr), _pDSView(nullptr)
{
}


FEDX11Renderer::~FEDX11Renderer()
{
	Release();
}


bool FEDX11Renderer::Create(void* i_phWnd)
{
	HWND hWnd = *(HWND*)i_phWnd;
	//----------------------------------------
	// 1�ܰ� : ������ ��ġ ���� �ʼ� ����
	//----------------------------------------
	// D3D ������ ��ġ Device �� ����ü�� Swap Chain ����. 
	CreateDeviceSwapChain(hWnd);

	// ��ġ-����ü���� ����Ÿ��(�����) ȹ��
	CreateRenderTarget();

	// ����/���ٽ� ���� ����.
	CreateDepthStencil();
	
	
	// ��ġ ��º��ձ�(Output Merger) �� ���͸� Ÿ�� �� ����-���ٽ� ���� ���.
	_pDXDC->OMSetRenderTargets(
				1,				// ����Ÿ�� ����.(max: D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)
				&_pRTView,		// ����Ÿ��("�����") ���.	
				_pDSView
				);
	
	// ����Ʈ ����.
	SetViewPort();

	//����-���ٽ� ������ ���� ��ü ����.
	//StateObjectCreate();

	//���� ���� ��ü ����
	//BlendStateCreate();

	return true;
}


void FEDX11Renderer::Release()
{
	//CTexture2D::SamplerRelease();
	RasterStateRelease();
	DSStateRelease();
	//BlendStateRelease();

	//��ġ ���� ���� : ���� ���� �ʱ�ȭ�� �ؾ� �մϴ�. (�޸� ���� ����)
	if (_pDXDC) _pDXDC->ClearState();
	//if (g_pSwapChain) g_pSwapChain->SetFullscreenState(false, NULL);

	SAFE_RELEASE(_pDS);				// ����/���ٽ� ���� ����.
	SAFE_RELEASE(_pDSView);
	SAFE_RELEASE(_pRTView);			// ����Ÿ�� ����.
	SAFE_RELEASE(_pSwapChain);		// ����ü�� ����.
	SAFE_RELEASE(_pDXDC);
	SAFE_RELEASE(_pDevice);			// ����̽� ����. �� ���߿� �����մϴ�.
}


/////////////////////////////////////////////////////////////////////////////
//
//  D3D ������ '��ġ(Device)' �� ����ü�� ����. 
//
HRESULT FEDX11Renderer::CreateDeviceSwapChain(HWND i_hWnd)
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
	sd.BufferDesc.Width = m_setting.uiWindowWidth;	// �ػ� ����.(����� ũ��)
	sd.BufferDesc.Height = m_setting.uiWindowHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		// ����� ����԰� (A8R8G8B8) â��忡���� ���� ���� 
	sd.BufferDesc.RefreshRate.Numerator = 0;			    // ���� ������.(��������ȭ VSync Ȱ��ȭ�� ǥ�ذ����� ���� : 60hz)
	//sd.BufferDesc.RefreshRate.Numerator = 0;				// ���� ������.(��������ȭ VSync Off)
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		// �뵵 ����: '����Ÿ��' 
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	//sd.Flags = 0;
	sd.SampleDesc.Count = m_setting.SampleCount;	// AA ����
	sd.SampleDesc.Quality = m_setting.SampleQuality;



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
HRESULT FEDX11Renderer::CreateRenderTarget()
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


HRESULT FEDX11Renderer::CreateDepthStencil()
{
	HRESULT hr = S_OK;

	//����/���ٽ� ���ۿ� ���� ����.
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(td));
	td.Width = m_setting.uiWindowWidth;
	td.Height = m_setting.uiWindowHeight;
	td.MipLevels = 1;
	td.ArraySize = 1;
	//td.Format = DXGI_FORMAT_D32_FLOAT;				// 32BIT. ���� ����
	//td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;		// ���� ���� (24bit) + ���ٽ� (8bit) / ���� �ϵ���� (DX9)
	td.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;		// ���� ���� (32bit) + ���ٽ� (8bit) / ���� �ϵ���� (DX11)
	td.SampleDesc.Count = m_setting.SampleCount;		// AA ���� - RT �� ���� �԰� �ؼ�.
	td.SampleDesc.Quality = m_setting.SampleQuality;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;			// ����-���ٽ� ���ۿ����� ����.
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;


	// ���� ���� ����.
	hr = _pDevice->CreateTexture2D(&td, NULL, &_pDS);
	if (FAILED(hr)) return hr;


	// ����-���ٽǹ��ۿ� ���ҽ� �� ���� ����. 
	D3D11_DEPTH_STENCIL_VIEW_DESC  dd;
	ZeroMemory(&dd, sizeof(dd));
	dd.Format = td.Format;
	//dd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D; //AA ����.
	dd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS; //AA ����.
	dd.Texture2D.MipSlice = 0;

	//����-���ٽ� ���� �� ����.
	hr = _pDevice->CreateDepthStencilView(_pDS, &dd, &_pDSView);
	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}


void FEDX11Renderer::SetViewPort() const
{
	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = (FLOAT)m_setting.uiWindowWidth;
	vp.Height = (FLOAT)m_setting.uiWindowHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	_pDXDC->RSSetViewports(1, &vp);
}


void FEDX11Renderer::RasterStateCreate(BYTE flag)
{
	// ������ ���� ��ü
	D3D11_RASTERIZER_DESC rd;

	// �ﰢ�� ���� ä���
	rd.FillMode = flag & 0x80 ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;

	// �����̽� �ø�
	switch (flag & 0x60)
	{
	case FE_RS_CULL_NONE:
		rd.CullMode = D3D11_CULL_NONE;
		break;
	case FE_RS_CULL_FRONT:
		rd.CullMode = D3D11_CULL_FRONT;
		break;
	case FE_RS_CULL_BACK:
		rd.CullMode = D3D11_CULL_BACK;
		break;
	}
	rd.FrontCounterClockwise = flag & 0x10 ? true : false;
	rd.DepthBias = 0;
	rd.DepthBiasClamp = 0;
	rd.SlopeScaledDepthBias = 0;
	rd.DepthClipEnable = flag & 0x08 ? false : true;
	rd.ScissorEnable = flag & 0x04 ? true : false;
	rd.MultisampleEnable = flag & 0x02 ? true : false;
	rd.AntialiasedLineEnable = flag & 0x01 ? true : false;

	_pDevice->CreateRasterizerState(&rd, &_RSStateMap[flag]);
}


void FEDX11Renderer::RasterStateRelease()
{
	FOR_STL(_RSStateMap)
	{
		SAFE_RELEASE((*iter).second);
	}

	_RSStateMap.clear();
}


void FEDX11Renderer::RasterStateLoad()
{
	RasterStateCreate(FE_RS_SOLID);
	RasterStateCreate(FE_RS_CULL_FRONT);
	RasterStateCreate(FE_RS_CULL_BACK);
	RasterStateCreate(FE_RS_WIREFRAME);
	RasterStateCreate(FE_RS_WIREFRAME | FE_RS_CULL_FRONT);
	RasterStateCreate(FE_RS_WIREFRAME | FE_RS_CULL_BACK);
}


void FEDX11Renderer::DSStateCreate(DWORD flag)
{
	D3D11_DEPTH_STENCIL_DESC ds;
	ds.DepthEnable = flag & 0x80000000 ? FALSE : TRUE;
	ds.DepthWriteMask = flag & 0x40000000 ? D3D11_DEPTH_WRITE_MASK_ZERO : D3D11_DEPTH_WRITE_MASK_ALL;
	switch (flag & 0x38000000)
	{
	case FE_DS_DEPTH_COMPARISON_NEVER:
		ds.DepthFunc = D3D11_COMPARISON_NEVER;
		break;
	case FE_DS_DEPTH_COMPARISON_LESS:
		ds.DepthFunc = D3D11_COMPARISON_LESS;
		break;
	case FE_DS_DEPTH_COMPARISON_EQUAL:
		ds.DepthFunc = D3D11_COMPARISON_EQUAL;
		break;
	case FE_DS_DEPTH_COMPARISON_LESS_EQUAL:
		ds.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		break;
	case FE_DS_DEPTH_COMPARISON_GREATER:
		ds.DepthFunc = D3D11_COMPARISON_GREATER;
		break;
	case FE_DS_DEPTH_COMPARISON_NOT_EQUAL:
		ds.DepthFunc = D3D11_COMPARISON_NOT_EQUAL;
		break;
	case FE_DS_DEPTH_COMPARISON_GREATER_EQUAL:
		ds.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
		break;
	case FE_DS_DEPTH_COMPARISON_ALWAYS:
		ds.DepthFunc = D3D11_COMPARISON_LESS;
		break;
	}

	//----------------------------------------------------------------------
	// ���ٽ� ���� ���� ��ü�� ����.��
	//----------------------------------------------------------------------
	// ���ٽ� ���� ��Ʈ ���� ����.
	// (Stencil.Ref & Stencil.Mask) Comparison-Func ( StencilBuffer.Value & Stencil.Mask)
	//
	// *StencilBufferValue : ���� �˻��� �ȼ��� ���ٽǰ�.
	// *ComFunc : �� �Լ�. ( > < >= <= ==  Always Never)
	//----------------------------------------------------------------------

	// ���ٽ� ���� ����
	ds.StencilEnable = flag & 0x04000000 ? TRUE : FALSE;		// ���ٽ� ���۸� ��� �� ���ΰ�.
	ds.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;		// ���ٽ� �б� ����ũ (8bit: 0xff)
	ds.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;		// ���ٽ� ���� ����ũ (8bit: 0xff)

	// [�ո�] ���ٽ� �� ���н� ����
	switch (flag & 0x03800000)
	{
	case FE_DS_STENCIL_FRONT_FAIL_OP_KEEP:
		ds.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		break;
	case FE_DS_STENCIL_FRONT_FAIL_OP_ZERO:
		ds.FrontFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
		break;
	case FE_DS_STENCIL_FRONT_FAIL_OP_REPLACE:
		ds.FrontFace.StencilFailOp = D3D11_STENCIL_OP_REPLACE;
		break;
	case FE_DS_STENCIL_FRONT_FAIL_OP_INCR_SAT:
		ds.FrontFace.StencilFailOp = D3D11_STENCIL_OP_INCR_SAT;
		break;
	case FE_DS_STENCIL_FRONT_FAIL_OP_DECR_SAT:
		ds.FrontFace.StencilFailOp = D3D11_STENCIL_OP_DECR_SAT;
		break;
	case FE_DS_STENCIL_FRONT_FAIL_OP_INVERT:
		ds.FrontFace.StencilFailOp = D3D11_STENCIL_OP_INVERT;
		break;
	case FE_DS_STENCIL_FRONT_FAIL_OP_INCR:
		ds.FrontFace.StencilFailOp = D3D11_STENCIL_OP_INCR;
		break;
	case FE_DS_STENCIL_FRONT_FAIL_OP_DECR:
		ds.FrontFace.StencilFailOp = D3D11_STENCIL_OP_DECR;
		break;
	}
	// [�ո�] ���ٽ�/���� �� ���н� ����
	switch (flag & 0x00700000)
	{
	case FE_DS_STENCIL_FRONT_DEPTH_FAIL_OP_KEEP:
		ds.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		break;
	case FE_DS_STENCIL_FRONT_DEPTH_FAIL_OP_ZERO:
		ds.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
		break;
	case FE_DS_STENCIL_FRONT_DEPTH_FAIL_OP_REPLACE:
		ds.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_REPLACE;
		break;
	case FE_DS_STENCIL_FRONT_DEPTH_FAIL_OP_INCR_SAT:
		ds.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR_SAT;
		break;
	case FE_DS_STENCIL_FRONT_DEPTH_FAIL_OP_DECR_SAT:
		ds.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR_SAT;
		break;
	case FE_DS_STENCIL_FRONT_DEPTH_FAIL_OP_INVERT:
		ds.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INVERT;
		break;
	case FE_DS_STENCIL_FRONT_DEPTH_FAIL_OP_INCR:
		ds.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		break;
	case FE_DS_STENCIL_FRONT_DEPTH_FAIL_OP_DECR:
		ds.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		break;
	}
	// [�ո�] ���ٽ� �� ������ ����
	switch (flag & 0x000E0000)
	{
	case FE_DS_STENCIL_FRONT_PASS_OP_KEEP:
		ds.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		break;
	case FE_DS_STENCIL_FRONT_PASS_OP_ZERO:
		ds.FrontFace.StencilPassOp = D3D11_STENCIL_OP_ZERO;
		break;
	case FE_DS_STENCIL_FRONT_PASS_OP_REPLACE:
		ds.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		break;
	case FE_DS_STENCIL_FRONT_PASS_OP_INCR_SAT:
		ds.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;
		break;
	case FE_DS_STENCIL_FRONT_PASS_OP_DECR_SAT:
		ds.FrontFace.StencilPassOp = D3D11_STENCIL_OP_DECR_SAT;
		break;
	case FE_DS_STENCIL_FRONT_PASS_OP_INVERT:
		ds.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INVERT;
		break;
	case FE_DS_STENCIL_FRONT_PASS_OP_INCR:
		ds.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
		break;
	case FE_DS_STENCIL_FRONT_PASS_OP_DECR:
		ds.FrontFace.StencilPassOp = D3D11_STENCIL_OP_DECR;
		break;
	}
	// [�ո�] ���ٽ� �� �Լ�
	switch (flag & 0x0001C000)
	{
	case FE_DS_STENCIL_FRONT_COMPARISON_NEVER:
		ds.FrontFace.StencilFunc = D3D11_COMPARISON_NEVER;
		break;
	case FE_DS_STENCIL_FRONT_COMPARISON_LESS:
		ds.FrontFace.StencilFunc = D3D11_COMPARISON_LESS;
		break;
	case FE_DS_STENCIL_FRONT_COMPARISON_EQUAL:
		ds.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
		break;
	case FE_DS_STENCIL_FRONT_COMPARISON_LESS_EQUAL:
		ds.FrontFace.StencilFunc = D3D11_COMPARISON_LESS_EQUAL;
		break;
	case FE_DS_STENCIL_FRONT_COMPARISON_GREATER:
		ds.FrontFace.StencilFunc = D3D11_COMPARISON_GREATER;
		break;
	case FE_DS_STENCIL_FRONT_COMPARISON_NOT_EQUAL:
		ds.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
		break;
	case FE_DS_STENCIL_FRONT_COMPARISON_GREATER_EQUAL:
		ds.FrontFace.StencilFunc = D3D11_COMPARISON_GREATER_EQUAL;
		break;
	case FE_DS_STENCIL_FRONT_COMPARISON_ALWAYS:
		ds.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		break;
	}
	// [�޸�] ���ٽ� �� ���н� ����
	switch (flag & 0x00003800)
	{
	case FE_DS_STENCIL_BACK_FAIL_OP_KEEP:
		ds.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		break;
	case FE_DS_STENCIL_BACK_FAIL_OP_ZERO:
		ds.BackFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
		break;
	case FE_DS_STENCIL_BACK_FAIL_OP_REPLACE:
		ds.BackFace.StencilFailOp = D3D11_STENCIL_OP_REPLACE;
		break;
	case FE_DS_STENCIL_BACK_FAIL_OP_INCR_SAT:
		ds.BackFace.StencilFailOp = D3D11_STENCIL_OP_INCR_SAT;
		break;
	case FE_DS_STENCIL_BACK_FAIL_OP_DECR_SAT:
		ds.BackFace.StencilFailOp = D3D11_STENCIL_OP_DECR_SAT;
		break;
	case FE_DS_STENCIL_BACK_FAIL_OP_INVERT:
		ds.BackFace.StencilFailOp = D3D11_STENCIL_OP_INVERT;
		break;
	case FE_DS_STENCIL_BACK_FAIL_OP_INCR:
		ds.BackFace.StencilFailOp = D3D11_STENCIL_OP_INCR;
		break;
	case FE_DS_STENCIL_BACK_FAIL_OP_DECR:
		ds.BackFace.StencilFailOp = D3D11_STENCIL_OP_DECR;
		break;
	}
	// [�޸�] ���ٽ�/���� �� ���н� ����
	switch (flag & 0x00000700)
	{
	case FE_DS_STENCIL_BACK_DEPTH_FAIL_OP_KEEP:
		ds.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		break;
	case FE_DS_STENCIL_BACK_DEPTH_FAIL_OP_ZERO:
		ds.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
		break;
	case FE_DS_STENCIL_BACK_DEPTH_FAIL_OP_REPLACE:
		ds.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_REPLACE;
		break;
	case FE_DS_STENCIL_BACK_DEPTH_FAIL_OP_INCR_SAT:
		ds.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR_SAT;
		break;
	case FE_DS_STENCIL_BACK_DEPTH_FAIL_OP_DECR_SAT:
		ds.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR_SAT;
		break;
	case FE_DS_STENCIL_BACK_DEPTH_FAIL_OP_INVERT:
		ds.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INVERT;
		break;
	case FE_DS_STENCIL_BACK_DEPTH_FAIL_OP_INCR:
		ds.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		break;
	case FE_DS_STENCIL_BACK_DEPTH_FAIL_OP_DECR:
		ds.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		break;
	}
	// [�޸�] ���ٽ� �� ������ ����
	switch (flag & 0x000000E0)
	{
	case FE_DS_STENCIL_BACK_PASS_OP_KEEP:
		ds.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		break;
	case FE_DS_STENCIL_BACK_PASS_OP_ZERO:
		ds.BackFace.StencilPassOp = D3D11_STENCIL_OP_ZERO;
		break;
	case FE_DS_STENCIL_BACK_PASS_OP_REPLACE:
		ds.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		break;
	case FE_DS_STENCIL_BACK_PASS_OP_INCR_SAT:
		ds.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;
		break;
	case FE_DS_STENCIL_BACK_PASS_OP_DECR_SAT:
		ds.BackFace.StencilPassOp = D3D11_STENCIL_OP_DECR_SAT;
		break;
	case FE_DS_STENCIL_BACK_PASS_OP_INVERT:
		ds.BackFace.StencilPassOp = D3D11_STENCIL_OP_INVERT;
		break;
	case FE_DS_STENCIL_BACK_PASS_OP_INCR:
		ds.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
		break;
	case FE_DS_STENCIL_BACK_PASS_OP_DECR:
		ds.BackFace.StencilPassOp = D3D11_STENCIL_OP_DECR;
		break;
	}
	// [�޸�] ���ٽ� �� �Լ�
	switch (flag & 0x0000001C)
	{
	case FE_DS_STENCIL_BACK_COMPARISON_NEVER:
		ds.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;
		break;
	case FE_DS_STENCIL_BACK_COMPARISON_LESS:
		ds.BackFace.StencilFunc = D3D11_COMPARISON_LESS;
		break;
	case FE_DS_STENCIL_BACK_COMPARISON_EQUAL:
		ds.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;
		break;
	case FE_DS_STENCIL_BACK_COMPARISON_LESS_EQUAL:
		ds.BackFace.StencilFunc = D3D11_COMPARISON_LESS_EQUAL;
		break;
	case FE_DS_STENCIL_BACK_COMPARISON_GREATER:
		ds.BackFace.StencilFunc = D3D11_COMPARISON_GREATER;
		break;
	case FE_DS_STENCIL_BACK_COMPARISON_NOT_EQUAL:
		ds.BackFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
		break;
	case FE_DS_STENCIL_BACK_COMPARISON_GREATER_EQUAL:
		ds.BackFace.StencilFunc = D3D11_COMPARISON_GREATER_EQUAL;
		break;
	case FE_DS_STENCIL_BACK_COMPARISON_ALWAYS:
		ds.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		break;
	}

	_pDevice->CreateDepthStencilState(&ds, &_DSStateMap[flag]);
}


void FEDX11Renderer::DSStateRelease()
{
	FOR_STL(_DSStateMap)
	{
		SAFE_RELEASE((*iter).second);
	}

	_DSStateMap.clear();
}


void FEDX11Renderer::DSStateLoad()
{
	//----------------------------------------------------------------
	// ����/���ٽ� ���� ��ü ���� : "��º��ձ� Output Merger" ���� ����. 
	//----------------------------------------------------------------
	DSStateCreate(0);
	DSStateCreate(FE_DS_DEPTH_TEST_OFF);
	DSStateCreate(FE_DS_DEPTH_WRITE_OFF);
	DSStateCreate(FE_DS_DEPTH_TEST_OFF | FE_DS_DEPTH_WRITE_OFF);
	DSStateCreate(FE_DS_STENCIL_ON | FE_DS_STENCIL_FRONT_PASS_OP_REPLACE | FE_DS_STENCIL_BACK_PASS_OP_REPLACE);
	DSStateCreate(FE_DS_STENCIL_ON | FE_DS_STENCIL_FRONT_COMPARISON_EQUAL | FE_DS_STENCIL_BACK_COMPARISON_EQUAL);
	DSStateCreate(FE_DS_STENCIL_ON | FE_DS_STENCIL_FRONT_COMPARISON_NOT_EQUAL | FE_DS_STENCIL_BACK_COMPARISON_NOT_EQUAL);
}


void FEDX11Renderer::SetRSState(BYTE flag)
{
	if (_RSStateMap.find(flag) == _RSStateMap.end())
	{
		RasterStateCreate(flag);
	}

	_pDXDC->RSSetState(_RSStateMap[flag]);
}


void FEDX11Renderer::SetDSState(DWORD flag, UINT stencilRef)
{
	if (_DSStateMap.find(flag) == _DSStateMap.end())
	{
		DSStateCreate(flag);
	}

	_pDXDC->OMSetDepthStencilState(_DSStateMap[flag], stencilRef);
}


void FEDX11Renderer::SetViewports(UINT NumViewports, const FEViewport *pViewports)
{
	D3D11_VIEWPORT* viewports = new D3D11_VIEWPORT[NumViewports];
	
	for (UINT i = 0; i < NumViewports; i++)
	{
		viewports[i].TopLeftX = pViewports[i].TopLeftX;
		viewports[i].TopLeftY = pViewports[i].TopLeftY;
		viewports[i].Width = pViewports[i].Width;
		viewports[i].Height = pViewports[i].Height;
		viewports[i].MinDepth = pViewports[i].MinDepth;
		viewports[i].MaxDepth = pViewports[i].MaxDepth;
	}

	_pDXDC->RSSetViewports(NumViewports, viewports);
}


void FEDX11Renderer::SetVertexBuffer(UINT StartSlot, UINT NumBuffers, const IFEBuffer* ppVertexBuffers, const UINT* pStrides, const UINT* pOffsets) const
{
	ID3D11Buffer* pVB;
	if (ppVertexBuffers == nullptr)	pVB = nullptr;
	else	pVB = static_cast<const FEDX11Buffer*>(ppVertexBuffers)->GetBuffer();
	_pDXDC->IASetVertexBuffers(StartSlot, NumBuffers, &pVB, pStrides, pOffsets);
}


void FEDX11Renderer::SetIndexBuffer(const IFEBuffer* pIndexBuffer, FEGI_FORMAT Format, UINT Offset) const
{
	_pDXDC->IASetIndexBuffer(static_cast<const FEDX11Buffer*>(pIndexBuffer)->GetBuffer(), (DXGI_FORMAT)Format, Offset);		// �ε��� ���� ����
}


void FEDX11Renderer::SetPrimitiveTopology(FE_PRIMITIVE_TOPOLOGY Topology) const
{
	_pDXDC->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)Topology);
}


void FEDX11Renderer::ClearBackBuffer(const FEVector4& i_color) const
{
	_pDXDC->ClearRenderTargetView(_pRTView, (float*)&i_color);			//����Ÿ�� �����.
	_pDXDC->ClearDepthStencilView(_pDSView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);	//����/���ٽ� �����.
}


void FEDX11Renderer::Draw(UINT VertexCount, UINT StartVertexLocation) const
{
	_pDXDC->Draw(VertexCount, StartVertexLocation);
}


void FEDX11Renderer::DrawIndexed(UINT IndexCount, UINT StartIndexLocation, UINT BaseVertexLocation) const
{
	_pDXDC->DrawIndexed(IndexCount, StartIndexLocation, BaseVertexLocation);
}


void FEDX11Renderer::Flip() const
{
	_pSwapChain->Present(0, 0);			//ȭ����� : Flip!
}


ID3D11Device* FEDX11Renderer::GetDevice() const
{
	return _pDevice;
}


ID3D11DeviceContext* FEDX11Renderer::GetDXDC() const
{
	return _pDXDC;
}
#endif