#pragma once
#ifndef _FE_IBUFFER
#define _FE_IBUFFER

#include <FEBuildSetting.h>
#include <FEDefine.h>
#include "FERendererDefine.h"

class IFEBuffer
{
public:
	UINT stride;
	UINT offset;

protected:
	virtual bool Create(FE_BIND_FLAG bindFlag, FE_USAGE usage, bool cpuAccess, UINT bufferSize, const void* bufferData = nullptr) = 0;

public:
	IFEBuffer() = default;
	virtual ~IFEBuffer() = default;

	static IFEBuffer* CreateBuffer(FE_BIND_FLAG bindFlag, FE_USAGE usage, bool cpuAccess, UINT bufferSize, const void* bufferData = nullptr, UINT stride = 0, UINT offset = 0);

	virtual void UpdateBuffer(const void* pData, UINT size) = 0;
};

#endif