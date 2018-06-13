#include "FESystem.h"
#include <FESceneManager.h>


FESystem* FESystem::_pInstance = nullptr;

FESystem::FESystem()
	: _pWindow(nullptr), _pRenderer(nullptr), m_bExit(false)
{
}


FESystem::~FESystem()
{
	Release();
}


bool FESystem::Create(LPCTSTR i_sWindowName, const UINT i_width, const UINT i_height)
{
	//---------------
	// 설정 읽어오기
	//---------------


	//------------
	// 윈도우 생성
	//------------
	_pWindow = FEWindow::CreateFEWindow(i_sWindowName, i_width, i_height);

	if (_pWindow == nullptr)
		return false;

	_pRenderer = IFERenderer::CreateRenderer(_pWindow->GetWindowHandle());

	return true;
}


void FESystem::Release()
{
	SAFE_DELETE(_pWindow);
}


bool FESystem::LoadData()
{
	FESceneManager* pSM = FESceneManager::GetInstance();
	
	// ImportScene 함수를 통해 파일 형태로 저장해 놓은 씬들을 읽어 씬들을 구성하도록 만들자
	// 효과 : FEScene 클래스 하나로 모든 씬들을 만들 수 있다. 어차피 씬들의 다른 점은 오브젝트들의 배치 및 설정이다. 이 다른점들을 파일을 읽어 씬에 적용할 수 있도록 만들자.
	pSM->ImportScene();

	pSM->LoadScene((UINT)0);

	pSM->ChangeScene();

	return true;
}


void FESystem::Run()
{
	while (!m_bExit)
	{
		if (!_pWindow->MessagePump())		// 메세지 펌프.
			break;

		FESceneManager::_pCurrentScene->Initialize();
		FESceneManager::_pCurrentScene->Update();

		//_pRenderer->ClearBackBuffer(FESceneManager::_pCurrentScene->s_bkColor);
		FESceneManager::_pCurrentScene->Render();
		_pRenderer->Flip();
	}
}


FESystem* FESystem::GetInstance()
{
	if (_pInstance == nullptr) _pInstance = new FESystem;

	return _pInstance;
}