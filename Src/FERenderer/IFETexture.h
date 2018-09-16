#pragma once
#ifndef _FE_ITEXTURE
#define _FE_ITEXTURE

#include <FEBuildSetting.h>
#include <FEDefine.h>

class IFETexture
{
protected:
	virtual bool Create(LPCTSTR filename) = 0;

public:
	IFETexture() = default;
	virtual ~IFETexture() = default;

	static IFETexture* CreateTexture(LPCTSTR filename);

	virtual void Render(UINT startSlot) const = 0;
};

#endif