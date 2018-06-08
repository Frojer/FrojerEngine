#include "FEWindow.h"
#include "FEWinWindow.h"

FEWindow::FEWindow()
	: m_uiWidth(0), m_uiHeight(0)
{
}


FEWindow::~FEWindow()
{
}


FEWindow* FEWindow::CreateFEWindow(LPCTSTR i_sWindowName, const UINT i_width, const UINT i_height)
{
	bool result = false;
	FEWindow* pWindow = nullptr;

#ifdef _WIN32
	pWindow = new FEWinWindow();
#else
	#error 윈도우가 아니자낭
#endif

	if (pWindow == nullptr)
		return nullptr;

	pWindow->m_sWindowName = i_sWindowName;
	pWindow->m_uiWidth = i_width;
	pWindow->m_uiHeight = i_height;

	result = pWindow->InitWindow();

	if (!result)	SAFE_DELETE(pWindow);

	return pWindow;
}