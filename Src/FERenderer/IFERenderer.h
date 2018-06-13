#pragma once

#include <FEDefine.h>
#include <FEMath.h>

struct SystemSetting
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
	SystemSetting m_Setting;

protected:
	virtual bool Create(void* i_phWnd) = 0;
	virtual void Release() = 0;

public:
	IFERenderer();
	virtual ~IFERenderer();

	static IFERenderer* CreateRenderer(void* i_phWnd);

	virtual void ClearBackBuffer(const FEVector4& i_color) = 0;
	virtual void Flip() = 0;
};