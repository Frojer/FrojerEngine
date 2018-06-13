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
	
	//D3D ��� ���� (Direct3D feature level) 
	//���� ����� DX ���� ����. DX ������ ��ġ�� ȣȯ�� ���
	D3D_FEATURE_LEVEL _featureLevels;// = D3D_FEATURE_LEVEL_11_0;		// DX11 ����.
	//D3D_FEATURE_LEVEL featureLevels = D3D_FEATURE_LEVEL_9_3;			// DX9.0c ����.
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