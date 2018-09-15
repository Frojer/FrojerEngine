#include "FESceneManager.h"

//-------------------------------
// 윈도우 폴더 내 파일 찾기
//-------------------------------
#include <io.h>
#include <conio.h>
//-------------------------------

//----------------
// FEUserScene
//----------------
#include <FEUserScene.h>

FESceneManager* FESceneManager::_pInstance = nullptr;
IFEScene* FESceneManager::_pCurrentScene = nullptr;

FESceneManager::FESceneManager()
	: _pChangeScene(nullptr)
{
}


FESceneManager::~FESceneManager()
{
	Release();
}


bool FESceneManager::ImportScene()
{
	LoadUserScene();

	return true;
}


void FESceneManager::AddScene(IFEScene* i_pScene)
{
	_vScene.push_back(i_pScene);
}


void FESceneManager::ChangeScene()
{
	if (_pChangeScene == nullptr)
		return;

	SAFE_RELEASE(FESceneManager::_pCurrentScene);

	FESceneManager::_pCurrentScene = _pChangeScene;
	_pChangeScene = nullptr;

	FESceneManager::_pCurrentScene->Load();
}


FESceneManager* FESceneManager::GetInstance()
{
	if (_pInstance == nullptr)	_pInstance = new FESceneManager();

	return _pInstance;
}

IFEScene* FESceneManager::GetCurrentScene()
{
	return _pCurrentScene;
}


bool FESceneManager::LoadScene(LPCTSTR i_sceneName)
{
	if (_pChangeScene != nullptr)
		return false;

	for (UINT i = 0; i < _vScene.size(); i++)
	{
		if (_vScene[i]->m_sName == i_sceneName)
		{
			_pChangeScene = _vScene[i];
			break;
		}
	}

	if (_pChangeScene == nullptr)
		return false;

	return true;
}


bool FESceneManager::LoadScene(const UINT i_sceneNumber)
{
	if (_pChangeScene != nullptr)
		return false;

	if (_vScene.size() <= i_sceneNumber)
		return false;

	_pChangeScene = _vScene[i_sceneNumber];

	return true;
}


void FESceneManager::Release()
{
	for (UINT i = 0; i < _vScene.size(); i++)
	{
		_vScene[i]->Release();
		delete _vScene[i];
		_vScene[i] = nullptr;
	}

	_vScene.clear();
}