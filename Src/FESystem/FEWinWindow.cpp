#include "FEWinWindow.h"

FEWinWindow::FEWinWindow()
{
}


FEWinWindow::~FEWinWindow()
{
}


bool FEWinWindow::InitWindow()
{
	WNDCLASSEX wc = {
		sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0, 0,
		::GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		m_sWindowName.c_str(), NULL
	};

	RegisterClassEx(&wc);

	//윈도우 생성.
	m_hWnd = ::CreateWindow(m_sWindowName.c_str(), m_sWindowName.c_str(),
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		//WS_OVERLAPPEDWINDOW, 
		0, 0,
		m_uiWidth, m_uiHeight,
		GetDesktopWindow(), NULL,
		wc.hInstance, NULL
	);

	if (m_hWnd == NULL) return false;


	::ShowWindow(m_hWnd, SW_SHOWDEFAULT);
	::UpdateWindow(m_hWnd);


	//클라이언트 영역 크기 재조정 : 필수. 삭제금지
	//ResizeWindow(hWnd, width, height);

	//마우스 커서 : 기본화살표로 설정.
	SetCursor(LoadCursor(NULL, IDC_ARROW));

	return true;
}


bool FEWinWindow::MessagePump()
{
	MSG msg;	::ZeroMemory(&msg, sizeof(msg));

	while (true)
	{
		//메세지 큐에 메세지가 있으면...처리하고..
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)			//윈도우 종료를 위해 리턴.
				break;

			//나머지 메세지 처리.
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else  //특별한 메세지가 없다면 리턴후 게임(렌더링)을 진행합니다..
		{
			return true;
		}
	}

	return false;
}


LRESULT CALLBACK FEWinWindow::MsgProc(HWND i_hWnd, UINT i_uiMsg, WPARAM i_wParam, LPARAM i_lParam)
{
	switch (i_uiMsg)
	{
	case WM_KEYDOWN:
		switch (i_wParam)
		{
		case VK_ESCAPE:
			SendMessage(i_hWnd, WM_DESTROY, 0, 0);
			break;
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(i_hWnd, i_uiMsg, i_wParam, i_lParam);
}