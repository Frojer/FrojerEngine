#include <FEDefine.h>
#include <FEFileManager.h>

// chdir() 사용을 위한 헤더
#include <direct.h>

#pragma comment(lib, "FEFileManager")

#ifdef _UNICODE
int wmain(int argc, wchar_t* argv[])
#else
void main(int argc, char* argv[])
#endif
{
#ifdef _DEBUG
	_tchdir(FE_TEXT("../../SDK/FileConverter/"));
#endif

	if (argc != 3)
		return -1;

	FEFileManager::GetInstance()->_dataPath = argv[1];
	FEFileManager::GetInstance()->_outPath = argv[2];
	FEFileManager::GetInstance()->Update();

	return 0;
}