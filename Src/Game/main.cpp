#include <Windows.h>
#include <FEWindow.h>

#ifdef _DEBUG
	#ifdef _WIN64
		#pragma comment(lib, "FESystem64d")
	#else
		#pragma comment(lib, "FESystemd")
	#endif
#else
	#ifdef _WIN64
		#pragma comment(lib, "FESystem64")
	#else
		#pragma comment(lib, "FESystem")
	#endif
#endif
/////////////////////////////////////////////////////////////////////////////
//
// WinMain : 메인 함수.
//
int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	FEWindow* window = FEWindow::CreateFEWindow(L"FE_Test", 800, 600);

	window->Run();

	return 0;
}