#pragma once
#ifndef _FE_ANIMATION
#define _FE_ANIMATION

#include "FEObjectHeader.h"

class FEAnimation :	public FEBehaviour
{
public:
	enum
	{
		RUN,
		PAUSE
	};
	struct ANIM_POS
	{
		float animTime;
		FEVector3 pos;
	};
	struct ANIM_ROT
	{
		float animTime;
		FEVector3 axis;
		float angle;
	};
	struct ANIM_SCALE
	{
		float animTime;
		FEVector3 scale;
		FEVector3 axis;
		float angle;
	};

	char _animState;
	float _animTime;

public:
	bool _bLoop;
	float _maxAnimTime;
	std::vector<ANIM_POS> m_animPos;
	std::vector<ANIM_ROT> m_animRot;
	std::vector<ANIM_SCALE> m_animScale;

protected:
	virtual void AnimationUpdate() override;

public:
	FEAnimation();
	virtual ~FEAnimation();

	void Play();
	void Pause();
	void Stop();
};
#endif