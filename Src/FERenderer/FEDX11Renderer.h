#pragma once
#ifndef _FE_DX11_RENDERER
#define _FE_DX11_RENDERER

#include "IFERenderer.h"
#include <unordered_map>

// DX ǥ�� ��� 
#include <d3d11.h>					// DX ǥ�� ���. (DX 11.0)
//#include <d3d11_1.h>				// DX ǥ�� ���. (DX 11.1)
#pragma comment(lib, "d3d11")		// DX ���̺귯�� �ε�.  d3d11.dll �ʿ�.

/////////////////////////////////////////////////////////////////////
//
//  DXGI : DirectX Graphics Infrastructure 
//  DX �� ������ �������� �ϵ���� ������ ���� ����� �����մϴ�.
//
/////////////////////////////////////////////////////////////////////
//#include <dxgi.h>					// DXGI ���. (d3d11.h �� ���Ե�)
#pragma comment(lib, "dxgi")		// DXGI ���̺귯��. DXGI.dll �ʿ�.

class FEDX11Renderer : public IFERenderer
{
private:
	ID3D11Device*				_pDevice;
	ID3D11DeviceContext*		_pDXDC;
	IDXGISwapChain*				_pSwapChain;
	ID3D11RenderTargetView*		_pRTView;
	
	//D3D ��� ���� (Direct3D feature level) 
	//���� ����� DX ���� ����. DX ������ ��ġ�� ȣȯ�� ���
	D3D_FEATURE_LEVEL _featureLevels;// = D3D_FEATURE_LEVEL_11_0;		// DX11 ����.
	//D3D_FEATURE_LEVEL featureLevels = D3D_FEATURE_LEVEL_9_3;			// DX9.0c ����.

	//���� ���ٽ� ���� ����.
	ID3D11Texture2D*		 _pDS;			// ����-���ٽ� ����.
	ID3D11DepthStencilView*  _pDSView;		// ����-���ٽ� ��.

	//���� ��ü �迭 : "��ɺ�" �׷����� �����մϴ�.
	std::unordered_map<BYTE, ID3D11RasterizerState*> _RSStateMap;
	// ����/���ٽ� ���� ���°�ü.	
	std::unordered_map<DWORD, ID3D11DepthStencilState*> _DSStateMap;

private:
	virtual bool Create(void* i_phWnd) override;
	void Release();

	HRESULT CreateDeviceSwapChain(HWND i_hWnd);
	HRESULT CreateRenderTarget();
	void	SetViewPort() const;

	// ������ ���°�ü
	void RasterStateCreate(BYTE flag);
	void RasterStateRelease();
	void RasterStateLoad();

	// ����/���ٽ� ���� ���°�ü
	void DSStateCreate(DWORD flag);
	void DSStateRelease();
	void DSStateLoad();

public:
	FEDX11Renderer();
	virtual ~FEDX11Renderer();

	virtual void SetRSState(BYTE flag) override;
	virtual void SetDSState(DWORD flag, UINT stencilRef) override;
	//virtual void SetBlendState(BLEND_STATE bs) override;

	virtual void SetVertexBuffers(UINT StartSlot, UINT NumBuffers, const IFEBuffer* ppVertexBuffers, const UINT* pStrides, const UINT* pOffsets) const override;
	virtual void SetIndexBuffer(const IFEBuffer* pIndexBuffer, FEGI_FORMAT Format, UINT Offset) const override;
	virtual void SetPrimitiveTopology(FE_PRIMITIVE_TOPOLOGY Topology) const override;

	virtual void ClearBackBuffer(const FEVector4& i_color) const override;
	virtual void Draw(UINT VertexCount, UINT StartVertexLocation) const override;
	virtual void DrawIndexed(UINT IndexCount, UINT StartIndexLocation, UINT BaseVertexLocation) const override;
	virtual void Flip() const override;

	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDXDC() const;
};

#endif