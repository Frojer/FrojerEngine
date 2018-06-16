#include "AnimScene.h"
#include <FEObject.h>

AnimScene::AnimScene()
{
}


AnimScene::~AnimScene()
{
}


void AnimScene::Load()
{
	IFEScene::s_BGColor = FEVector4(0, 0.125f, 0.3f, 1);

	FEObject* pSys = new FEObject(FEVector3(10.0f, 0.0f, 5.0f), FEVector3::Zero, FEVector3::One);
	pSys->m_Name = _TEXT("System");
	FEObject* pHero = new FEObject(FEVector3(0.0f, 10.0f, 5.0f), FEVector3::Zero, FEVector3::One);
	pHero->m_Name = _TEXT("Hero");
	FEObject* pMonster = new FEObject(FEVector3(0.0f, 0.0f, 15.0f), FEVector3::Zero, FEVector3::One);
	pMonster->m_Name = _TEXT("Monster");
	FEObject* pNPC = new FEObject(FEVector3(1.0f, 20.0f, 5.0f), FEVector3::Zero, FEVector3::One);
	pNPC->m_Name = _TEXT("NPC");
	FEObject* pBoss = new FEObject(FEVector3(107.0f, 0.0f, 15.0f), FEVector3::Zero, FEVector3::One);
	pBoss->m_Name = _TEXT("Boss");

	pMonster->SetParent(pBoss);
}