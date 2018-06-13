#pragma once

#include "FEDXRenderer.h"

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

///////////////////////////////////////////////////////////////////////////////
//
//  타입 과 상수 재정의 : 버전별 호환성 향상 증대가 목적.
//
///////////////////////////////////////////////////////////////////////////////
// DX11 장치관련 재정의.											
typedef ID3D11Device*			LPDEVICE;
typedef ID3D11DeviceContext*	LPDXDC;
typedef IDXGISwapChain*			LPSWAPCHAIN;
typedef ID3D11RenderTargetView*	LPRTVIEW;

///*
//// DirectX Math 타입 재정의 : 호환성 향상
//// DX9/DX10 : DirectX SDK June.2010 이하
//typedef D3DXMATRIXA16	MATRIXA;	//행렬 : 16바이트 정렬 버전.
//typedef D3DXMATRIX		MATRIX;		//행렬 : 일반 버전.
//typedef D3DXVECTOR4		VECTOR4;
//typedef D3DXVECTOR3		VECTOR3;
//typedef D3DXVECTOR2		VECTOR2;
//*/
//// DirectX Math 타입 재정의 : 구형 소스와의 호환성 향상이 목적.
//// DX10/11 "XNAMath ver.2": DirectX SDK june.2010
//// DX11/12 "XNAMath ver.3": DirectXMath, Windows SDK 8.x 에 포함  
//// 링크 : https://msdn.microsoft.com/ko-kr/library/windows/desktop/ee418728(v=vs.85).aspx
////
//typedef DirectX::XMMATRIX		MATRIXA;	//행렬 : 16바이트 정렬, SIMD 버전. 전역/지역 변수용. "Register Type"
//typedef DirectX::XMFLOAT4X4		MATRIX;		//행렬 : 일반 버전. SIMD 미지원, Class 데이터 저장용. "Storage Type"
//typedef DirectX::XMVECTOR		VECTOR;		//4성분 벡터 : 16바이트 정렬, SIMD 버전. 전역/지역 변수용. "Register Type"
//typedef DirectX::XMFLOAT4		VECTOR4;	//4성분 벡터 : 일반 버전, SIMD 미지원, Class 데이터 저장용. "Storage Type"
//typedef DirectX::XMFLOAT3		VECTOR3;
//typedef DirectX::XMFLOAT2		VECTOR2;
//
////색상 타입: 2가지.
////typedef XMCOLOR		COLOR;		// r, g, b, a.  [정수형 0~255]
//typedef DirectX::XMFLOAT4		COLOR;		// r, g, b, a.  [실수형 0~1.0]