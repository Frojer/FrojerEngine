#pragma once
#ifndef _FE_SYSTEM
#define _FE_SYSTEM

#include "FEWindow.h"

class FESystem
{
private:
	static FESystem* _pInstance;

	void* _pInstanceHandle;
	FEWindow* _pWindow;

public:
	bool m_bExit;

private:
	FESystem();

public:
	~FESystem();

	bool Create(LPCTSTR i_sWindowName, void* i_pInstanceHandle);

	void Run();

	bool LoadData();

	void Release();

	static FESystem* GetInstance();

	friend class FEDirectInput;
};

#endif