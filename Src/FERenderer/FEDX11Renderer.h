#pragma once
#ifndef _FE_DX11_RENDERER
#define _FE_DX11_RENDERER

#include "IFERenderer.h"
#ifdef FE_DX11
#include "FEDX11Shader.h"
#include <unordered_map>

// DX 표준 헤더 
#include <d3d11.h>					// DX 표준 헤더. (DX 11.0)
//#include <d3d11_1.h>				// DX 표준 헤더. (DX 11.1)
#pragma comment(lib, "d3d11")		// DX 라이브러리 로딩.  d3d11.dll 필요.

/////////////////////////////////////////////////////////////////////
//
//  DXGI : DirectX Graphics Infrastructure 
//  DX 의 버전에 독립적인 하드웨어 저수준 접근 방식을 제공합니다.
//
/////////////////////////////////////////////////////////////////////
//#include <dxgi.h>					// DXGI 헤더. (d3d11.h 에 포함됨)
#pragma comment(lib, "dxgi")		// DXGI 라이브러리. DXGI.dll 필요.

class FEDX11Renderer : public IFERenderer
{
private:
	ID3D11Device*				_pDevice;
	ID3D11DeviceContext*		_pDXDC;
	IDXGISwapChain*				_pSwapChain;
	ID3D11RenderTargetView*		_pRTView;
	
	//D3D 기능 레벨 (Direct3D feature level) 
	//현재 사용할 DX 버전 지정. DX 렌더링 장치의 호환성 향상
	D3D_FEATURE_LEVEL _featureLevels;// = D3D_FEATURE_LEVEL_11_0;		// DX11 대응.
	//D3D_FEATURE_LEVEL featureLevels = D3D_FEATURE_LEVEL_9_3;			// DX9.0c 대응.

	//깊이 스텐실 버퍼 관련.
	ID3D11Texture2D*		 _pDS;			// 깊이-스텐실 버퍼.
	ID3D11DepthStencilView*  _pDSView;		// 깊이-스텐실 뷰.

	// 래스터라이저 상태객체
	std::unordered_map<BYTE, ID3D11RasterizerState*> _RSStateMap;
	// 깊이/스텐실 버퍼 상태객체.
	std::unordered_map<DWORD, ID3D11DepthStencilState*> _DSStateMap;
	// BlendState 상태객체.
	std::vector<std::pair<FE_BLEND_DESC, ID3D11BlendState*> > _BlendStateArr;
	// SamplerState 상태객체
	std::vector<std::pair<FE_SAMPLER_STATE_FLAG, ID3D11SamplerState*> > _smpStateArr;

private:
	virtual bool Create(void* i_phWnd) override;
	void Release();

	HRESULT CreateDeviceSwapChain(HWND i_hWnd);
	HRESULT CreateRenderTarget();
	HRESULT CreateDepthStencil();
	void	SetViewPort() const;

	// 레스터 상태객체
	void RasterStateCreate(BYTE flag);
	void RasterStateRelease();
	void RasterStateLoad();

	// 깊이/스텐실 버퍼 상태객체
	void DSStateCreate(DWORD flag);
	void DSStateRelease();
	void DSStateLoad();

	// 깊이/스텐실 버퍼 상태객체
	ID3D11BlendState* BlendStateCreate(const FE_BLEND_DESC& desc);
	void BlendStateRelease();
	void BlendStateLoad();

public:
	FEDX11Renderer();
	virtual ~FEDX11Renderer();

	virtual void SetRSState(BYTE flag) override;
	virtual void SetDSState(DWORD flag, UINT stencilRef) override;
	virtual void SetBlendState(const FE_BLEND_DESC& desc) override;

	virtual void SetViewports(UINT NumViewports, const FEViewport *pViewports) override;

	virtual void SetVertexBuffer(UINT StartSlot, UINT NumBuffers, const IFEBuffer* ppVertexBuffers, const UINT* pStrides, const UINT* pOffsets) const override;
	virtual void SetIndexBuffer(const IFEBuffer* pIndexBuffer, FEGI_FORMAT Format, UINT Offset) const override;
	virtual void SetPrimitiveTopology(FE_PRIMITIVE_TOPOLOGY Topology) const override;

	virtual void ClearBackBuffer(const FEVector4& i_color) const override;
	virtual void Draw(UINT VertexCount, UINT StartVertexLocation) const override;
	virtual void DrawIndexed(UINT IndexCount, UINT StartIndexLocation, UINT BaseVertexLocation) const override;
	virtual void Flip() const override;

	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDXDC() const;

	friend class FEDX11Shader;
};
#endif
#endif