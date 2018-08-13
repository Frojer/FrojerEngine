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

	/*
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
	*/

	VF_PNT v;
	IndexFormat i;
	memset(&v, 0, sizeof(VF_PNT));
	std::vector<VF_PNT> verts;
	v.pos = FEVector3(-0.5f, -0.5f, 0.0f);
	verts.push_back(v);
	v.pos = FEVector3(0.0f, 0.5f, 0.0f);
	verts.push_back(v);
	v.pos = FEVector3(0.5f, -0.5f, 0.0f);
	verts.push_back(v);

	std::vector<IndexFormat> indcs;
	i.x = 0;
	i.y = 1;
	i.z = 2;
	indcs.push_back(i);

	FEMesh* pMesh = FEMesh::CreateMesh(verts, indcs);
	pMesh->m_Name = FE_TEXT("DemoMesh");

	FEGameObject* pTriangle = new FEGameObject(FEVector3::Zero, FEVector3::Zero, FEVector3::One);
	pTriangle->m_Name = FE_TEXT("»ï°¢Çü");
	FERenderer* renderer = pTriangle->AddComponent<FERenderer>();
	renderer->m_pMaterial = FEMaterial::Find(FE_TEXT("DemoMaterial"));
	renderer->m_pMesh = pMesh;
	renderer->SetPrimitiveTopology(PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//FEShader* pShader = new FEShader;
	//pTriangle->GetRenderer()->m_pMaterial = new FEMaterial()

}