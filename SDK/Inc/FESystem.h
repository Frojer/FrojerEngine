#pragma once

#include "FEWindow.h"

class FESystem
{
private:
	static FESystem* _pInstance;

	FEWindow* _pWindow;

public:
	bool m_bExit;

private:
	FESystem();
public:
	~FESystem();

	bool Create(LPCTSTR i_sWindowName, const UINT i_width, const UINT i_height);

	void Run();

	void Release();

	static FESystem* GetInstance();
};