#pragma once

#include "FEWindow.h"

#pragma region FEScene.lib
#ifdef _DEBUG
#ifdef _WIN64
#pragma comment(lib, "FEScene64d")
#else
#pragma comment(lib, "FEScened")
#endif
#else
#ifdef _WIN64
#pragma comment(lib, "FEScene64")
#else
#pragma comment(lib, "FEScene")
#endif
#endif
#pragma endregion

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

	bool LoadData();

	void Release();

	static FESystem* GetInstance();
};