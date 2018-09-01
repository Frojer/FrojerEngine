#include "FETime.h"
#include <Windows.h>

#pragma comment(lib, "Winmm")

FETime* FETime::_pInstance = nullptr;

FETime::FETime()
	: _timeScale(1.0f), _deltaTime(0.0f), _curTime(0), _oldTime(0)
{
	// �����ð� ���� dTime �� ���е� ����.��
	timeBeginPeriod(1);

	// ���α׷��� priority �� ����.��	 
	//::SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
	// ���μ��� ���ο��� thread priority �� ����.
	//AfxBeginThread(MyThead, this, THREAD_PRIORITY_TIME_CRITICAL);

	_curTime = _oldTime = timeGetTime();
}
FETime::~FETime()
{
	// �����ð� ���е� ����.
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