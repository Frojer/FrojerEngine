#include "FETime.h"
#include <Windows.h>

#pragma comment(lib, "Winmm")

FETime* FETime::_pInstance = nullptr;

FETime::FETime()
	: _timeScale(1.0f), _deltaTime(0.0f), _curTime(0), _oldTime(0)
{
	// 엔진시간 계산시 dTime 의 정밀도 개선.★
	timeBeginPeriod(1);

	// 프로그램의 priority 를 높임.★	 
	//::SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
	// 프로세스 내부에서 thread priority 를 높임.
	//AfxBeginThread(MyThead, this, THREAD_PRIORITY_TIME_CRITICAL);

	_curTime = _oldTime = timeGetTime();
}
FETime::~FETime()
{
	// 엔진시간 정밀도 리셋.
	timeEndPeriod(1);
}

void FETime::TimeUpdate()
{
	_oldTime = _curTime;
	_curTime = timeGetTime();

	_deltaTime = (_curTime - _oldTime) * 0.001f;
}

float FETime::GetPureDeltaTime()
{
	return _pInstance->_deltaTime;
}
float FETime::GetDeltaTime()
{
	return _pInstance->_deltaTime * _pInstance->_timeScale;
}

float FETime::GetTimeScale()
{
	return _pInstance->_timeScale;
}
void FETime::SetTimeScale(float timeScale)
{
	timeScale = _pInstance->_timeScale;
}