#pragma once
#ifndef _FE_ANIMATION_CLIP
#define _FE_ANIMATION_CLIP

#include <map>

#include "FEObjectHeader.h"

class FEAnimationClip : public FEObject
{
	std::map<float, FEVector3> animPos;
	std::map<float, FEVector3> animRot;
	std::map<float, FEVector3> animScale;

public:
	FEAnimationClip();
	virtual ~FEAnimationClip();
};

#endif