#pragma once
#include <FEObjectHeader.h>

enum TIME_SLOT
{
	TIME_DAY,
	TIME_EVENING,
	TIME_NIGHT,
	TIME_TWILIGHT
};



class TriWorldSystem : public FEBehaviour
{
public:
	static const FEVector4 s_timeColorBG[4];
	static const FEVector4 s_timeColorLight[4];

	BYTE m_timeSlot;

	FELight* m_pDirectionalLight;
	FELight* m_pPointLight;
	FEGameObject* m_pTerrain;
	FEGameObject* m_pTree;
	FEGameObject* m_pBox;
	FEGameObject* m_pHero;
public:
	TriWorldSystem();
	virtual ~TriWorldSystem();

	virtual void Update() override;
};

