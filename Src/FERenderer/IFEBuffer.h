#pragma once
#ifndef _FE_IBUFFER
#define _FE_IBUFFER

#include <FEDefine.h>
#include "FERendererDefine.h"

class IFEBuffer
{
protected:
	virtual bool Create(FE_BIND_FLAG bindFlag, FE_USAGE usage, bool cpuAccess, UINT bufferSize, const void* bufferData = nullptr) = 0;

public:
	IFEBuffer() = default;
	virtual ~IFEBuffer() = default;

	static IFEBuffer* CreateBuffer(FE_BIND_FLAG bindFlag, FE_USAGE usage, bool cpuAccess, UINT bufferSize, const void* bufferData = nullptr);

	virtual void UpdateBuffer(const void* pData, UINT size) = 0;
};

#endif