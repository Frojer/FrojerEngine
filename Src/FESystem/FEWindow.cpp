#include "FEWindow.h"
#include "FEWinWindow.h"

FEWindow::FEWindow()
{
}


FEWindow::~FEWindow()
{
}


void FEWindow::Update()
{

}


void FEWindow::Render()
{

}


void FEWindow::Run()
{
	while (!m_bExit)
	{
		if (!MessagePump())		// �޼��� ����.
			break;

		Update();
		Render();
	}
}


FEWindow* FEWindow::CreateFEWindow(LPCTSTR i_sWindowName, const UINT i_width, const UINT i_height)
{
	FEWindow* window = nullptr;

#ifdef _WIN32
	window = new FEWinWindow();
#else
	#error �����찡 �ƴ��ڳ�
#endif

	if (window == nullptr)
		return nullptr;

	window->m_sWindowName = i_sWindowName;
	window->m_uiWidth = i_width;
	window->m_uiHeight = i_height;

	window->InitWindow();

	return window;
}