#pragma once

#include <FEObjectHeader.h>
#include "TriWorldSystem.h"

enum WINDMILL_TYPE
{
	WINDMILL_NORMAL,
	WINDMILL_TRIPLE_TYPE1,
	WINDMILL_TRIPLE_TYPE2
};

class Windmill : public FEBehaviour
{
public:
	TriWorldSystem* m_pSystem;
	WINDMILL_TYPE m_type;
	FEGameObject* m_pWindmillBody;
	FEGameObject* m_pWindmillWing[3];

public:
	Windmill();
	virtual ~Windmill();

	virtual void Update() override;
};