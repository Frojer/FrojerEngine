#pragma once

#include <FEDefine.h>
#include <FEMath.h>

struct SystemSetting
{
	UINT uiWindowWidth, uiWindowHeight;

	// ��üȭ�� ��� ����
	bool bWindowMode;
	// ��������ȭ ��뿩��
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