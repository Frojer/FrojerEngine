#pragma once

#ifndef _FE_ISHADER
#define _FE_ISHADER

#include <FEDefine.h>
#include "IFEBuffer.h"

enum FE_SHADER_SEMANTICS
{
	FE_SHADER_SEMANTIC_POSITION			= 0x00000001,
	FE_SHADER_SEMANTIC_COLOR			= 0x00000004,
	FE_SHADER_SEMANTIC_NORMAL			= 0x00000010,
	FE_SHADER_SEMANTIC_TEXCOORD			= 0x00000040,
	FE_SHADER_SEMANTIC_BLENDINDEXCES	= 0x00000400,
	FE_SHADER_SEMANTIC_BLENDWEIGHT		= 0x00001000,
	FE_SHADER_SEMANTIC_POSITIONT		= 0x00004000,
	FE_SHADER_SEMANTIC_PSIZE			= 0x00008000,
	FE_SHADER_SEMANTIC_TANGENT			= 0x00020000,
	FE_SHADER_SEMANTIC_BINORMAL			= 0x00080000,
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

	virtual void SetConstantBuffer(UINT StartSlot, IFEBuffer* pConstantBuffers) = 0;
};

#endif