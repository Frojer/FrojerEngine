#include <Windows.h>
#include <FEDefine.h>
#include <FESystem.h>

// chdir() 사용을 위한 헤더
#include <direct.h>

#pragma region FESystem.lib
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
#pragma endregion

/////////////////////////////////////////////////////////////////////////////
//
// WinMain : 메인 함수.
//
int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
#ifdef _DEBUG
	_tchdir(FE_TEXT("../../bin/"));
#endif

	bool result = true;

	FESystem* pSystem = FESystem::GetInstance();

	result = pSystem->Create(_TEXT("FE_TEST"), &hInstance);

	if (!result)
	{
		SAFE_DELETE(pSystem);
		return 0;
	}

	result = pSystem->LoadData();

	pSystem->Run();

	SAFE_DELETE(pSystem);

	return 0;
}