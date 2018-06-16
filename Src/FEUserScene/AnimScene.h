#pragma once
#ifndef _ANIM_SCENE
#define _ANIM_SCENE
#endif

#include <IFEScene.h>

class AnimScene : public IFEScene
{
public:
	AnimScene();
	~AnimScene();

	virtual void Load() override;
};