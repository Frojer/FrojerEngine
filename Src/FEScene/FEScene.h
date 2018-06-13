#pragma once

#include <vector>
#include "IFEScene.h"

class FEScene :	public IFEScene
{

public:
	FEScene();
	virtual ~FEScene();

	virtual void Load() override;

	virtual void Initialize() override;
	virtual void Release() override;

	virtual void Update() override;
	virtual void Render() override;
};