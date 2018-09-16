#pragma once
#ifndef _FE_ISHADER
#define _FE_ISHADER

#include <FEBuildSetting.h>
#include <FEDefine.h>
#include "IFEBuffer.h"
#include "IFETexture.h"

enum FE_SHADER_SEMANTICS : UINT
{
	FE_SHADER_SEMANTIC_POSITION			= 0x00000001,
	FE_SHADER_SEMANTIC_COLOR			= 0x00000002,
	FE_SHADER_SEMANTIC_NORMAL			= 0x00000004,
	FE_SHADER_SEMANTIC_TEXCOORD			= 0x00000008,
	FE_SHADER_SEMANTIC_NUM				= 4
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
};

#endif