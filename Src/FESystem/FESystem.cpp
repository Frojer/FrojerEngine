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

bool FESystem::Create(LPCTSTR i_sWindowName)
{
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
	IFERenderer::CreateRenderer(_pWindow->GetWindowHandle(), setting, FE_DX11);

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

	// �⺻ ���̴� ����
	FEShader* pShader = new FEShader(CreateUUIDHashCode64());
	pShader->m_Name = FE_TEXT("Standard");

	//str = START_PATH;
	//str.append(FE_TEXT("shd/"));
	//pShader->CreateShader((str + FE_TEXT("Standard.vso")).c_str(), (str + FE_TEXT("Standard.pso")).c_str(), static_cast<FE_SHADER_SEMANTICS>(3));

	pShader->_countMatrix = 0;
	pShader->_countVector = 0;
	pShader->_countScalar = 0;

	// �⺻ �ؽ��� ����
	
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
		if (!_pWindow->MessagePump())		// �޼��� ����.
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