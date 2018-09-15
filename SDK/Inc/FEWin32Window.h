#pragma once
#ifndef _FE_WIN_WINDOW
#define _FE_WIN_WINDOW

#include <Windows.h>
#include "FEWindow.h"

class FEWin32Window : public FEWindow
{
protected:
	HWND _hWnd;
	tstring _sClassName;

protected:
	virtual bool InitWindow() override;
	virtual bool MessagePump() override;

	static LRESULT CALLBACK MsgProc(HWND i_hWnd, UINT i_uiMsg, WPARAM i_wParam, LPARAM i_lParam);
public:
	FEWin32Window();
	virtual ~FEWin32Window() override;

	virtual void Release() override;

	virtual bool ResizeWindow(UINT i_width, UINT i_height) override;
};

#endif