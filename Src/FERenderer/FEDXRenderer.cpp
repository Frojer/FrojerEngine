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
	// 1단계 : 렌더링 장치 관련 필수 설정
	//----------------------------------------
	// D3D 렌더링 장치 Device 및 스왑체인 Swap Chain 생성. 
	CreateDeviceSwapChain(hWnd);

	// 장치-스왑체인의 렌더타겟(백버퍼) 획득
	CreateRenderTarget();
	
	
	// 장치 출력병합기(Output Merger) 에 렌터링 타겟 및 깊이-스텐실 버퍼 등록.
	_pDXDC->OMSetRenderTargets(
				1,				// 렌더타겟 개수.(max: D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)
				&_pRTView,		// 렌더타겟("백버퍼") 등록.	
				nullptr
				);
	
	// 뷰포트 설정.
	SetViewPort();

	return true;
}


void FEDXRenderer::Release()
{
	//장치 상태 리셋 : 제거 전에 초기화를 해야 합니다. (메모리 누수 방지)
	if (_pDXDC) _pDXDC->ClearState();
	//if (g_pSwapChain) g_pSwapChain->SetFullscreenState(false, NULL);


	SAFE_RELEASE(_pRTView);			//렌더타겟 제거.
	SAFE_RELEASE(_pSwapChain);			//스왑체인 제거.
	SAFE_RELEASE(_pDXDC);
	SAFE_RELEASE(_pDevice);			//디바이스 제거. 맨 나중에 제거합니다.
}


/////////////////////////////////////////////////////////////////////////////
//
//  D3D 렌더링 '장치(Device)' 및 스왑체인 생성. 
//
HRESULT FEDXRenderer::CreateDeviceSwapChain(HWND i_hWnd)
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
	sd.BufferDesc.Width = m_Setting.uiWindowWidth;	// 해상도 결정.(백버퍼 크기)
	sd.BufferDesc.Height = m_Setting.uiWindowHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		// 백버퍼 색상규격 (A8R8G8B8) 창모드에서는 생략 가능 
	sd.BufferDesc.RefreshRate.Numerator = 0;			    // 버퍼 갱신율.(수직동기화 VSync 활성화시 표준갱신율 적용 : 60hz)
	//sd.BufferDesc.RefreshRate.Numerator = 0;				// 버퍼 갱신율.(수직동기화 VSync Off)
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		// 용도 설정: '렌더타겟' 
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	//sd.Flags = 0;
	sd.SampleDesc.Count = 1;	// AA 설정
	sd.SampleDesc.Quality = 0;



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
HRESULT FEDXRenderer::CreateRenderTarget()
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
	_pDXDC->ClearRenderTargetView(_pRTView, (float*)&i_color);			//렌더타겟 지우기.
}


void FEDXRenderer::Flip()
{
	_pSwapChain->Present(0, 0);			//화면출력 : Flip!
}