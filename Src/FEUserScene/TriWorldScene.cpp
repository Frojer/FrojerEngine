#include "TriWorldScene.h"
#include "TriWorldSystem.h"
#include "Grid.h"
#include "CameraController.h"
#include "Windmill.h"
#include "Hero.h"
#include <FEDefine.h>
#include <FEGameObject.h>
#include <windows.h>

#define TREE_MAX 300

TriWorldScene::TriWorldScene()
{
}


TriWorldScene::~TriWorldScene()
{
}


void TriWorldScene::Load()
{
	IFEScene::s_BGColor = FEVector4(0, 0.125f, 0.3f, 1);

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

	// Directional Light 만들기
	FEGameObject* pDirectionalLight = new FEGameObject();
	pDirectionalLight->GetTransform()->SetRotationDegree(FEVector3(50.0f, -30.0f, 0.0f));
	auto light = pDirectionalLight->AddComponent<FELight>();
	light->m_lightType = FE_LIGHT_TYPE_DIRECTION;
	light->m_diffuse = FEVector4(1.0f, 1.0f, 1.0f, 1.0f);
	light->m_ambient = FEVector4(0.2f, 0.2f, 0.2f, 1.0f);

	// 터레인 만들기
	FEGameObject* pTerrain = FEGameObject::CopyObject(FEGameObject::FindPrefab(5697116093628749358));
	pTerrain->GetTransform()->m_vScale = FEVector3(0.5f, 0.5f, 0.5f);

	// 나무 만들기
	FEGameObject* pTree = FEGameObject::CopyObject(FEGameObject::FindPrefab(7465656355178739812));
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
	}

	// 일반 풍차 만들기
	FEGameObject* pWindmill = FEGameObject::CopyObject(FEGameObject::FindPrefab(8507257348452055230));
	auto wmComp = pWindmill->AddComponent<Windmill>();
	wmComp->m_type = WINDMILL_NORMAL;
	wmComp->m_pWindmillBody = pWindmill->GetChildren().begin()->second;
	wmComp->m_pWindmillWing[0] = (++pWindmill->GetChildren().begin())->second;
	wmComp->m_pWindmillWing[0]->GetTransform()->m_vScale *= 0.65f;
	wmComp->m_pWindmillWing[0]->GetTransform()->SetPositionLocal(FEVector3(0.0f, 1.5f, 1.0f));
	pWindmill->GetTransform()->SetPositionWorld(FEVector3(0.0f, 0.0f, 0.0f));
	pWindmill->GetTransform()->Rotate(FEVector3(0.0f, 180.0f, 0.0f));

	// 삼단 풍차 만들기
	pWindmill = FEGameObject::CopyObject(FEGameObject::FindPrefab(8507257348452055230));
	wmComp = pWindmill->AddComponent<Windmill>();
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
	pWindmill->GetTransform()->SetPositionWorld(FEVector3(10.0f, 0.0f, 0.0f));
	pWindmill->GetTransform()->Rotate(FEVector3(0.0f, 180.0f, 0.0f));

	// 삼단 풍차 타입 2 만들기
	pWindmill = FEGameObject::CopyObject(FEGameObject::FindPrefab(8507257348452055230));
	wmComp = pWindmill->AddComponent<Windmill>();
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
	pWindmill->GetTransform()->SetPositionWorld(FEVector3(-10.0f, 0.0f, 0.0f));
	pWindmill->GetTransform()->Rotate(FEVector3(0.0f, 180.0f, 0.0f));

	// 영웅 만들기
	FEGameObject* pHero = FEGameObject::CopyObject(FEGameObject::FindPrefab(6576196112351069742));
	pHero->GetTransform()->SetRotationDegree(FEVector3(-90.0f, -90.0f, 0.0f));
	pHero->AddComponent<Hero>()->m_pHero = pHero;

	// 박스 만들기
	FEGameObject* pBox = FEGameObject::CopyObject(FEGameObject::FindPrefab(2460141765634699607));
	pBox->GetTransform()->SetPositionWorld(FEVector3(10.0f, 1.0f, 0.0f));
	
	pSysCom->m_pTree = pTree;
	pSysCom->m_pTerrain = pTerrain;
	pSysCom->m_pHero = pHero;
	pSysCom->m_pBox = pBox;

	FEMaterial* mtrl;
	FEVector2 offset, tiling;
	mtrl = pTerrain->GetChildren().begin()->second->GetRenderer()->m_pMaterial;
	mtrl->GetTextureOffset(0, offset);
	mtrl->GetTextureTiling(0, tiling);
	mtrl->SetVector(0, FEVector4(offset, tiling));
}