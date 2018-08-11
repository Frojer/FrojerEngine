#include "FESystem.h"
#include <IFERenderer.h>
#include <FEDefine.h>
#include <FESceneManager.h>

using namespace std;

FESystem* FESystem::_pInstance = nullptr;

FESystem::FESystem()
	: _pWindow(nullptr), m_bExit(false)
{
}


FESystem::~FESystem()
{
	Release();
}

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

bool FESystem::Create(LPCTSTR i_sWindowName)
{
	//--------------
	// ���� �о����
	//--------------
	FESystemSetting setting = LoadSetting();


	//------------
	// ������ ����
	//------------
	_pWindow = FEWindow::CreateFEWindow(i_sWindowName, setting.uiWindowWidth, setting.uiWindowHeight, setting.bWindowMode, setting.bBorderless);

	if (_pWindow == nullptr)
		return false;


	//------------
	// ������ ����
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
	
	// ImportScene �Լ��� ���� ���� ���·� ������ ���� ������ �о� ������ �����ϵ��� ������
	// ȿ�� : FEScene Ŭ���� �ϳ��� ��� ������ ���� �� �ִ�. ������ ������ �ٸ� ���� ������Ʈ���� ��ġ �� �����̴�. �� �ٸ������� ������ �о� ���� ������ �� �ֵ��� ������.
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
		if (!_pWindow->MessagePump())		// �޼��� ����.
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