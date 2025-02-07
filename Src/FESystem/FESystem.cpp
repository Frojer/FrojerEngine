#include "FESystem.h"
#include "FELoader.h"
#include "FETime.h"
#include <FEDefine.h>
#include <FEUtility.h>
#include <FESceneManager.h>
#include <FEDebug.h>

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
	// 설정 읽어오기
	//--------------
	FESystemSetting setting = FELoader::LoadSetting();


	//------------
	// 윈도우 생성
	//------------
	_pWindow = FEWindow::CreateFEWindow(i_sWindowName, setting.uiWindowWidth, setting.uiWindowHeight, setting.bWindowMode, setting.bBorderless);

	if (_pWindow == nullptr)
		return false;


	//------------
	// 렌더러 생성
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

	//// 기본 텍스쳐 생성
	//FELoader::LoadTexture(FE_TEXT("Resource/Standard/"), FE_TEXT("Default.png.fet"));
	//FELoader::LoadTexture(FE_TEXT("Resource/Standard/"), FE_TEXT("Error.png.fet"));
	//// 기본 셰이더 생성
	//FELoader::LoadShader(FE_TEXT("Resource/Standard/"), FE_TEXT("Standard.fes"));
	
	// 모든 파일 로드
	FELoader::FindTextureFile(START_PATH);
	FELoader::FindShaderFile(START_PATH);
	FELoader::FindMaterialFile(START_PATH);
	FELoader::FindMeshFile(START_PATH);

	FEMaterial::_pDefaultTex = FETexture::Find(-5783958937846238709);
	
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
	FETime::_pInstance = new FETime();

	while (!m_bExit)
	{
		if (!_pWindow->MessagePump())
			break;
		
		FETime::_pInstance->TimeUpdate();

		pSM->ChangeScene();

		FESceneManager::_pCurrentScene->Initialize();
		FESceneManager::_pCurrentScene->Update();

		pRenderer->ClearBackBuffer(FESceneManager::_pCurrentScene->s_BGColor);
		FEDebug::GetInstance()->RenderLineDrawQueue();
		FESceneManager::_pCurrentScene->Render();
		pRenderer->Flip();
	}
}


FESystem* FESystem::GetInstance()
{
	if (_pInstance == nullptr) _pInstance = new FESystem;

	return _pInstance;
}