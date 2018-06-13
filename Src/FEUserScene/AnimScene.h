#pragma once

#include <IFEScene.h>

class AnimScene : public IFEScene
{
public:
	AnimScene();
	~AnimScene();

	virtual void Load() override;
};