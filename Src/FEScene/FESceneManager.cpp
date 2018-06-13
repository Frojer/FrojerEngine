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
	/*
#ifdef _WIN32
	_finddata_t fd;
	intptr_t handle;
	int result = 1;
	handle = _findfirst("..\\..\\Data\\*", &fd);  //현재 폴더 내 모든 파일을 찾는다.

	// 파일이 하나도 없다면
	if (handle == -1)
		return false;

	while (result != -1)
	{
		printf("File: %s\n", fd.name);
		result = _findnext(handle, &fd);
	}

	_findclose(handle);
#else
#error 윈도우가 아니라구
#endif
	*/

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