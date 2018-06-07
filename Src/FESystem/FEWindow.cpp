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
		if (!MessagePump())		// 메세지 펌프.
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
	#error 윈도우가 아니자낭
#endif

	if (window == nullptr)
		return nullptr;

	window->m_sWindowName = i_sWindowName;
	window->m_uiWidth = i_width;
	window->m_uiHeight = i_height;

	window->InitWindow();

	return window;
}