#include "TriWorldScene.h"
#include "FETEST.h"
#include <FEDefine.h>
#include <FEGameObject.h>

TriWorldScene::TriWorldScene()
{
}


TriWorldScene::~TriWorldScene()
{
}


void TriWorldScene::Load()
{
	IFEScene::s_BGColor = FEVector4(0, 0.125f, 0.3f, 1);

	FEGameObject* pGrid = new FEGameObject();
	pGrid->AddComponent<FETEST>();
}