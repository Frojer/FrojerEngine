#pragma once

#ifndef _FE_ISHADER
#define _FE_ISHADER

#include <FEDefine.h>

class IFEShader
{
protected:
	virtual bool Create(LPCTSTR i_vsName, LPCTSTR i_psName) = 0;

public:
	IFEShader() = default;
	virtual ~IFEShader();

	static IFEShader* CreateShader(LPCTSTR i_vsName, LPCTSTR i_psName);

	virtual void Release() = 0;

	virtual void Render() const = 0;
};

#endif