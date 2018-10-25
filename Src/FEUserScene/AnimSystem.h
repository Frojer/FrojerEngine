#pragma once

#include <FEObjectHeader.h>

class AnimSystem : public FEBehaviour
{
public:
	FEGameObject* pLight;
	FEGameObject* Hero;

public:
	AnimSystem() = default;
	virtual ~AnimSystem() = default;

	virtual void Update() override;
};