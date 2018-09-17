#include <Windows.h>
#include <FEDefine.h>
#include <FESystem.h>

// chdir() ����� ���� ���
#include <direct.h>

#pragma comment(lib, "FESystem")

/////////////////////////////////////////////////////////////////////////////
//
// WinMain : ���� �Լ�.
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