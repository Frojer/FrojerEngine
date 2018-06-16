#pragma once
#ifndef _FE_IRENDERER
#define _FE_IRENDERER

#include <FEDefine.h>
#include <FEMath.h>

struct FESystemSetting
{
	UINT uiWindowWidth, uiWindowHeight;

	// 전체화면 사용 여부
	bool bWindowMode;
	// 수직동기화 사용여부
	bool bVSync;
};

class IFERenderer
{
public:
	FESystemSetting m_Setting;

protected:
	virtual bool Create(void* i_phWnd) = 0;
	virtual void Release() = 0;

public:
	IFERenderer();
	virtual ~IFERenderer();

	static IFERenderer* CreateRenderer(void* i_phWnd, const FESystemSetting& i_Setting);

	virtual void ClearBackBuffer(const FEVector4& i_color) = 0;
	virtual void Flip() = 0;
};

#endif