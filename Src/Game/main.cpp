#include <Windows.h>
#include <FEDefine.h>
#include <FESystem.h>

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
// WinMain : ���� �Լ�.
//
int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	bool result = true;

	FESystem* pSystem = FESystem::GetInstance();

	result = pSystem->Create(TEXT("FE_TEST"), 1600, 900);

	if (!result)
	{
		SAFE_DELETE(pSystem);
		return 0;
	}

	pSystem->Run();

	SAFE_DELETE(pSystem);

	return 0;
}