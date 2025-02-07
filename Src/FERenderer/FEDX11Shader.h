#pragma once
#ifndef _FE_DX11_SHADER
#define _FE_DX11_SHADER

#include "IFEShader.h"
#ifdef FE_DX11
#include "FEDX11Buffer.h"
#include <d3d11.h>

class FEDX11Shader : public IFEShader
{
private:
	ID3D11VertexShader*     _pVS;
	ID3D11PixelShader*      _pPS;
	ID3D11InputLayout*      _pVBLayout;

	BYTE*	_pVSCode;
	SIZE_T _VSCodeSize;
	SIZE_T _PSCodeSize;

private:
	bool CreateShader(LPCTSTR i_vsName, LPCTSTR i_psName);
	bool CreateInputLayout(FE_SHADER_SEMANTICS i_semanticsFlag);
	bool CreateSamplerState(UINT index, FE_SAMPLER_STATE_FLAG& sampler);

public:
	FEDX11Shader();
	virtual ~FEDX11Shader();

	virtual bool Create(LPCTSTR i_vsName, LPCTSTR i_psName, FE_SHADER_SEMANTICS i_semanticsFlag) override;
	void Release();

	virtual void Render() const override;
	virtual void SetConstantBuffer(UINT StartSlot, IFEBuffer* pConstantBuffer) override;
	virtual void SetShaderResource(UINT StartSlot, IFETexture* pTexture) override;
	virtual void SetSamplerState(UINT StartSlot, FE_SAMPLER_STATE_FLAG& sampler) override;
};
#endif
#endif