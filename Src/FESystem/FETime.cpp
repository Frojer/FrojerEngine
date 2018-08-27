#include "FETime.h"

float FETime::_timeScale = 1.0f;
float FETime::_deltaTime = 0.0f;

void FETime::TimeUpdate()
{

}

float FETime::GetPureDeltaTime()
{
	return _deltaTime;
}
float FETime::GetDeltaTime()
{
	return _deltaTime * _timeScale;
}

float FETime::GetTimeScale()
{
	return _timeScale;
}
void FETime::SetTimeScale(float timeScale)
{
	timeScale = _timeScale;
}