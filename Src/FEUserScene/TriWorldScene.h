#pragma once
#ifndef _TRIWORLD_SCENE
#define _TRIWORLD_SCENE

#include <IFEScene.h>

class TriWorldScene : public IFEScene
{
public:
	TriWorldScene();
	virtual ~TriWorldScene();

	virtual void Load() override;
};

#endif