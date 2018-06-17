#pragma once
#ifndef _FE_SYSTEM
#define _FE_SYSTEM

#include <IFERenderer.h>
#include "FEWindow.h"

class FESystem
{
private:
	static FESystem* _pInstance;

	FEWindow* _pWindow;
	IFERenderer* _pRenderer;

public:
	bool m_bExit;

private:
	FESystem();

	FESystemSetting LoadSetting();

public:
	~FESystem();

	bool Create(LPCTSTR i_sWindowName);

	void Run();

	bool LoadData();

	void Release();

	IFERenderer* GetRenderer();

	static FESystem* GetInstance();
};

#endif