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
	#error �����찡 �ƴ��ڳ�
#endif

	return true;
}