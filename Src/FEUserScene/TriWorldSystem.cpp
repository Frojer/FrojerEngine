#include "TriWorldSystem.h"
#include <IFEScene.h>
#include <FEDebug.h>
#include <windows.h>
#include <FETime.h>

#define GRAY_COLOR COLOR(0.20f, 0.20f, 0.20f, 1)
#define DARKGRAY_COLOR COLOR(0.12f, 0.12f, 0.12f, 1)
#define WINTER_COLOR COLOR(0.96032f, 0.96032f, 0.96032f, 1.0f)

#ifndef IsKeyDown
#define IsKeyDown(key)	((GetAsyncKeyState(key)&0x8000) == 0x8000)
#define IsKeyUp(key)	((GetAsyncKeyState(key)&0x8001) == 0x8001)
#endif

const FEVector4 TriWorldSystem::s_timeColorBG[4]
{
	FEVector4(0.45098f, 0.76862f, 1.0f, 1.0f),
	FEVector4(1.0f, 0.55294f, 0.05882f, 1.0f),
	FEVector4(0.15294f, 0.15294f, 0.38823f, 1.0f),
	FEVector4(0.55686f, 0.55686f, 0.82353f, 1.0f),
};
const FEVector4 TriWorldSystem::s_timeColorLight[4]
{
	FEVector4(1.0f, 1.0f, 1.0f, 1.0f),
	FEVector4(0.94902f, 0.42745f, 0.05882f, 1.0f),
	FEVector4(0.0f, 0.0f, 0.24118f, 1.0f),
	FEVector4(0.55686f, 0.55686f, 0.82353f, 1.0f),
};

TriWorldSystem::TriWorldSystem()
{
}


TriWorldSystem::~TriWorldSystem()
{
}

void TriWorldSystem::Update()
{
	if (IsKeyUp(VK_F6))
	{
		if (++m_timeSlot == 4)
			m_timeSlot = 0;

		IFEScene::s_BGColor = s_timeColorBG[m_timeSlot];
		m_pDirectionalLight->m_diffuse = s_timeColorLight[m_timeSlot];
	}

	if (IsKeyDown('Z'))	m_pPointLight->m_range += 10.0f * FETime::GetDeltaTime();
	if (IsKeyDown('X'))	m_pPointLight->m_range -= 10.0f * FETime::GetDeltaTime();
	if (IsKeyDown('C'))	m_pHero->GetTransform()->Translate(FEVector3::Up * 10.0f * FETime::GetDeltaTime());
	if (IsKeyDown('V'))	m_pHero->GetTransform()->Translate(FEVector3::Up * -10.0f * FETime::GetDeltaTime());

	//FEDebug::GetInstance()->DrawNormal(m_pHero, FEVector4(0.0f, 1.0f, 0.0f, 1.0f));
	FEDebug::GetInstance()->DrawNormal(m_pBox, FEVector4(0.0f, 1.0f, 0.0f, 1.0f));
	FEDebug::GetInstance()->DrawNormal(m_pTree, FEVector4(0.0f, 1.0f, 0.0f, 1.0f));
	FEDebug::GetInstance()->DrawNormal(m_pTerrain, FEVector4(0.0f, 1.0f, 0.0f, 1.0f));
	FE_SAMPLER_STATE_FLAG sd;
	m_pTerrain->GetChildren().begin()->second->GetRenderer()->m_pMaterial->SetSamplerState(0, sd);
}