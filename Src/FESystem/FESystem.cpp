#include "FESystem.h"

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


void FESystem::Run()
{
	while (!m_bExit)
	{
		if (!_pWindow->MessagePump())		// �޼��� ����.
			break;
/*
		Update();
		Render();*/
	}
}


FESystem* FESystem::GetInstance()
{
	if (_pInstance == nullptr) _pInstance = new FESystem;

	return _pInstance;
}