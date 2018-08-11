#pragma once
#ifndef _FE_WINDOW
#define _FE_WINDOW

#include "FESystemDefine.h"
#include <FEDefine.h>

class FEWindow
{
protected:
	tstring _sWindowName;
	void* _phWnd;

public:
	UINT m_uiWidth, m_uiHeight;
	bool m_bWindowMode;
	bool m_bBorderless;

protected:
	FEWindow();

	virtual bool InitWindow() = 0;
	virtual bool MessagePump() = 0;

	static FEWindow* CreateFEWindow(LPCTSTR i_sWindowName, const UINT i_width, const UINT i_height, const bool i_bWindowMode, const bool i_bBorderless);

public:
	virtual ~FEWindow();

	virtual void Release() = 0;

	virtual bool ResizeWindow(UINT i_width, UINT i_height) = 0;
	void* GetWindowHandle();

	friend class FESystem;
};

#endif