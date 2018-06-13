#pragma once

#include "FEDXRenderer.h"

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

///////////////////////////////////////////////////////////////////////////////
//
//  Ÿ�� �� ��� ������ : ������ ȣȯ�� ��� ���밡 ����.
//
///////////////////////////////////////////////////////////////////////////////
// DX11 ��ġ���� ������.											
typedef ID3D11Device*			LPDEVICE;
typedef ID3D11DeviceContext*	LPDXDC;
typedef IDXGISwapChain*			LPSWAPCHAIN;
typedef ID3D11RenderTargetView*	LPRTVIEW;

///*
//// DirectX Math Ÿ�� ������ : ȣȯ�� ���
//// DX9/DX10 : DirectX SDK June.2010 ����
//typedef D3DXMATRIXA16	MATRIXA;	//��� : 16����Ʈ ���� ����.
//typedef D3DXMATRIX		MATRIX;		//��� : �Ϲ� ����.
//typedef D3DXVECTOR4		VECTOR4;
//typedef D3DXVECTOR3		VECTOR3;
//typedef D3DXVECTOR2		VECTOR2;
//*/
//// DirectX Math Ÿ�� ������ : ���� �ҽ����� ȣȯ�� ����� ����.
//// DX10/11 "XNAMath ver.2": DirectX SDK june.2010
//// DX11/12 "XNAMath ver.3": DirectXMath, Windows SDK 8.x �� ����  
//// ��ũ : https://msdn.microsoft.com/ko-kr/library/windows/desktop/ee418728(v=vs.85).aspx
////
//typedef DirectX::XMMATRIX		MATRIXA;	//��� : 16����Ʈ ����, SIMD ����. ����/���� ������. "Register Type"
//typedef DirectX::XMFLOAT4X4		MATRIX;		//��� : �Ϲ� ����. SIMD ������, Class ������ �����. "Storage Type"
//typedef DirectX::XMVECTOR		VECTOR;		//4���� ���� : 16����Ʈ ����, SIMD ����. ����/���� ������. "Register Type"
//typedef DirectX::XMFLOAT4		VECTOR4;	//4���� ���� : �Ϲ� ����, SIMD ������, Class ������ �����. "Storage Type"
//typedef DirectX::XMFLOAT3		VECTOR3;
//typedef DirectX::XMFLOAT2		VECTOR2;
//
////���� Ÿ��: 2����.
////typedef XMCOLOR		COLOR;		// r, g, b, a.  [������ 0~255]
//typedef DirectX::XMFLOAT4		COLOR;		// r, g, b, a.  [�Ǽ��� 0~1.0]