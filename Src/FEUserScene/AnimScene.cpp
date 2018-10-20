#include "AnimScene.h"
#include "FETEST.h"
#include "Grid.h"
#include "CameraController.h"
#include <FEDefine.h>
#include <FEGameObject.h>

AnimScene::AnimScene()
{
}


AnimScene::~AnimScene()
{
}


void AnimScene::Load()
{
	IFEScene::s_BGColor = FEVector4(0, 0.125f, 0.3f, 1);

	FEGameObject* pGrid = new FEGameObject();
	auto grid = pGrid->AddComponent<Grid>();
	grid->x = true;
	grid->y = true;
	grid->z = true;
	//FEGameObject* pCam = new FEGameObject(FEVector3(0.0f, 0.0f, -30.0f), FEVector3::Zero, FEVector3::One);
	FEGameObject* pCam = new FEGameObject(FEVector3(0.0f, 10.0f, -25.0f), FEVector3::Right * FEConvertToRadian(25.0f), FEVector3::One);
	pCam->m_Name = FE_TEXT("Ä«¸Þ¶ó");
	pCam->AddComponent<CameraController>()->pCam = pCam->AddComponent<FECamera>();

	FEGameObject* pLight = new FEGameObject(FEVector3::Zero, FEVector3::Zero, FEVector3::One);
	pLight->m_Name = FE_TEXT("Direction Light");
	pLight->GetTransform()->SetRotationDegree(FEVector3(50.0f, -30.0f, 0.0f));
	auto light = pLight->AddComponent<FELight>();
	light->m_diffuse = FEVector4(1.0f, 1.0f, 1.0f, 1.0f);
	light->m_ambient = FEVector4(0.2f, 0.2f, 0.2f, 1.0f);
	light->m_lightType = FE_LIGHT_TYPE_DIRECTION;

	FEGameObject* pBox = FEGameObject::CopyObject(FEGameObject::FindPrefab(-2457711889405180742));
	auto pAnim = pBox->GetComponent<FEAnimation>();
}