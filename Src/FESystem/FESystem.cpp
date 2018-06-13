#include "FESystem.h"
#include <FEDefine.h>
#include <FESceneManager.h>

using namespace std;

FESystem* FESystem::_pInstance = nullptr;

FESystem::FESystem()
	: _pWindow(nullptr), _pRenderer(nullptr), m_bExit(false)
{
}


FESystem::~FESystem()
{
	Release();
}

FESystemSetting FESystem::LoadSetting()
{
	FESystemSetting setting;

	TCHAR str[256];
	tifstream f(_TEXT("../../Data/SystemSetting.ini"));

	if (f.fail())
	{
		setting.uiWindowWidth = 800;
		setting.uiWindowHeight = 600;
		setting.bWindowMode = false;
		setting.bVSync = false;
	}

	while (!f.eof())
	{
		f >> str;

		if (_tcsnicmp(_TEXT("Width"), str, 5) == 0)
		{
			f >> str >> setting.uiWindowWidth;
		}

		else if (_tcsnicmp(_TEXT("Height"), str, 6) == 0)
		{
			f >> str >> setting.uiWindowHeight;
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
	_pWindow = FEWindow::CreateFEWindow(i_sWindowName, setting.uiWindowWidth, setting.uiWindowHeight);

	if (_pWindow == nullptr)
		return false;


	//------------
	// ������ ����
	//------------
	_pRenderer = IFERenderer::CreateRenderer(_pWindow->GetWindowHandle(), setting);

	return true;
}


void FESystem::Release()
{
	SAFE_DELETE(_pWindow);
}


bool FESystem::LoadData()
{
	FESceneManager* pSM = FESceneManager::GetInstance();
	
	// ImportScene �Լ��� ���� ���� ���·� ������ ���� ������ �о� ������ �����ϵ��� ������
	// ȿ�� : FEScene Ŭ���� �ϳ��� ��� ������ ���� �� �ִ�. ������ ������ �ٸ� ���� ������Ʈ���� ��ġ �� �����̴�. �� �ٸ������� ������ �о� ���� ������ �� �ֵ��� ������.
	pSM->ImportScene();

	pSM->LoadScene((UINT)0);

	pSM->ChangeScene();

	return true;
}


void FESystem::Run()
{
	while (!m_bExit)
	{
		if (!_pWindow->MessagePump())		// �޼��� ����.
			break;

		FESceneManager::_pCurrentScene->Initialize();
		FESceneManager::_pCurrentScene->Update();

		_pRenderer->ClearBackBuffer(FESceneManager::_pCurrentScene->s_bkColor);
		FESceneManager::_pCurrentScene->Render();
		_pRenderer->Flip();
	}
}


FESystem* FESystem::GetInstance()
{
	if (_pInstance == nullptr) _pInstance = new FESystem;

	return _pInstance;
}