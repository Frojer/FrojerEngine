#include "FESystem.h"
#include <FESceneManager.h>

FESystem* FESystem::_pInstance = nullptr;

FESystem::FESystem()
{
}


FESystem::~FESystem()
{
	Release();
}


bool FESystem::Create(LPCTSTR i_sWindowName, const UINT i_width, const UINT i_height)
{
	//------------
	// ������ ����
	//------------
	_pWindow = FEWindow::CreateFEWindow(i_sWindowName, i_width, i_height);

	if (_pWindow == nullptr)
		return false;

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

	return true;
}


void FESystem::Run()
{
	while (!m_bExit)
	{
		if (!_pWindow->MessagePump())		// �޼��� ����.
			break;

		FESceneManager::_pCurrentScene->Update();
		FESceneManager::_pCurrentScene->Render();
	}
}


FESystem* FESystem::GetInstance()
{
	if (_pInstance == nullptr) _pInstance = new FESystem;

	return _pInstance;
}