#include "TriWorldSystem.h"
#include <IFEScene.h>
#include <FEDebug.h>
#include <windows.h>
#include <FETime.h>

#define GRAY_COLOR FEVector4(0.20f, 0.20f, 0.20f, 1)
#define DARKGRAY_COLOR FEVector4(0.12f, 0.12f, 0.12f, 1)
#define WINTER_COLOR FEVector4(0.96032f, 0.96032f, 0.96032f, 1.0f)

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
#define FOG_NONE 100000.0f
#define FOG_NEAR 25.0f
#define FOG_FAR 75.0f

	// 시간 변경
	if (IsKeyUp(VK_F6))		if (++m_timeSlot == TIME_MAX)	m_timeSlot = 0;
	// 계절 변경
	if (IsKeyUp(VK_F7))		if (++m_season == SEASON_MAX)	m_season = 0;

	// 그리드 On/Off
	if (IsKeyUp('1'))	m_pGrid->x = !m_pGrid->x;
	if (IsKeyUp('2'))	m_pGrid->y = !m_pGrid->y;
	if (IsKeyUp('3'))	m_pGrid->z = !m_pGrid->z;
	if (IsKeyUp('4'))	m_bDrawNormal = !m_bDrawNormal;

	// 주인공 횟불 반경 증/감소
	if (IsKeyDown('Z'))	m_pPointLight->m_range += 10.0f * FETime::GetDeltaTime();
	if (IsKeyDown('X'))	m_pPointLight->m_range -= 10.0f * FETime::GetDeltaTime();

	// 주인공 위/아래 이동
	if (IsKeyDown('C'))	m_pHero->GetTransform()->Translate(FEVector3::Up * 10.0f * FETime::GetDeltaTime());
	if (IsKeyDown('V'))	m_pHero->GetTransform()->Translate(FEVector3::Up * -10.0f * FETime::GetDeltaTime());

	// 겨울이고 낮이라면
	if (m_timeSlot == TIME_DAY && m_season == SEASON_WINTER)
	{
		// 배경색을 겨울배경으로 바꾸고
		IFEScene::s_BGColor = WINTER_COLOR;

		// 안개의 색역시 같은 배경색으로 바꾼다
		// FOGCOLOR
		if (m_pBoxMtrl != nullptr)		m_pBoxMtrl->SetVector(1, WINTER_COLOR);
		if (m_pTerrainMtrl != nullptr)	m_pTerrainMtrl->SetVector(2, WINTER_COLOR);
		if (m_pTreeMtrl != nullptr)		m_pTreeMtrl->SetVector(2, WINTER_COLOR);
		if (m_pWindmillMtrl != nullptr)	m_pWindmillMtrl->SetVector(2, WINTER_COLOR);
		for (int i = 0; i < 6; i++)		m_pHeroMtrl[i]->SetVector(0, WINTER_COLOR);
	}
	else
	{
		// 아니라면
		// 배경색을 해당 시간의 배경으로 바꾸고
		IFEScene::s_BGColor = s_timeColorBG[m_timeSlot];

		// 안개의 색역시 같은 배경색으로 바꾼다
		// FOGCOLOR
		if (m_pBoxMtrl != nullptr)		m_pBoxMtrl->SetVector(1, s_timeColorBG[m_timeSlot]);
		if (m_pTerrainMtrl != nullptr)	m_pTerrainMtrl->SetVector(2, s_timeColorBG[m_timeSlot]);
		if (m_pTreeMtrl != nullptr)		m_pTreeMtrl->SetVector(2, s_timeColorBG[m_timeSlot]);
		if (m_pWindmillMtrl != nullptr)	m_pWindmillMtrl->SetVector(2, s_timeColorBG[m_timeSlot]);
		for (int i = 0; i < 6; i++)		m_pHeroMtrl[i]->SetVector(0, s_timeColorBG[m_timeSlot]);
	}

	// 저녁/황혼 이라면 상수버퍼에 정상적인 안개값 주기
	if (m_timeSlot > 1)
	{
		m_pBoxMtrl->SetScalar(1, FOG_NEAR);
		m_pBoxMtrl->SetScalar(2, FOG_FAR);
		m_pTerrainMtrl->SetScalar(1, FOG_NEAR);
		m_pTerrainMtrl->SetScalar(2, FOG_FAR);
		m_pTreeMtrl->SetScalar(1, FOG_NEAR);
		m_pTreeMtrl->SetScalar(2, FOG_FAR);
		m_pWindmillMtrl->SetScalar(1, FOG_NEAR);
		m_pWindmillMtrl->SetScalar(2, FOG_FAR);
		for (int i = 0; i < 6; i++)
		{
			m_pHeroMtrl[i]->SetScalar(0, FOG_NEAR);
			m_pHeroMtrl[i]->SetScalar(1, FOG_FAR);
		}
	}
	else
	{
		m_pBoxMtrl->SetScalar(1, FOG_NONE);
		m_pBoxMtrl->SetScalar(2, FOG_NONE);
		m_pTerrainMtrl->SetScalar(1, FOG_NONE);
		m_pTerrainMtrl->SetScalar(2, FOG_NONE);
		m_pTreeMtrl->SetScalar(1, FOG_NONE);
		m_pTreeMtrl->SetScalar(2, FOG_NONE);
		m_pWindmillMtrl->SetScalar(1, FOG_NONE);
		m_pWindmillMtrl->SetScalar(2, FOG_NONE);
		for (int i = 0; i < 6; i++)
		{
			m_pHeroMtrl[i]->SetScalar(0, FOG_NONE);
			m_pHeroMtrl[i]->SetScalar(1, FOG_NONE);
		}
	}

	// 겨울이라면 풍차 날개의 회전 감소
	if (m_season == SEASON_WINTER)
	{
		if (m_cold > 0.0f)		m_cold -= 0.25f * FETime::GetDeltaTime();
		else					m_cold = 0.0f;
	}
	else
	{
		if (m_cold < 1.0f)		m_cold += 0.25f * FETime::GetDeltaTime();
		else					m_cold = 1.0f;
	}

	// 각 오브젝트의 상수버퍼에 계절 정보 삽입
	if (m_pBoxMtrl != nullptr)	m_pBoxMtrl->SetScalar(0, m_season);
	if (m_pTerrainMtrl != nullptr)	m_pTerrainMtrl->SetScalar(0, m_season);
	if (m_pTreeMtrl != nullptr)		m_pTreeMtrl->SetScalar(0, m_season);
	if (m_pWindmillMtrl != nullptr)	m_pWindmillMtrl->SetScalar(0, m_season);

	// 태양광 색상 시간에 따라 변경
	m_pDirectionalLight->m_diffuse = s_timeColorLight[m_timeSlot];

	// 노말 그리기
	if (m_bDrawNormal)
	{
		//FEDebug::GetInstance()->DrawNormal(m_pHero, FEVector4(0.0f, 1.0f, 0.0f, 1.0f));
		FEDebug::GetInstance()->DrawNormal(m_pBox, FEVector4(0.0f, 1.0f, 0.0f, 1.0f));
		for (int i = 0; i < TREE_MAX; i++)
			FEDebug::GetInstance()->DrawNormal(m_pTrees[i], FEVector4(0.0f, 1.0f, 0.0f, 1.0f));
		FEDebug::GetInstance()->DrawNormal(m_pTerrain, FEVector4(0.0f, 1.0f, 0.0f, 1.0f));
	}

	// 샘플러 설정
	FE_SAMPLER_STATE_FLAG sd;
	m_pTerrain->GetChildren().begin()->second->GetRenderer()->m_pMaterial->SetSamplerState(0, sd);
}