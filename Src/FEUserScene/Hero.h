#pragma once

#include <FEObjectHeader.h>

class Hero : public FEBehaviour
{
public:
	FEGameObject* m_pHero;

public:
	Hero();
	virtual ~Hero();

	virtual void Update() override;
};