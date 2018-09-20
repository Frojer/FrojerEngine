#include "FESystem.h"
#include "FELoader.h"
#include "FETime.h"
#include <FEDefine.h>
#include <FEUtility.h>
#include <FESceneManager.h>

FESystem* FESystem::_pInstance = nullptr;


FESystem::FESystem()
	: _pWindow(nullptr), m_bExit(false)
{
}


FESystem::~FESystem()
{
	Release();
}

bool FESystem::Create(LPCTSTR i_sWindowName, void* i_pInstanceHandle)
{
	_pInstanceHandle = i_pInstanceHandle;

	//--------------
	// ���� �о����
	//--------------
	FESystemSetting setting = FELoader::LoadSetting();


	//------------
	// ������ ����
	//------------
	_pWindow = FEWindow::CreateFEWindow(i_sWindowName, setting.uiWindowWidth, setting.uiWindowHeight, setting.bWindowMode, setting.bBorderless);

	if (_pWindow == nullptr)
		return false;


	//------------
	// ������ ����
	//------------
	IFERenderer::CreateRenderer(_pWindow->GetWindowHandle(), setting);

	return true;
}


void FESystem::Release()
{
	delete FESceneManager::GetInstance();
	delete IFERenderer::GetInstance();
	delete FETime::_pInstance;

	SAFE_DELETE(_pWindow);
}


bool FESystem::LoadData()
{
	tstring str;
	FESceneManager* pSM = FESceneManager::GetInstance();

	FEShader::CreateDefaultConstantBuffer();

	//// �⺻ �ؽ��� ����
	//FELoader::LoadTexture(FE_TEXT("Resource/Standard/"), FE_TEXT("Default.png.fet"));
	//FELoader::LoadTexture(FE_TEXT("Resource/Standard/"), FE_TEXT("Error.png.fet"));
	//// �⺻ ���̴� ����
	//FELoader::LoadShader(FE_TEXT("Resource/Standard/"), FE_TEXT("Standard.fes"));
	
	// ��� ���� �ε�
	FELoader::FindTextureFile(START_PATH);
	FELoader::FindShaderFile(START_PATH);
	FELoader::FindMaterialFile(START_PATH);
	FELoader::FindMeshFile(START_PATH);
	
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
	FETime::_pInstance = new FETime();

	while (!m_bExit)
	{
		if (!_pWindow->MessagePump())
			break;
		
		FETime::_pInstance->TimeUpdate();

		pSM->ChangeScene();

		FESceneManager::_pCurrentScene->Initialize();
		FESceneManager::_pCurrentScene->Update();

		FESceneManager::_pCurrentScene->Render();
		pRenderer->Flip();
		pRenderer->ClearBackBuffer(FESceneManager::_pCurrentScene->s_BGColor);
	}
}


FESystem* FESystem::GetInstance()
{
	if (_pInstance == nullptr) _pInstance = new FESystem;

	return _pInstance;
}