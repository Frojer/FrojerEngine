#pragma once

#include "IFERenderer.h"
#include "FEDXRendererDefine.h"

class FEDXRenderer : public IFERenderer
{
private:
	LPDEVICE		_pDevice;
	LPDXDC			_pDXDC;
	LPSWAPCHAIN		_pSwapChain;
	LPRTVIEW		_pRTView;
	
	//D3D 기능 레벨 (Direct3D feature level) 
	//현재 사용할 DX 버전 지정. DX 렌더링 장치의 호환성 향상
	D3D_FEATURE_LEVEL _featureLevels;// = D3D_FEATURE_LEVEL_11_0;		// DX11 대응.
	//D3D_FEATURE_LEVEL featureLevels = D3D_FEATURE_LEVEL_9_3;			// DX9.0c 대응.
private:
	virtual bool Create(void* i_phWnd) override;
	virtual void Release() override;

	HRESULT CreateDeviceSwapChain(HWND i_hWnd);
	HRESULT CreateRenderTarget();
	void	SetViewPort();

public:
	FEDXRenderer();
	virtual ~FEDXRenderer();

	virtual void ClearBackBuffer(const FEVector4& i_color) override;
	virtual void Flip() override;
};