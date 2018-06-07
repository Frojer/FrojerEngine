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

	//������ ����.
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


	//Ŭ���̾�Ʈ ���� ũ�� ������ : �ʼ�. ��������
	//ResizeWindow(hWnd, width, height);

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