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
	// ���� �о����
	//---------------


	//------------
	// ������ ����
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