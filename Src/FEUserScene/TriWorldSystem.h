#pragma once
#include <FEObjectHeader.h>

class TriWorldSystem : public FEBehaviour
{
public:
	FEGameObject* m_pTerrain;
	FEGameObject* m_pTree;
	FEGameObject* m_pBox;
	FEGameObject* m_pHero;
public:
	TriWorldSystem();
	virtual ~TriWorldSystem();

	virtual void Update() override;
};

