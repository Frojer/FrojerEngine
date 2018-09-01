#include <FEDefine.h>
#include <FEFileManager.h>

#pragma region FEFileManager.lib
#ifdef _DEBUG
#ifdef _WIN64
#pragma comment(lib, "FEFileManager64d")
#else
#pragma comment(lib, "FEFileManagerd")
#endif
#else
#ifdef _WIN64
#pragma comment(lib, "FEFileManager64")
#else
#pragma comment(lib, "FEFileManager")
#endif
#endif
#pragma endregion

#ifdef _UNICODE
int wmain(int argc, wchar_t* argv[])
#else
void main(int argc, char* argv[])
#endif
{
	if (argc != 3)
		return -1;

	FEFileManager::GetInstance()->_dataPath = argv[1];
	FEFileManager::GetInstance()->_outPath = argv[2];
	FEFileManager::GetInstance()->Update();

	return 0;
}