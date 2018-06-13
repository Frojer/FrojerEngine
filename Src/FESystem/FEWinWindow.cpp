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

	//������ ����.
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


	//Ŭ���̾�Ʈ ���� ũ�� ������ : �ʼ�. ��������
	ResizeWindow(m_uiWidth, m_uiHeight);

	//���콺 Ŀ�� : �⺻ȭ��ǥ�� ����.
	SetCursor(LoadCursor(NULL, IDC_ARROW));

	return true;
}


bool FEWinWindow::MessagePump()
{
	MSG msg;	::ZeroMemory(&msg, sizeof(msg));

	while (true)
	{
		//�޼��� ť�� �޼����� ������...ó���ϰ�..
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)			//������ ���Ḧ ���� ����.
				break;

			//������ �޼��� ó��.
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else  //Ư���� �޼����� ���ٸ� ������ ����(������)�� �����մϴ�..
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
	// ���콺 Ŀ���� ǥ���մϴ�.
	ShowCursor(true);

	/*// Ǯ��ũ�� ��带 �������� �� ���÷��� ������ �ٲߴϴ�.
	if (FULL_SCREEN)
	{
	ChangeDisplaySettings(NULL, 0);
	}*/

	// â�� �����մϴ�.
	DestroyWindow(_hWnd);
	_hWnd = NULL;

	// ���ø����̼� �ν��Ͻ��� �����մϴ�.
	UnregisterClass(_sClassName.c_str(), ::GetModuleHandle(NULL));
}


/////////////////////////////////////////////////////////////////////////
//
// void B3D_ResizeWindow(UINT NewWidth, UINT NewHeight)
//
// â����� ��� �ػ� ����� ���߾� ��ü ������ ũ�⸦ �������մϴ�. 
// ex)
//'Ŭ���� ��Ʈ'������ 640 * 480 �� ���߾�  ��ü �������� ũ�⸦ 
// �ٽ� �����մϴ�. --> �׷��� '���� �̹���'�� ��� ȭ�鿡 ���δ�.
//
// ��  �� :  Ŭ���̾�Ʈ ������ ũ�� - NewWidth, NewHeight
// ���ϰ� :  ����. 
//
// 2004.05.28. ������.
//
/////////////////////////////////////////////////////////////////////////
bool FEWinWindow::ResizeWindow(UINT i_width, UINT i_height)
{
	BOOL result;

	//���� �������� ��Ÿ�� ���ϱ�.
	RECT oldrc;
	DWORD sytle = (DWORD) ::GetWindowLong(_hWnd, GWL_STYLE);
	DWORD exstyle = (DWORD) ::GetWindowLong(_hWnd, GWL_EXSTYLE);

	//���� ������ '��ü' ũ�� (��ũ����ǥ) �� ��´�.
	result = ::GetWindowRect(_hWnd, &oldrc);

	if (!result)
	{
		return false;
	}

	//���� ������ �������� 'Ŭ���̾�Ʈ' ���� ũ�� ����ϱ�.
	RECT newrc;
	newrc.left = 0;
	newrc.top = 0;
	newrc.right = i_width;
	newrc.bottom = i_height;

	//newrc ��ŭ�� Ŭ���̾�Ʈ ������ �����ϴ� ������ '��ü' ũ�⸦ ���մϴ�.
	//���� '�޴�' �� ���ٴ� �����Ͽ� ó���ǰ� ����.
	//���� ����� newrc �� �ٽ� ���´�. ( ��ũ�� ��ǥ )
	//exstyle &= ~WS_EX_TOPMOST;
	result = ::AdjustWindowRectEx(&newrc, sytle, NULL, exstyle);

	if (!result)
	{
		return false;
	}


	//������ �������� �ʺ�� ���� ���մϴ�. 
	int newWidth = (newrc.right - newrc.left);
	int newHeight = (newrc.bottom - newrc.top);


	//���ο� ũ�⸦ �����쿡 �����մϴ�.
	result = ::SetWindowPos(_hWnd, HWND_NOTOPMOST,
		oldrc.left, oldrc.top,
		newWidth, newHeight, SWP_SHOWWINDOW);

	if (!result)
	{
		return false;
	}

	return true;
}