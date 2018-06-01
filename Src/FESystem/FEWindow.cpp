#include "FEWindow.h"
#include "FEWinWindow.h"

FEWindow::FEWindow()
{
}


FEWindow::~FEWindow()
{
}


bool CreateFEWindow()
{
#ifdef _WIN32

#else
	#error 윈도우가 아니자낭
#endif

	return true;
}