#include "FEUserScene.h"
#include "UserScenes.h"
#include <FESceneManager.h>

void LoadUserScene()
{
	FESceneManager* pSM = FESceneManager::GetInstance();

	pSM->AddScene(new TriWorldScene);
	pSM->AddScene(new AnimScene);
}