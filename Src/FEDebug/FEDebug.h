#pragma once
#ifndef _FE_DEBUG
#define _FE_DEBUG

#include <FEDefine.h>

class FEDebug
{
public:
	FEDebug();
	~FEDebug();

	static bool Initialize();
	static void Release();

	static void ErrorMessage(TCHAR* text);
	static void WarningMessage(TCHAR* text);
};
#endif