#pragma once

#include "FESystemDefine.h"
#include <FEDefine.h>

class FEWindow
{
protected:
	tstring _sWindowName;
	void* _phWnd;
public:
	UINT m_uiWidth, m_uiHeight;

protected:
	FEWindow();

	virtual bool InitWindow() = 0;
	virtual bool MessagePump() = 0;

	static FEWindow* CreateFEWindow(LPCTSTR i_sWindowName, const UINT i_width, const UINT i_height);

public:
	virtual ~FEWindow();

	virtual void Release() = 0;

	void* GetWindowHandle();

	friend class FESystem;
};