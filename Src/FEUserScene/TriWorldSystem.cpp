#include "TriWorldSystem.h"
#include <FEDebug.h>


TriWorldSystem::TriWorldSystem()
{
}


TriWorldSystem::~TriWorldSystem()
{
}

void TriWorldSystem::Update()
{
	//FEDebug::GetInstance()->DrawNormal(m_pHero, FEVector4(0.0f, 1.0f, 0.0f, 1.0f));
	FEDebug::GetInstance()->DrawNormal(m_pBox, FEVector4(0.0f, 1.0f, 0.0f, 1.0f));
	FEDebug::GetInstance()->DrawNormal(m_pTree, FEVector4(0.0f, 1.0f, 0.0f, 1.0f));
	FEDebug::GetInstance()->DrawNormal(m_pTerrain, FEVector4(0.0f, 1.0f, 0.0f, 1.0f));
	FE_SAMPLER_STATE_FLAG sd;
	m_pTerrain->GetChildren().begin()->second->GetRenderer()->m_pMaterial->SetSamplerState(0, sd);
}