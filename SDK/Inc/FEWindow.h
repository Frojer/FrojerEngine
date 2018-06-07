#pragma once

#include "FESystemDefine.h"
#include <FEDefine.h>

class FEWindow
{
protected:
	tstring m_sWindowName;
public:
	UINT m_uiWidth, m_uiHeight;
	bool m_bExit;

protected:
	FEWindow();

	virtual bool InitWindow() = 0;
	virtual bool MessagePump() = 0;

	void Update();
	void Render();

public:
	virtual ~FEWindow();

	void Run();

	static FEWindow* CreateFEWindow(LPCTSTR i_sWindowName, const UINT i_width, const UINT i_height);
};