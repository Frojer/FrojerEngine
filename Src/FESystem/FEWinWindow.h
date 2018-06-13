#pragma once
#include <Windows.h>
#include "FEWindow.h"

class FEWinWindow : public FEWindow
{
protected:
	HWND _hWnd;
	tstring _sClassName;

protected:
	virtual bool InitWindow() override;
	virtual bool MessagePump() override;

	static LRESULT CALLBACK MsgProc(HWND i_hWnd, UINT i_uiMsg, WPARAM i_wParam, LPARAM i_lParam);
public:
	FEWinWindow();
	virtual ~FEWinWindow() override;

	virtual void Release() override;

	virtual bool ResizeWindow(UINT i_width, UINT i_height) override;
};