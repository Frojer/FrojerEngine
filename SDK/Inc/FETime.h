#pragma once
#ifndef _FE_TIME
#define _FE_TIME

#include <FEDefine.h>

class FETime
{
private:
	DWORD _curTime;
	DWORD _oldTime;
	float _timeScale;
	float _deltaTime;

	static FETime* _pInstance;

private:
	FETime();
	~FETime();

	void TimeUpdate();

public:

	static float GetPureDeltaTime();
	static float GetDeltaTime();

	static float GetTimeScale();
	static void SetTimeScale(float timeScale);


	friend class FESystem;
};
#endif