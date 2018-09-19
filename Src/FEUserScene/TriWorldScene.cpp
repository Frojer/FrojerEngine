#include "TriWorldScene.h"
#include "TriWorldSystem.h"
#include "Grid.h"
#include "CameraController.h"
#include "Windmill.h"
#include "Hero.h"
#include <FEDefine.h>
#include <FEGameObject.h>
#include <windows.h>

TriWorldScene::TriWorldScene()
{
}


TriWorldScene::~TriWorldScene()
{
}


void TriWorldScene::Load()
{
#define AUTUMN_COLOR FEVector4(1.5f, 0.75f, 0.0f, 1.0f)

	IFEScene::s_BGColor = FEVector4(0.45098f, 0.76862f, 1.0f, 1.0f);

	// 시스템 생성
	FEGameObject* pSystem = new FEGameObject(FEVector3::One, FEVector3::Zero, FEVector3::One);
	auto pSysCom = pSystem->AddComponent<TriWorldSystem>();

	// 카메라 생성
	FEGameObject* pCam = new FEGameObject(FEVector3(0.0f, 5.0f, -30.0f), FEVector3::Zero, FEVector3::One);
	pCam->AddComponent<CameraController>()->pCam = pCam->AddComponent<FECamera>();

	// 그리드 생성하기
	FEGameObject* pGrid = new FEGameObject();
	auto grid = pGrid->AddComponent<Grid>();
	grid->x = false;
	grid->y = true;
	grid->z = true;

	// 터레인 만들기
	FEGameObject* pTerrain = FEGameObject::CopyObject(FEGameObject::FindPrefab(5697116093628749358));
	pTerrain->GetTransform()->m_vScale = FEVector3(0.5f, 0.5f, 0.5f);
	pSysCom->m_pTerrainMtrl = pTerrain->GetChildren().begin()->second->GetRenderer()->m_pMaterial;
	pSysCom->m_pTerrainMtrl->SetShader(FEShader::Find(FE_TEXT("WinterShader")));
	pSysCom->m_pTerrainMtrl->SetVector(0, AUTUMN_COLOR);
	pSysCom->m_pTerrainMtrl->SetTexture(1, FETexture::Find(1658363954890942618));
	//FEVector2 offset;
	//FEVector2 tiling;
	//pTerrain->GetChildren().begin()->second->GetRenderer()->m_pMaterial->GetTextureOffset(0, offset);
	//pTerrain->GetChildren().begin()->second->GetRenderer()->m_pMaterial->GetTextureTiling(0, tiling);
	//tiling *= 10;
	//pTerrain->GetChildren().begin()->second->GetRenderer()->m_pMaterial->SetVector(0, FEVector4(offset, tiling));

	// 나무 만들기
	FEGameObject* pTree = FEGameObject::CopyObject(FEGameObject::FindPrefab(7465656355178739812));
	pSysCom->m_pTrees[0] = pTree;
	pSysCom->m_pTreeMtrl = pTree->GetChildren().begin()->second->GetRenderer()->m_pMaterial;
	pSysCom->m_pTreeMtrl->SetShader(FEShader::Find(FE_TEXT("WinterShader")));
	pSysCom->m_pTreeMtrl->SetVector(0, AUTUMN_COLOR);
	pSysCom->m_pTreeMtrl->SetTexture(1, FETexture::Find(-8342061839902120095));
	/*FE_BLEND_DESC bd;
	bd.RenderTarget[0].BlendEnable = true;
	bd.RenderTarget[0].BlendOp = FE_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlend = FE_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = FE_BLEND_INV_SRC_ALPHA;	
	pTree->GetChildren().begin()->second->GetRenderer()->SetBlendState(bd);*/
	srand(GetTickCount());
	for (int i = 1; i < TREE_MAX; i++)
	{
		FEVector3 pos;
		pos.x = (rand() % 100) - 50;
		pos.z = (rand() % 100) - 50;
		pTree = FEGameObject::CopyObject(pTree);
		pTree->GetTransform()->SetPositionWorld(pos);
		pTree->GetTransform()->SetRotationDegree(FEVector3(0.0f, 180.0f, 0.0f));

		pSysCom->m_pTrees[i] = pTree;
	}

	// 일반 풍차 만들기
	FEGameObject* pWindmill = FEGameObject::CopyObject(FEGameObject::FindPrefab(8507257348452055230));
	pSysCom->m_pWindmillMtrl = pWindmill->GetChildren().begin()->second->GetRenderer()->m_pMaterial;
	pSysCom->m_pWindmillMtrl->SetShader(FEShader::Find(FE_TEXT("WinterShader")));
	pSysCom->m_pWindmillMtrl->SetVector(0, FEVector4::One);
	pSysCom->m_pWindmillMtrl->SetTexture(1, FETexture::Find(-2299588805160112115));
	auto wmComp = pWindmill->AddComponent<Windmill>();
	wmComp->m_pSystem = pSysCom;
	wmComp->m_type = WINDMILL_NORMAL;
	wmComp->m_pWindmillBody = pWindmill->GetChildren().begin()->second;
	wmComp->m_pWindmillWing[0] = (++pWindmill->GetChildren().begin())->second;
	wmComp->m_pWindmillWing[0]->GetTransform()->m_vScale *= 0.65f;
	wmComp->m_pWindmillWing[0]->GetTransform()->SetPositionLocal(FEVector3(0.0f, 1.5f, 1.0f));
	pWindmill->GetTransform()->SetPositionWorld(FEVector3(5.0f, -0.0002f, -10.0f));
	pWindmill->GetTransform()->Rotate(FEVector3(0.0f, 180.0f, 0.0f));

	// 삼단 풍차 만들기
	pWindmill = FEGameObject::CopyObject(FEGameObject::FindPrefab(8507257348452055230));
	wmComp = pWindmill->AddComponent<Windmill>();
	wmComp->m_pSystem = pSysCom;
	wmComp->m_type = WINDMILL_TRIPLE_TYPE1;
	wmComp->m_pWindmillBody = pWindmill->GetChildren().begin()->second;
	wmComp->m_pWindmillBody->GetTransform()->m_vScale.y = 2.0f;
	wmComp->m_pWindmillWing[0] = (++pWindmill->GetChildren().begin())->second;
	wmComp->m_pWindmillWing[1] = FEGameObject::CopyObject(wmComp->m_pWindmillWing[0]);
	wmComp->m_pWindmillWing[1]->SetParent(wmComp->m_pWindmillWing[0]);
	wmComp->m_pWindmillWing[2] = FEGameObject::CopyObject(wmComp->m_pWindmillWing[0]);
	wmComp->m_pWindmillWing[2]->SetParent(wmComp->m_pWindmillWing[1]);
	wmComp->m_pWindmillWing[0]->GetTransform()->m_vScale *= 0.65f;
	wmComp->m_pWindmillWing[0]->GetTransform()->m_vScale *= 1.5f;
	wmComp->m_pWindmillWing[1]->GetTransform()->m_vScale /= 1.5f;
	wmComp->m_pWindmillWing[2]->GetTransform()->m_vScale *= 0.8f;
	wmComp->m_pWindmillWing[0]->GetTransform()->SetPositionLocal(FEVector3(0.0f, 2.0f, 1.0f));
	wmComp->m_pWindmillWing[1]->GetTransform()->SetPositionLocal(FEVector3(0.0f, 0.0f, 0.5f));
	wmComp->m_pWindmillWing[2]->GetTransform()->SetPositionLocal(FEVector3(0.0f, 0.0f, 0.5f));
	pWindmill->GetTransform()->SetPositionWorld(FEVector3(-5.0f, -0.0001f, -10.0f));
	pWindmill->GetTransform()->Rotate(FEVector3(0.0f, 180.0f, 0.0f));

	// 삼단 풍차 타입 2 만들기
	pWindmill = FEGameObject::CopyObject(FEGameObject::FindPrefab(8507257348452055230));
	wmComp = pWindmill->AddComponent<Windmill>();
	wmComp->m_pSystem = pSysCom;
	wmComp->m_type = WINDMILL_TRIPLE_TYPE2;
	wmComp->m_pWindmillBody = pWindmill->GetChildren().begin()->second;
	wmComp->m_pWindmillBody->GetTransform()->m_vScale.y = 2.0f;
	wmComp->m_pWindmillWing[0] = (++pWindmill->GetChildren().begin())->second;
	wmComp->m_pWindmillWing[1] = FEGameObject::CopyObject(wmComp->m_pWindmillWing[0]);
	wmComp->m_pWindmillWing[1]->SetParent(pWindmill);
	wmComp->m_pWindmillWing[2] = FEGameObject::CopyObject(wmComp->m_pWindmillWing[0]);
	wmComp->m_pWindmillWing[2]->SetParent(pWindmill);
	wmComp->m_pWindmillWing[0]->GetTransform()->m_vScale *= 0.975;
	wmComp->m_pWindmillWing[1]->GetTransform()->m_vScale *= 0.65f;
	wmComp->m_pWindmillWing[2]->GetTransform()->m_vScale *= 0.8f;
	wmComp->m_pWindmillWing[1]->GetTransform()->Rotate(FEVector3(0.0f, 90.0f, 0.0f));
	wmComp->m_pWindmillWing[2]->GetTransform()->Rotate(FEVector3(0.0f, -135.0f, 0.0f));
	wmComp->m_pWindmillWing[0]->GetTransform()->SetPositionLocal(FEVector3(0.0f, 2.0f, 1.0f));
	wmComp->m_pWindmillWing[1]->GetTransform()->SetPositionLocal(FEVector3(1.0f, 4.0f, 0.0f));
	wmComp->m_pWindmillWing[2]->GetTransform()->SetPositionLocal(FEVector3(-0.5f, 6.0f, 0.5f));
	pWindmill->GetTransform()->SetPositionWorld(FEVector3(0.0f, 0.0f, -10.0f));
	pWindmill->GetTransform()->Rotate(FEVector3(0.0f, 180.0f, 0.0f));

	// 영웅 만들기
	FEGameObject* pHero = FEGameObject::CopyObject(FEGameObject::FindPrefab(6576196112351069742));
	pHero->GetTransform()->SetRotationDegree(FEVector3(-90.0f, -90.0f, 0.0f));
	pHero->AddComponent<Hero>()->m_pHero = pHero;
	
	// Directional Light 만들기
	FEGameObject* pDirectionalLight = new FEGameObject();
	pDirectionalLight->GetTransform()->SetRotationDegree(FEVector3(50.0f, -30.0f, 0.0f));
	auto light = pDirectionalLight->AddComponent<FELight>();
	light->m_lightType = FE_LIGHT_TYPE_DIRECTION;
	light->m_diffuse = FEVector4(1.0f, 1.0f, 1.0f, 1.0f);
	light->m_ambient = FEVector4(0.2f, 0.2f, 0.2f, 1.0f);

	// Point Light 만들기
	FEGameObject* pPointLight = new FEGameObject();
	pPointLight->SetParent(pHero);
	pPointLight->GetTransform()->SetPositionLocal(FEVector3(1.5f, 0.0f, 1.0f));
	pPointLight->GetTransform()->m_vScale = FEVector3(0.02f, 0.02f, 0.02f);
	auto pointLitCom = pPointLight->AddComponent<FELight>();
	pointLitCom->m_diffuse = FEVector4(1.0f, 0.40784f, 0.090196f, 1.0f);
	pointLitCom->m_ambient = FEVector4(0.5f, 0.20392f, 0.045098f, 1.0f);
	pointLitCom->m_range = 15.0f;
	pointLitCom->m_lightType = FE_LIGHT_TYPE_POINT;

	FEGameObject* pTorch = FEGameObject::CopyObject(FEGameObject::FindPrefab(6379430736193720248));
	pTorch->SetParent(pPointLight);
	pTorch->GetRenderer()->m_pMaterial->SetShader(FEShader::Find(2465136134763));

	// 박스 만들기
	FEGameObject* pBox = FEGameObject::CopyObject(FEGameObject::FindPrefab(2460141765634699607));
	pSysCom->m_pBoxMtrl = pBox->GetChildren().begin()->second->GetRenderer()->m_pMaterial;
	pSysCom->m_pBoxMtrl->SetShader(FEShader::Find(FE_TEXT("Box")));
	pSysCom->m_pBoxMtrl->SetVector(0, AUTUMN_COLOR);
	pSysCom->m_pBoxMtrl->SetTexture(1, FETexture::Find(1790542892283644886));
	pSysCom->m_pBoxMtrl->SetTexture(2, FETexture::Find(-9117736830242202577));
	pSysCom->m_pBoxMtrl->SetTexture(3, FETexture::Find(3226161949705243547));
	pBox->GetTransform()->SetPositionWorld(FEVector3(-3.0f, 1.0f, -25.0f));
	
	pSysCom->m_pDirectionalLight = light;
	pSysCom->m_pPointLight = pointLitCom;
	pSysCom->m_pTerrain = pTerrain;
	pSysCom->m_pHero = pHero;
	pSysCom->m_pBox = pBox;
}