#pragma once

#ifndef _FE_ISHADER
#define _FE_ISHADER

#include <FEDefine.h>
#include "IFEBuffer.h"

class IFEShader
{
protected:
	virtual bool Create(LPCTSTR i_vsName, LPCTSTR i_psName) = 0;

public:
	IFEShader() = default;
	virtual ~IFEShader() = default;

	static IFEShader* CreateShader(LPCTSTR i_vsName, LPCTSTR i_psName);

	virtual void Render() const = 0;

	virtual void SetConstantBuffer(UINT StartSlot, IFEBuffer* pConstantBuffers) = 0;
};

#endif