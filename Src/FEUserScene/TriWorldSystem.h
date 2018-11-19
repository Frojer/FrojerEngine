#pragma once
#include <FEObjectHeader.h>
#include "Grid.h"

#define TREE_MAX 300

enum TIME_SLOT
{
	TIME_DAY,
	TIME_EVENING,
	TIME_NIGHT,
	TIME_TWILIGHT,
	TIME_MAX
};

enum SEASON
{
	SEASON_SUMMER,
	SEASON_AUTUMN,
	SEASON_WINTER,
	SEASON_MAX
};

class TriWorldSystem : public FEBehaviour
{
public:
	static const FEVector4 s_timeColorBG[4];
	static const FEVector4 s_timeColorLight[4];

	BYTE m_timeSlot;
	BYTE m_season;

	float m_cold = 1.0f;
	bool m_bDrawNormal = false;

	Grid* m_pGrid;

	FELight* m_pDirectionalLight;
	FELight* m_pPointLight;
	FEGameObject* m_pTerrain;
	FEGameObject* m_pTrees[TREE_MAX];
	FEGameObject* m_pBox;
	FEGameObject* m_pHero;
	FEMaterial* m_pTerrainMtrl;
	FEMaterial* m_pTreeMtrl;
	FEMaterial* m_pBoxMtrl;
	FEMaterial* m_pHeroMtrl[6];
	FEMaterial* m_pWindmillMtrl;
public:
	TriWorldSystem();
	virtual ~TriWorldSystem();

	virtual void Update() override;
};

