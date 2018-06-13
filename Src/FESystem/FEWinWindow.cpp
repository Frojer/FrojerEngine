#include "FEWinWindow.h"

FEWinWindow::FEWinWindow()
{
}


FEWinWindow::~FEWinWindow()
{
	Release();
}


bool FEWinWindow::InitWindow()
{
	_sClassName = _sWindowName;

	WNDCLASSEX wc = {
		sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0, 0,
		::GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		_sWindowName.c_str(), NULL
	};

	RegisterClassEx(&wc);

	//윈도우 생성.
	_hWnd = ::CreateWindow(_sClassName.c_str(), _sWindowName.c_str(),
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		//WS_OVERLAPPEDWINDOW, 
		0, 0,
		m_uiWidth, m_uiHeight,
		GetDesktopWindow(), NULL,
		wc.hInstance, NULL
	);

	if (_hWnd == NULL) return false;

	_phWnd = &_hWnd;


	::ShowWindow(_hWnd, SW_SHOWDEFAULT);
	::UpdateWindow(_hWnd);


	//클라이언트 영역 크기 재조정 : 필수. 삭제금지
	ResizeWindow(m_uiWidth, m_uiHeight);

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


void FEWinWindow::Release()
{
	// 마우스 커서를 표시합니다.
	ShowCursor(true);

	/*// 풀스크린 모드를 빠져나올 때 디스플레이 설정을 바꿉니다.
	if (FULL_SCREEN)
	{
	ChangeDisplaySettings(NULL, 0);
	}*/

	// 창을 제거합니다.
	DestroyWindow(_hWnd);
	_hWnd = NULL;

	// 어플리케이션 인스턴스를 제거합니다.
	UnregisterClass(_sClassName.c_str(), ::GetModuleHandle(NULL));
}


/////////////////////////////////////////////////////////////////////////
//
// void B3D_ResizeWindow(UINT NewWidth, UINT NewHeight)
//
// 창모드의 경우 해상도 사이즈에 맞추어 전체 윈도우 크기를 재조정합니다. 
// ex)
//'클라이 언트'영역을 640 * 480 에 맞추어  전체 윈도우의 크기를 
// 다시 보정합니다. --> 그래야 '게임 이미지'가 모두 화면에 보인다.
//
// 인  자 :  클라이언트 영역의 크기 - NewWidth, NewHeight
// 리턴값 :  없음. 
//
// 2004.05.28. 버전업.
//
/////////////////////////////////////////////////////////////////////////
bool FEWinWindow::ResizeWindow(UINT i_width, UINT i_height)
{
	BOOL result;

	//현재 윈도우의 스타일 구하기.
	RECT oldrc;
	DWORD sytle = (DWORD) ::GetWindowLong(_hWnd, GWL_STYLE);
	DWORD exstyle = (DWORD) ::GetWindowLong(_hWnd, GWL_EXSTYLE);

	//현재 윈도우 '전체' 크기 (스크린좌표) 를 얻는다.
	result = ::GetWindowRect(_hWnd, &oldrc);

	if (!result)
	{
		return false;
	}

	//새로 생성될 윈도우의 '클라이언트' 영역 크기 계산하기.
	RECT newrc;
	newrc.left = 0;
	newrc.top = 0;
	newrc.right = i_width;
	newrc.bottom = i_height;

	//newrc 만큼의 클라이언트 영역을 포함하는 윈도우 '전체' 크기를 구합니다.
	//현재 '메뉴' 는 없다는 가정하에 처리되고 있음.
	//계산된 결과는 newrc 에 다시 들어온다. ( 스크린 좌표 )
	//exstyle &= ~WS_EX_TOPMOST;
	result = ::AdjustWindowRectEx(&newrc, sytle, NULL, exstyle);

	if (!result)
	{
		return false;
	}


	//보정된 윈도우의 너비와 폭을 구합니다. 
	int newWidth = (newrc.right - newrc.left);
	int newHeight = (newrc.bottom - newrc.top);


	//새로운 크기를 윈도우에 설정합니다.
	result = ::SetWindowPos(_hWnd, HWND_NOTOPMOST,
		oldrc.left, oldrc.top,
		newWidth, newHeight, SWP_SHOWWINDOW);

	if (!result)
	{
		return false;
	}

	return true;
}