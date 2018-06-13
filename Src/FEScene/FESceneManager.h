#pragma once

#include <vector>
#include <FEDefine.h>
#include "IFEScene.h"

class FESceneManager
{
private:
	static FESceneManager* _pInstance;

	std::vector<IFEScene*> _vScene;
	IFEScene* _pChangeScene;
	static IFEScene* _pCurrentScene;

private:
	bool ImportScene();

	void AddScene(IFEScene* i_pScene);
	void ChangeScene();

public:
	FESceneManager();
	~FESceneManager();

	static FESceneManager* GetInstance();

	bool LoadScene(LPCTSTR i_sceneName);
	bool LoadScene(const UINT i_sceneNumber);

	void Release();

	//bool FESystem::LoadData()
	friend class FESystem;
	friend void LoadUserScene();
};