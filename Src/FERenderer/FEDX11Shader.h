#pragma once
#ifndef _FE_DX11_SHADER
#define _FE_DX11_SHADER

#include "IFEShader.h"
#include <d3d11.h>

class FEDX11Shader : public IFEShader
{
private:
	ID3D11VertexShader*     _pVS;
	ID3D11PixelShader*      _pPS;
	ID3D11InputLayout*      _pVBLayout;

	BYTE*	_pVSCode;
	SIZE_T	 _VSCodeSize;
	SIZE_T	 _PSCodeSize;

private:
	bool CreateShader(LPCTSTR i_vsName, LPCTSTR i_psName);
	bool CreateInputLayout();

public:
	FEDX11Shader();
	virtual ~FEDX11Shader();

	virtual bool Create(LPCTSTR i_vsName, LPCTSTR i_psName) override;
	virtual void Release() override;

	virtual void Render() const override;
};

#endif