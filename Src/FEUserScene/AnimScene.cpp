#include "AnimScene.h"

AnimScene::AnimScene()
{
}


AnimScene::~AnimScene()
{
}


void AnimScene::Load()
{
	IFEScene::s_backgroundColor = FEVector4(0, 0.125f, 0.3f, 1);
}