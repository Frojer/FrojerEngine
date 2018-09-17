#pragma once
#include <FEObjectHeader.h>

class TriWorldSystem : public FEBehaviour
{
public:
	FEGameObject* m_pHero;
	FEGameObject* m_pBox;
public:
	TriWorldSystem();
	virtual ~TriWorldSystem();

	virtual void Update() override;
};

