#pragma once
#ifndef _FE_TIME
#define _FE_TIME

class FETime
{
private:
	static float _timeScale;
	static float _deltaTime;

private:
	FETime() = delete;
	~FETime() = default;

	static void TimeUpdate();

public:

	static float GetPureDeltaTime();
	static float GetDeltaTime();

	static float GetTimeScale();
	static void SetTimeScale(float timeScale);


	friend class FESystem;
};
#endif