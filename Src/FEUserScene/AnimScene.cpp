#include "AnimScene.h"
#include "FETEST.h"
#include "Grid.h"
#include "CameraController.h"
#include "AnimSystem.h"
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
	IFEScene::s_BGColor = FEVector4(0.45098f, 0.77254f, 0.82745f, 1.0f);

	FEGameObject* pSys = new FEGameObject();
	auto pSysCom = pSys->AddComponent<AnimSystem>();

	FEGameObject* pGrid = new FEGameObject();
	auto grid = pGrid->AddComponent<Grid>();
	//FEGameObject* pCam = new FEGameObject(FEVector3(0.0f, 0.0f, -30.0f), FEVector3::Zero, FEVector3::One);
	FEGameObject* pCam = new FEGameObject(FEVector3(0.0f, 10.0f, -25.0f), FEVector3::Right * FEConvertToRadian(25.0f), FEVector3::One);
	pCam->m_Name = FE_TEXT("Ä«¸Þ¶ó");
	pCam->AddComponent<CameraController>()->pCam = pCam->AddComponent<FECamera>();

	FEGameObject* pLight = new FEGameObject(FEVector3::Zero, FEVector3::Zero, FEVector3::One);
	pLight->m_Name = FE_TEXT("Direction Light");
	pLight->GetTransform()->SetRotationDegree(FEVector3(75.0f, 15.0f, 0.0f));
	auto light = pLight->AddComponent<FELight>();
	pSysCom->pLight = pLight;

	light->m_diffuse = FEVector4(1.0f, 1.0f, 1.0f, 1.0f);
	light->m_ambient = FEVector4(0.5f, 0.5f, 0.5f, 1.0f);
	light->m_lightType = FE_LIGHT_TYPE_DIRECTION;

	FEGameObject* pTemple = FEGameObject::CopyObject(FEGameObject::FindPrefab(4798772092671233107));
	auto children = pTemple->GetChildren();
	for (auto iter = children.begin(); iter != children.end(); iter++)
	{
		auto renderer = iter->second->GetRenderer();
		if (renderer != nullptr)
		{
			renderer->m_pMaterial->SetShader(FEShader::Find(FE_TEXT("Fog")));
			renderer->m_pMaterial->SetVector(0, FEVector4(0.45098f, 0.77254f, 0.82745f, 1.0f));
			renderer->m_pMaterial->SetScalar(0, 40.0f);
			renderer->m_pMaterial->SetScalar(1, 120.0f);
		}
	}

	FEGameObject* pHero = FEGameObject::CopyObject(FEGameObject::FindPrefab(4121433002739557253));
	auto children2 = pHero->GetChildren();
	for (auto iter = children2.begin(); iter != children2.end(); iter++)
	{
		auto renderer = iter->second->GetRenderer();
		if (renderer != nullptr)
		{
			renderer->m_pMaterial->SetShader(FEShader::Find(FE_TEXT("Fog")));
			renderer->m_pMaterial->SetVector(0, FEVector4(0.45098f, 0.77254f, 0.82745f, 1.0f));
			renderer->m_pMaterial->SetScalar(0, 40.0f);
			renderer->m_pMaterial->SetScalar(1, 120.0f);
		}
	}
	pHero->GetTransform()->Rotate(FEVector3::Up * 180);
	pHero->GetTransform()->m_vScale = FEVector3::One * 10;
	pSysCom->Hero = pHero;
}