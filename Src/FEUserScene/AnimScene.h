#pragma once

#include <IFEScene.h>

class AnimScene : public IFEScene
{
public:
	AnimScene();
	~AnimScene();

	virtual void Load() override;

	virtual void Initialize() override;

	virtual void Release() override;

	virtual void Update() override;
	virtual void Render() override;
};