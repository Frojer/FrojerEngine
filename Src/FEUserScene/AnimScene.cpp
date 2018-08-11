#include "AnimScene.h"
#include "FETEST.h"
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

	FEGameObject* pSys = new FEGameObject(FEVector3(10.0f, 0.0f, 5.0f), FEVector3::Zero, FEVector3::One);
	pSys->m_Name = FE_TEXT("System");
	FEGameObject* pHero = new FEGameObject(FEVector3(0.0f, 10.0f, 5.0f), FEVector3::Zero, FEVector3::One);
	pHero->m_Name = FE_TEXT("Hero");
	FEGameObject* pMonster = new FEGameObject(FEVector3(0.0f, 0.0f, 15.0f), FEVector3::Zero, FEVector3::One);
	pMonster->m_Name = FE_TEXT("Monster");
	FEGameObject* pNPC = new FEGameObject(FEVector3(1.0f, 20.0f, 5.0f), FEVector3::Zero, FEVector3::One);
	pNPC->m_Name = FE_TEXT("NPC");
	FEGameObject* pBoss = new FEGameObject(FEVector3(107.0f, 0.0f, 15.0f), FEVector3::Zero, FEVector3::One);
	pBoss->m_Name = FE_TEXT("Boss");

	pMonster->SetParent(pBoss);

	pSys->AddComponent<FETEST>();

	IFEObject* obj = new IFEObject;

	auto id = pSys->GetComponent<FETEST>()->GetID();

	auto newSys = FEGameObject::CopyObject(pSys);
}