#include "FESystem.h"
#include <IFERenderer.h>
#include <FEDefine.h>
#include <FESceneManager.h>
#include "FEUtility.h"

#ifdef _WIN32
//-------------------------------
// 윈도우 폴더 내 파일 찾기
//-------------------------------
#include <io.h>
#include <conio.h>
//-------------------------------
#else
#endif

using namespace std;

FESystem* FESystem::_pInstance = nullptr;

void LoadShader(tstring i_path)
{
	tstring extension;

#ifdef _WIN32
	_tfinddata_t fd;
	intptr_t handle;
	int result = 1;

	// 현재 폴더 내 모든 파일을 찾는다.
	handle = _tfindfirst((i_path + FE_TEXT("*")).c_str(), &fd);

	// 파일이 하나도 없다면
	if (handle == -1)
		return;

	do
	{
		FileNameExtension(fd.name, extension);

		if (extension.size() == 0)
		{
			if (!TCSCMP_SAME(fd.name, FE_TEXT(".")) && !TCSCMP_SAME(fd.name, FE_TEXT("..")))
				LoadShader(i_path + fd.name + FE_TEXT("/"));
		}

		else if (TCSCMP_SAME(extension.c_str(), FE_TEXT("fes")))
		{
			FEShader* pShader;
			TCHAR str[256];
			TCHAR str2[256];

			tifstream f((i_path + fd.name).c_str());

			if (f.fail())
			{
				//FEDebug::WarningMessage(FE_TEXT("Failed to load shader."));
				return;
			}

			f >> str >> str2;
			pShader = FEShader::CreateShader((i_path + str).c_str(), (i_path + str2).c_str());

			if (pShader == nullptr)
			{
				//FEDebug::WarningMessage(FE_TEXT("Failed to load shader."));
				return;
			}

			while (!f.eof())
			{
				f >> str;

				if (TCSCMP_SAME(str, FE_TEXT("Name")))
				{
					f >> str >> pShader->m_Name;
				}

				else if (TCSCMP_SAME(str, FE_TEXT("countTexture")))
				{
					f >> str >> pShader->_countTexture;
				}

				else if (TCSCMP_SAME(str, FE_TEXT("countMatrix")))
				{
					f >> str >> pShader->_countMatrix;
				}

				else if (TCSCMP_SAME(str, FE_TEXT("countVector")))
				{
					f >> str >> pShader->_countVector;
				}

				else if (TCSCMP_SAME(str, FE_TEXT("countScalar")))
				{
					f >> str >> pShader->_countScalar;
				}

				else if (TCSCMP_SAME(str, FE_TEXT("useLight")))
				{
					f >> str >> pShader->_useLight;
				}
			}

			f.close();
		}

		result = _tfindnext(handle, &fd);
	} while (result != -1);

	_findclose(handle);
#else
#error 윈도우가 아니라구
#endif
}
void LoadMaterial(tstring i_path)
{
	tstring extension;

#ifdef _WIN32
	_tfinddata_t fd;
	intptr_t handle;
	int result = 1;

	// 현재 폴더 내 모든 파일을 찾는다.
	handle = _tfindfirst((i_path + FE_TEXT("*")).c_str(), &fd);

	// 파일이 하나도 없다면
	if (handle == -1)
		return;

	do
	{
		FileNameExtension(fd.name, extension);

		if (extension.size() == 0)
		{
			if (!TCSCMP_SAME(fd.name, FE_TEXT(".")) && !TCSCMP_SAME(fd.name, FE_TEXT("..")))
				LoadMaterial(i_path + fd.name + FE_TEXT("/"));
		}

		else if (TCSCMP_SAME(extension.c_str(), FE_TEXT("fem")))
		{
			FEMaterial* pMaterial;
			TCHAR str[256];

			tifstream f((i_path + fd.name).c_str());

			if (f.fail())
			{
				//FEDebug::WarningMessage(FE_TEXT("Failed to load shader."));
				return;
			}

			while (!f.eof())
			{
				f >> str;

				if (TCSCMP_SAME(str, FE_TEXT("Shader")))
				{
					f >> str >> str;

					pMaterial = FEMaterial::CreateMaterial(FEShader::Find(str));

					if (pMaterial == nullptr)
					{
						//FEDebug::WarningMessage(FE_TEXT("Failed to load shader."));
						return;
					}
				}

				else if (TCSCMP_SAME(str, FE_TEXT("Name")))
				{
					f >> str >> pMaterial->m_Name;
				}
			}

			f.close();
		}

		result = _tfindnext(handle, &fd);
	} while (result != -1);

	_findclose(handle);
#else
#error 윈도우가 아니라구
#endif
}
/*void CheckFiles(tstring i_path)
{
	tstring extension;

#ifdef _WIN32
	_tfinddata_t fd;
	intptr_t handle;
	int result = 1;

	// 현재 폴더 내 모든 파일을 찾는다.
	handle = _tfindfirst((i_path + FE_TEXT("*")).c_str(), &fd);

	// 파일이 하나도 없다면
	if (handle == -1)
		return;

	do
	{
		FileNameExtension(fd.name, extension);

		if (extension.size() == 0)
		{
			LoadShader(i_path + fd.name + FE_TEXT("/"));
		}

		else if (TCSCMP_SAME(extension.c_str(), FE_TEXT("extension")))
		{

		}

		result = _tfindnext(handle, &fd);
	} while (result != -1);

	_findclose(handle);
#else
#error 윈도우가 아니라구
#endif
}*/
FESystemSetting LoadSetting()
{
	FESystemSetting setting;

	TCHAR str[256];
	tifstream f(FE_TEXT("../../Data/SystemSetting.ini"));

	if (f.fail())
	{
		setting.uiWindowWidth = 800;
		setting.uiWindowHeight = 600;
		setting.bWindowMode = false;
		setting.bBorderless = false;
		setting.bVSync = false;
	}

	while (!f.eof())
	{
		f >> str;

		if (_tcsnicmp(FE_TEXT("Width"), str, 5) == 0)
		{
			f >> str >> setting.uiWindowWidth;
		}

		else if (_tcsnicmp(FE_TEXT("Height"), str, 6) == 0)
		{
			f >> str >> setting.uiWindowHeight;
		}

		else if (_tcsnicmp(FE_TEXT("WindowMode"), str, 10) == 0)
		{
			f >> str >> setting.bWindowMode;
		}

		else if (_tcsnicmp(FE_TEXT("Borderless"), str, 10) == 0)
		{
			f >> str >> setting.bBorderless;
		}

		else if (_tcsnicmp(FE_TEXT("VSync"), str, 5) == 0)
		{
			f >> str >> setting.bVSync;
		}
	}

	f.close();

	return setting;
}


FESystem::FESystem()
	: _pWindow(nullptr), m_bExit(false)
{
}


FESystem::~FESystem()
{
	Release();
}

bool FESystem::Create(LPCTSTR i_sWindowName)
{
	//--------------
	// 설정 읽어오기
	//--------------
	FESystemSetting setting = LoadSetting();


	//------------
	// 윈도우 생성
	//------------
	_pWindow = FEWindow::CreateFEWindow(i_sWindowName, setting.uiWindowWidth, setting.uiWindowHeight, setting.bWindowMode, setting.bBorderless);

	if (_pWindow == nullptr)
		return false;


	//------------
	// 렌더러 생성
	//------------
	IFERenderer::CreateRenderer(_pWindow->GetWindowHandle(), setting, FE_DX11);

	return true;
}


void FESystem::Release()
{
	delete FESceneManager::GetInstance();
	delete IFERenderer::GetInstance();

	SAFE_DELETE(_pWindow);
}


bool FESystem::LoadData()
{
	FESceneManager* pSM = FESceneManager::GetInstance();

	LoadShader(FE_TEXT("../../Data/"));
	FEShader::CreateDefaultConstantBuffer();
	LoadMaterial(FE_TEXT("../../Data/"));
	
	// ImportScene 함수를 통해 파일 형태로 저장해 놓은 씬들을 읽어 씬들을 구성하도록 만들자
	// 효과 : FEScene 클래스 하나로 모든 씬들을 만들 수 있다. 어차피 씬들의 다른 점은 오브젝트들의 배치 및 설정이다. 이 다른점들을 파일을 읽어 씬에 적용할 수 있도록 만들자.
	pSM->ImportScene();

	pSM->LoadScene((UINT)0);

	return true;
}


void FESystem::Run()
{
	FESceneManager* pSM = FESceneManager::GetInstance();
	IFERenderer* pRenderer = IFERenderer::GetInstance();

	while (!m_bExit)
	{
		if (!_pWindow->MessagePump())		// 메세지 펌프.
			break;

		pSM->ChangeScene();

		FESceneManager::_pCurrentScene->Initialize();
		FESceneManager::_pCurrentScene->Update();

		pRenderer->ClearBackBuffer(FESceneManager::_pCurrentScene->s_BGColor);
		FESceneManager::_pCurrentScene->Render();
		pRenderer->Flip();
	}
}


FESystem* FESystem::GetInstance()
{
	if (_pInstance == nullptr) _pInstance = new FESystem;

	return _pInstance;
}