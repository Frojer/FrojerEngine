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

	FEObject* pSys = new FEObject();
	pSys->m_Name = _TEXT("System");
	FEObject* pHero = new FEObject();
	pHero->m_Name = _TEXT("Hero");
	FEObject* pMonster = new FEObject();
	pMonster->m_Name = _TEXT("Monster");
	FEObject* pNPC = new FEObject();
	pNPC->m_Name = _TEXT("NPC");
	FEObject* pBoss = new FEObject();
	pBoss->m_Name = _TEXT("Boss");
}