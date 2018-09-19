#pragma once
#ifndef _FE_ISHADER
#define _FE_ISHADER

#include <FEBuildSetting.h>
#include "FERendererDefine.h"
#include <FEDefine.h>
#include "IFEBuffer.h"
#include "IFETexture.h"
#include <vector>

enum FE_SHADER_SEMANTICS : UINT
{
	FE_SHADER_SEMANTIC_POSITION			= 0x00000001,
	FE_SHADER_SEMANTIC_COLOR			= 0x00000002,
	FE_SHADER_SEMANTIC_NORMAL			= 0x00000004,
	FE_SHADER_SEMANTIC_TEXCOORD			= 0x00000008,
	FE_SHADER_SEMANTIC_NUM				= 4
};

// SamplerState
struct FE_SAMPLER_STATE_FLAG
{
	FE_FILTER filter;
	FE_ADDRESS_MODE addressU;
	FE_ADDRESS_MODE addressV;
	FE_ADDRESS_MODE addressW;
	float mipLODBias;
	UINT maxAnisotropy;
	FE_COMPARISON_FUNC comparisonFunc;
	FEVector4 borderColor;
	float minLOD;
	float maxLOD;

	FE_SAMPLER_STATE_FLAG()
		: filter(FE_FILTER_ANISOTROPIC),
		addressU(FE_ADDRESS_MODE_WRAP),
		addressV(FE_ADDRESS_MODE_WRAP),
		addressW(FE_ADDRESS_MODE_WRAP),
		mipLODBias(0.0f),
		maxAnisotropy(1),
		comparisonFunc(FE_COMPARISON_NEVER),
		borderColor(FEVector4::One),
		minLOD(0.0f),
		maxLOD(FE_FLOAT32_MAX) {}

	bool operator==(const FE_SAMPLER_STATE_FLAG& rhs) const
	{
		if (filter == rhs.filter &&
			addressU == rhs.addressU &&
			addressV == rhs.addressV &&
			addressW == rhs.addressW &&
			fabsf(mipLODBias - rhs.mipLODBias) <= FE_EPSILON &&
			maxAnisotropy == rhs.maxAnisotropy &&
			comparisonFunc == rhs.comparisonFunc &&
			borderColor == rhs.borderColor &&
			minLOD == rhs.minLOD &&
			maxLOD == rhs.maxLOD)
			return true;

		return false;
	}
};



class IFEShader
{
protected:
	virtual bool Create(LPCTSTR i_vsName, LPCTSTR i_psName, FE_SHADER_SEMANTICS i_semanticsFlag) = 0;

public:
	IFEShader() = default;
	virtual ~IFEShader() = default;

	static IFEShader* CreateShader(LPCTSTR i_vsName, LPCTSTR i_psName, FE_SHADER_SEMANTICS i_semanticsFlag);

	virtual void Render() const = 0;

	virtual void SetConstantBuffer(UINT StartSlot, IFEBuffer* pConstantBuffer) = 0;
	virtual void SetShaderResource(UINT StartSlot, IFETexture* pTexture) = 0;
	virtual void SetSamplerState(UINT StartSlot, FE_SAMPLER_STATE_FLAG& sampler) = 0;
};

#endif