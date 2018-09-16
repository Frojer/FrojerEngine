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
	// 1단계 : 렌더링 장치 관련 필수 설정
	//----------------------------------------
	// D3D 렌더링 장치 Device 및 스왑체인 Swap Chain 생성. 
	CreateDeviceSwapChain(hWnd);

	// 장치-스왑체인의 렌더타겟(백버퍼) 획득
	CreateRenderTarget();

	// 깊이/스텐실 버퍼 생성.
	CreateDepthStencil();
	
	
	// 장치 출력병합기(Output Merger) 에 렌터링 타겟 및 깊이-스텐실 버퍼 등록.
	_pDXDC->OMSetRenderTargets(
				1,				// 렌더타겟 개수.(max: D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)
				&_pRTView,		// 렌더타겟("백버퍼") 등록.	
				_pDSView
				);
	
	// 뷰포트 설정.
	SetViewPort();

	//깊이-스텐실 렌더링 상태 객체 생성.
	//StateObjectCreate();

	//블렌드 상태 객체 생성
	//BlendStateCreate();

	return true;
}


void FEDX11Renderer::Release()
{
	//CTexture2D::SamplerRelease();
	RasterStateRelease();
	DSStateRelease();
	//BlendStateRelease();

	//장치 상태 리셋 : 제거 전에 초기화를 해야 합니다. (메모리 누수 방지)
	if (_pDXDC) _pDXDC->ClearState();
	//if (g_pSwapChain) g_pSwapChain->SetFullscreenState(false, NULL);

	SAFE_RELEASE(_pDS);				// 깊이/스텐실 버퍼 제거.
	SAFE_RELEASE(_pDSView);
	SAFE_RELEASE(_pRTView);			// 렌더타겟 제거.
	SAFE_RELEASE(_pSwapChain);		// 스왑체인 제거.
	SAFE_RELEASE(_pDXDC);
	SAFE_RELEASE(_pDevice);			// 디바이스 제거. 맨 나중에 제거합니다.
}


/////////////////////////////////////////////////////////////////////////////
//
//  D3D 렌더링 '장치(Device)' 및 스왑체인 생성. 
//
HRESULT FEDX11Renderer::CreateDeviceSwapChain(HWND i_hWnd)
{
	HRESULT hr = S_OK;

	// 장치 (Device) 및 스왑체인(SwapChain) 정보 구성.
	// 스왑체인은 다중버퍼링 시스템을 말하며
	// 고전적인 '플립핑Flipping' 체인과 동일한 의미입니다.  
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.Windowed = TRUE;					// 풀스크린 또는 창모드 선택.
	sd.OutputWindow = i_hWnd;			// 출력할 윈도우 핸들.
	sd.BufferCount = 1;					// 백버퍼 개수.
	sd.BufferDesc.Width = m_setting.uiWindowWidth;	// 해상도 결정.(백버퍼 크기)
	sd.BufferDesc.Height = m_setting.uiWindowHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		// 백버퍼 색상규격 (A8R8G8B8) 창모드에서는 생략 가능 
	sd.BufferDesc.RefreshRate.Numerator = 0;			    // 버퍼 갱신율.(수직동기화 VSync 활성화시 표준갱신율 적용 : 60hz)
	//sd.BufferDesc.RefreshRate.Numerator = 0;				// 버퍼 갱신율.(수직동기화 VSync Off)
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		// 용도 설정: '렌더타겟' 
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	//sd.Flags = 0;
	sd.SampleDesc.Count = m_setting.SampleCount;	// AA 설정
	sd.SampleDesc.Quality = m_setting.SampleQuality;



	// D3D 렌더링 '장치(Device)' 및 스왑체인 생성. 
	//
	hr = D3D11CreateDeviceAndSwapChain(
		NULL,						// 비디오 어뎁터 포인터 (기본장치는 NULL)
		D3D_DRIVER_TYPE_HARDWARE,	// HW 가속 
		NULL,						// SW Rasterizer DLL 핸들.  HW 가속시에는 NULL.
		0,							// 디바이스 생성 플래그.(기본값)
		// D3D11_CREATE_DEVICE_DEBUG,//디바이스 생성 플래그.(디버그)
		&_featureLevels,			// (생성할) 디바이스 기능 레벨(Feature Level) 배열
		1,							// (생성할) 디바이스 기능 레벨(Feature Level) 배열 크기.
		D3D11_SDK_VERSION,			// DX SDK 버전.
		&sd,						// 디바이스 생성 상세 옵션.
		&_pSwapChain,				// [출력] 스왑체인 인터페이스 얻기.
		&_pDevice,					// [출력] 디바이스 인터페이스 얻기.
		NULL,						// [출력] (생성된) 디바이스 기능 레벨. 필요없다면 NULL 설정. 
		&_pDXDC						// [출력] 디바이스 컨텍스트 얻기.
	);

	if (FAILED(hr))
	{
		// 디바이스 / 스왑체인 생성 실패
		return hr;
	}

	return hr;
}


/////////////////////////////////////////////////////////////////////////////
//
// 장치-스왑체인의 렌더타겟(백버퍼) 획득
//
// 리소스뷰(Resource View)  
// DX11 의 리소스는 "공유" 목적으로 유연하게 설계되어 있습니다. 
// (메모리 관리의 효율성 증대 및 다용도 활용이 가능..)
// 따라서 사용전 각 용도에 맞는 접근방식(인터페이스)를 획득(생성)해야 합니다.
// 이 작업을 "리소스 뷰 Resource View" 를 통해 처리합니다.
//                     
// 스왑체인에 연결(Bind)된 렌더링 출력버퍼 - "백버퍼 BackBuffer" 는 2D 기반 리소스(텍스처) 이며
// "렌더타겟 RenderTarget" 용으로 리소스뷰를 생성, 해당 버퍼를 접근/운용 하겠습니다.
//
HRESULT FEDX11Renderer::CreateRenderTarget()
{
	HRESULT hr = S_OK;

	// 백버퍼 획득.
	ID3D11Texture2D *pBackBuffer;
	hr = _pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);	// 백버퍼 얻기, 2D 텍스쳐 행태, 화면출력용
	if (FAILED(hr))	return hr;

	// 획득한 백버퍼에 렌더타겟 뷰 생성.(렌더타겟'형'으로 설정함)
	hr = _pDevice->CreateRenderTargetView(pBackBuffer, NULL, &_pRTView);		// '렌더타겟뷰'를 생성.

	if (FAILED(hr))
	{
		// 백버퍼-렌더타겟뷰 생성 실패
	}

	//리소스 뷰 생성 후, 불필요한 DX 핸들은 해제해야 합니다.(메모리 누수 방지)
	SAFE_RELEASE(pBackBuffer);

	return hr;
}


HRESULT FEDX11Renderer::CreateDepthStencil()
{
	HRESULT hr = S_OK;

	//깊이/스텐실 버퍼용 정보 구성.
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(td));
	td.Width = m_setting.uiWindowWidth;
	td.Height = m_setting.uiWindowHeight;
	td.MipLevels = 1;
	td.ArraySize = 1;
	//td.Format = DXGI_FORMAT_D32_FLOAT;				// 32BIT. 깊이 버퍼
	//td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;		// 깊이 버퍼 (24bit) + 스텐실 (8bit) / 구형 하드웨어 (DX9)
	td.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;		// 깊이 버퍼 (32bit) + 스텐실 (8bit) / 신형 하드웨어 (DX11)
	td.SampleDesc.Count = m_setting.SampleCount;		// AA 설정 - RT 과 동일 규격 준수.
	td.SampleDesc.Quality = m_setting.SampleQuality;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;			// 깊이-스텐실 버퍼용으로 설정.
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;


	// 깊이 버퍼 생성.
	hr = _pDevice->CreateTexture2D(&td, NULL, &_pDS);
	if (FAILED(hr)) return hr;


	// 깊이-스텐실버퍼용 리소스 뷰 정보 설정. 
	D3D11_DEPTH_STENCIL_VIEW_DESC  dd;
	ZeroMemory(&dd, sizeof(dd));
	dd.Format = td.Format;
	//dd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D; //AA 없음.
	dd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS; //AA 적용.
	dd.Texture2D.MipSlice = 0;

	//깊이-스텐실 버퍼 뷰 생성.
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
	// 렌더링 상태 객체
	D3D11_RASTERIZER_DESC rd;

	// 삼각형 색상 채우기
	rd.FillMode = flag & 0x80 ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;

	// 백페이스 컬링
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
	// 스텐실 버퍼 연산 객체들 생성.★
	//----------------------------------------------------------------------
	// 스텐실 버퍼 비트 연산 공식.
	// (Stencil.Ref & Stencil.Mask) Comparison-Func ( StencilBuffer.Value & Stencil.Mask)
	//
	// *StencilBufferValue : 현재 검사할 픽셀의 스텐실값.
	// *ComFunc : 비교 함수. ( > < >= <= ==  Always Never)
	//----------------------------------------------------------------------

	// 스텐실 버퍼 설정
	ds.StencilEnable = flag & 0x04000000 ? TRUE : FALSE;		// 스텐실 버퍼를 사용 할 것인가.
	ds.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;		// 스텐실 읽기 마스크 (8bit: 0xff)
	ds.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;		// 스텐실 쓰기 마스크 (8bit: 0xff)

	// [앞면] 스텐실 비교 실패시 동작
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
	// [앞면] 스텐실/깊이 비교 실패시 동작
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
	// [앞면] 스텐실 비교 성공시 동작
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
	// [앞면] 스텐실 비교 함수
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
	// [뒷면] 스텐실 비교 실패시 동작
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
	// [뒷면] 스텐실/깊이 비교 실패시 동작
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
	// [뒷면] 스텐실 비교 성공시 동작
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
	// [뒷면] 스텐실 비교 함수
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
	// 깊이/스텐실 상태 개체 생성 : "출력병합기 Output Merger" 상태 조절. 
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
	_pDXDC->IASetIndexBuffer(static_cast<const FEDX11Buffer*>(pIndexBuffer)->GetBuffer(), (DXGI_FORMAT)Format, Offset);		// 인덱스 버퍼 세팅
}


void FEDX11Renderer::SetPrimitiveTopology(FE_PRIMITIVE_TOPOLOGY Topology) const
{
	_pDXDC->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)Topology);
}


void FEDX11Renderer::ClearBackBuffer(const FEVector4& i_color) const
{
	_pDXDC->ClearRenderTargetView(_pRTView, (float*)&i_color);			//렌더타겟 지우기.
	_pDXDC->ClearDepthStencilView(_pDSView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);	//깊이/스텐실 지우기.
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
	_pSwapChain->Present(0, 0);			//화면출력 : Flip!
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