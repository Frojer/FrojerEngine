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
	FEGameObject* pCam = new FEGameObject(FEVector3(-6.83591080f, 9.19818401f, -21.5910091f), FEVector3::Zero, FEVector3::One);
	pCam->GetTransform()->SetRotationQuaternion(FEVector4(0.212332159f, 0.306440085f, -0.0703246295f, 0.925235093f));
	pCam->AddComponent<CameraController>()->pCam = pCam->AddComponent<FECamera>();

	// 그리드 생성하기
	FEGameObject* pGrid = new FEGameObject();
	auto grid = pGrid->AddComponent<Grid>();

	pSysCom->m_pGrid = grid;

	// 터레인 만들기
	FEGameObject* pTerrain = FEGameObject::CopyObject(FEGameObject::FindPrefab(4417013628616341411));
	pTerrain->GetTransform()->m_vScale = FEVector3(0.5f, 0.5f, 0.5f);
	pSysCom->m_pTerrainMtrl = pTerrain->GetRenderer()->m_pMaterial;
	pSysCom->m_pTerrainMtrl->SetShader(FEShader::Find(FE_TEXT("WinterShader")));
	pSysCom->m_pTerrainMtrl->SetVector(0, FEVector4(0.0f, 0.0f, 10.0f, 10.0f));
	pSysCom->m_pTerrainMtrl->SetVector(1, AUTUMN_COLOR);
	pSysCom->m_pTerrainMtrl->SetTexture(1, FETexture::Find(5721527515870738053));
	//FEVector2 offset;
	//FEVector2 tiling;
	//pTerrain->GetChildren().begin()->second->GetRenderer()->m_pMaterial->GetTextureOffset(0, offset);
	//pTerrain->GetChildren().begin()->second->GetRenderer()->m_pMaterial->GetTextureTiling(0, tiling);
	//tiling *= 10;
	//pTerrain->GetChildren().begin()->second->GetRenderer()->m_pMaterial->SetVector(0, FEVector4(offset, tiling));

	// 나무 만들기
	FEGameObject* pTree = FEGameObject::CopyObject(FEGameObject::FindPrefab(-1394610338352907823));
	pSysCom->m_pTrees[0] = pTree;
	pTree->GetRenderer()->SetRenderPriority(2);
	pSysCom->m_pTreeMtrl = pTree->GetRenderer()->m_pMaterial;
	pSysCom->m_pTreeMtrl->SetShader(FEShader::Find(FE_TEXT("WinterShader")));
	pSysCom->m_pTreeMtrl->SetVector(0, FEVector4(0.0f, 0.0f, 1.0f, 1.0f));
	pSysCom->m_pTreeMtrl->SetVector(1, AUTUMN_COLOR);
	pSysCom->m_pTreeMtrl->SetTexture(1, FETexture::Find(3048992087290116511));
	
	srand(GetTickCount());
	for (int i = 1; i < TREE_MAX; i++)
	{
		FEVector3 pos;
		pos.x = (rand() % 100) - 50;
		pos.z = (rand() % 100) - 50;
		pTree = FEGameObject::CopyObject(pTree);
		pTree->GetTransform()->SetPositionWorld(pos);
		//pTree->GetTransform()->SetRotationDegree(FEVector3(0.0f, 180.0f, 0.0f));

		pSysCom->m_pTrees[i] = pTree;
	}

	FE_BLEND_DESC bd;
	bd.RenderTarget[0].BlendEnable = true;
	bd.RenderTarget[0].BlendOp = FE_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlend = FE_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = FE_BLEND_INV_SRC_ALPHA;
	
	// 일반 풍차 만들기
	FEGameObject* pWindmill = FEGameObject::CopyObject(FEGameObject::FindPrefab(6289458781704997571));
	pWindmill->GetChildren().begin()->second->GetRenderer()->SetRenderPriority(3);
	pWindmill->GetChildren().begin()->second->GetRenderer()->SetBlendState(bd);
	pWindmill->GetChildren().begin()->second->GetRenderer()->m_renderType = RENDER_TYPE_TWOFACE;
	pSysCom->m_pWindmillMtrl = pWindmill->GetChildren().begin()->second->GetRenderer()->m_pMaterial;
	pSysCom->m_pWindmillMtrl->SetShader(FEShader::Find(FE_TEXT("WinterShader")));
	pSysCom->m_pWindmillMtrl->SetVector(0, FEVector4(0.0f, 0.0f, 1.0f, 1.0f));
	pSysCom->m_pWindmillMtrl->SetVector(1, FEVector4::One);
	pSysCom->m_pWindmillMtrl->SetTexture(1, FETexture::Find(-5009514137093084626));
	auto wmComp = pWindmill->AddComponent<Windmill>();
	wmComp->m_pSystem = pSysCom;
	wmComp->m_type = WINDMILL_NORMAL;
	wmComp->m_pWindmillBody = pWindmill->GetChildren().begin()->second;
	wmComp->m_pWindmillWing[0] = (++pWindmill->GetChildren().begin())->second;
	wmComp->m_pWindmillWing[0]->GetRenderer()->SetBlendState(bd);
	wmComp->m_pWindmillWing[0]->GetTransform()->m_vScale *= 0.65f;
	wmComp->m_pWindmillWing[0]->GetTransform()->SetPositionLocal(FEVector3(0.0f, 1.5f, 1.0f));
	pWindmill->GetTransform()->SetPositionWorld(FEVector3(5.0f, -0.0002f, -10.0f));
	pWindmill->GetTransform()->Rotate(FEVector3(0.0f, 180.0f, 0.0f));

	// 삼단 풍차 만들기
	pWindmill = FEGameObject::CopyObject(FEGameObject::FindPrefab(6289458781704997571));
	pWindmill->GetChildren().begin()->second->GetRenderer()->SetRenderPriority(3);
	pWindmill->GetChildren().begin()->second->GetRenderer()->SetBlendState(bd);
	pWindmill->GetChildren().begin()->second->GetRenderer()->m_renderType = RENDER_TYPE_TWOFACE;
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
	wmComp->m_pWindmillWing[0]->GetRenderer()->SetBlendState(bd);
	wmComp->m_pWindmillWing[1]->GetRenderer()->SetBlendState(bd);
	wmComp->m_pWindmillWing[2]->GetRenderer()->SetBlendState(bd);
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
	pWindmill = FEGameObject::CopyObject(FEGameObject::FindPrefab(6289458781704997571));
	pWindmill->GetChildren().begin()->second->GetRenderer()->SetRenderPriority(3);
	pWindmill->GetChildren().begin()->second->GetRenderer()->SetBlendState(bd);
	pWindmill->GetChildren().begin()->second->GetRenderer()->m_renderType = RENDER_TYPE_TWOFACE;
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
	wmComp->m_pWindmillWing[0]->GetRenderer()->SetBlendState(bd);
	wmComp->m_pWindmillWing[1]->GetRenderer()->SetBlendState(bd);
	wmComp->m_pWindmillWing[2]->GetRenderer()->SetBlendState(bd);
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
	FEGameObject* pHero = FEGameObject::CopyObject(FEGameObject::FindPrefab(6640943049090391582));
	pHero->GetTransform()->SetPositionLocal(FEVector3(2.20766687f, 0.0f, -13.5519924f));
	pHero->GetTransform()->SetRotationQuaternion(FEVector4(0.0f, -0.0416140929f, 0.000000000f, -0.999135017f));
	pHero->AddComponent<Hero>()->m_pHero = pHero;
	auto heroChildren = pHero->GetChildren();
	for (auto iter = heroChildren.begin(); iter != heroChildren.end(); iter++)
	{
		(*iter).second->GetRenderer()->m_pMaterial->SetShader(FEShader::Find(239857098132532));
		(*iter).second->GetRenderer()->m_renderType = RENDER_TYPE_TWOFACE;
		tstring name = (*iter).second->GetRenderer()->m_pMaterial->m_Name;

		if (name == FE_TEXT("01_Weapon"))
		{
			(*iter).second->GetRenderer()->m_pMaterial->SetTexture(1, FETexture::Find(-3456410373712560427));
			pSysCom->m_pHeroMtrl[0] = (*iter).second->GetRenderer()->m_pMaterial;
		}
		else if (name == FE_TEXT("02_Body"))
		{
			(*iter).second->GetRenderer()->m_pMaterial->SetTexture(1, FETexture::Find(4387018058212695514));
			pSysCom->m_pHeroMtrl[1] = (*iter).second->GetRenderer()->m_pMaterial;
		}
		else if (name == FE_TEXT("03_Armor"))
		{
			(*iter).second->GetRenderer()->m_pMaterial->SetTexture(1, FETexture::Find(2832331587261179096));
			pSysCom->m_pHeroMtrl[2] = (*iter).second->GetRenderer()->m_pMaterial;
		}
		else if (name == FE_TEXT("04-Head"))
		{
			(*iter).second->GetRenderer()->m_pMaterial->SetTexture(1, FETexture::Find(-7742163606095368462));
			pSysCom->m_pHeroMtrl[3] = (*iter).second->GetRenderer()->m_pMaterial;
		}
		else if (name == FE_TEXT("05-HeadGear"))
		{
			(*iter).second->GetRenderer()->m_pMaterial->SetTexture(1, FETexture::Find(-3498842328099961521));
			pSysCom->m_pHeroMtrl[4] = (*iter).second->GetRenderer()->m_pMaterial;
		}
		else if (name == FE_TEXT("06-Pack"))
		{
			(*iter).second->GetRenderer()->m_pMaterial->SetTexture(1, FETexture::Find(4959284352556493848));
			pSysCom->m_pHeroMtrl[5] = (*iter).second->GetRenderer()->m_pMaterial;
		}
	}

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
	pPointLight->GetTransform()->SetPositionLocal(FEVector3(1.0f, 1.0f, 0.0f));
	pPointLight->GetTransform()->m_vScale = FEVector3(0.02f, 0.02f, 0.02f);
	auto pointLitCom = pPointLight->AddComponent<FELight>();
	pointLitCom->m_diffuse = FEVector4(1.0f, 0.40784f, 0.090196f, 1.0f);
	pointLitCom->m_ambient = FEVector4(0.5f, 0.20392f, 0.045098f, 1.0f);
	pointLitCom->m_range = 15.0f;
	pointLitCom->m_lightType = FE_LIGHT_TYPE_POINT;

	FEGameObject* pTorch = FEGameObject::CopyObject(FEGameObject::FindPrefab(6180508671031374862));
	pTorch->SetParent(pPointLight);
	pTorch->GetRenderer()->m_pMaterial->SetShader(FEShader::Find(2465136134763));

	// 박스 만들기
	FEGameObject* pBox = FEGameObject::CopyObject(FEGameObject::FindPrefab(-6008426018645421450));
	pBox->GetRenderer()->SetRenderPriority(0);
	pSysCom->m_pBoxMtrl = pBox->GetRenderer()->m_pMaterial;
	pSysCom->m_pBoxMtrl->SetShader(FEShader::Find(FE_TEXT("Box")));
	pSysCom->m_pBoxMtrl->SetVector(0, AUTUMN_COLOR);
	pSysCom->m_pBoxMtrl->SetTexture(1, FETexture::Find(4940300976105062166));
	pSysCom->m_pBoxMtrl->SetTexture(2, FETexture::Find(8496873953192865247));
	pSysCom->m_pBoxMtrl->SetTexture(3, FETexture::Find(-690700366506641043));
	pBox->GetTransform()->SetPositionWorld(FEVector3(-3.0f, 1.0f, -25.0f));
	
	pSysCom->m_pDirectionalLight = light;
	pSysCom->m_pPointLight = pointLitCom;
	pSysCom->m_pTerrain = pTerrain;
	pSysCom->m_pHero = pHero;
	pSysCom->m_pBox = pBox;
}