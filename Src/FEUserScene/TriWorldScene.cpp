#include "TriWorldScene.h"
#include "TriWorldSystem.h"
#include "Grid.h"
#include "CameraController.h"
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

	FEGameObject* pSystem = new FEGameObject(FEVector3::One, FEVector3::Zero, FEVector3::One);
	auto pSysCom = pSystem->AddComponent<TriWorldSystem>();

	FEGameObject* pCam = new FEGameObject(FEVector3(0.0f, 5.0f, -30.0f), FEVector3::Zero, FEVector3::One);
	pCam->AddComponent<CameraController>()->pCam = pCam->AddComponent<FECamera>();

	FEGameObject* pGrid = new FEGameObject();
	auto grid = pGrid->AddComponent<Grid>();
	grid->x = false;
	grid->y = true;
	grid->z = false;

	FEGameObject* pDirectionalLight = new FEGameObject();
	pDirectionalLight->GetTransform()->SetRotationDegree(FEVector3(50.0f, -30.0f, 0.0f));
	auto light = pDirectionalLight->AddComponent<FELight>();
	light->m_lightType = FE_LIGHT_TYPE_DIRECTION;
	light->m_diffuse = FEVector4(1.0f, 1.0f, 1.0f, 1.0f);
	light->m_ambient = FEVector4(0.2f, 0.2f, 0.2f, 1.0f);

	FEGameObject* pHero = FEGameObject::CopyObject(FEGameObject::FindPrefab(-7789239395380756604));
	pHero->GetTransform()->SetRotationDegree(FEVector3(-90.0f, -90.0f, 0.0f));
	FEGameObject* pBox = FEGameObject::CopyObject(FEGameObject::FindPrefab(2448678431520525937));
	pBox->GetTransform()->SetPositionWorld(FEVector3(10.0f, 0.0f, 0.0f));
	pBox->GetTransform()->SetRotationDegree(FEVector3(45.0f, 0.0f, 0.0f));
	pBox->GetTransform()->m_vScale = FEVector3(0.1f, 0.1f, 0.1f);
	//FEGameObject* pHero = FEGameObject::CopyObject(FEGameObject::FindPrefab(5993413170060267237));

	pSysCom->m_pHero = pHero;
	pSysCom->m_pBox = pBox;
}