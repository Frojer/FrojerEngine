#pragma once

#include <tchar.h>
#include <string>


///////////////////////
//    문자열 재정의    //
///////////////////////
typedef unsigned int	UINT;
typedef TCHAR*			LPTSTR;
typedef const TCHAR*	LPCTSTR;

#ifdef _UNICODE
typedef std::wstring tstring;
#elif
typedef std::string tstring;
#endif



/////////////////////
//    매크로 정의    //
/////////////////////
#define SAFE_RELEASE(pObj)	if ((pObj) != nullptr)	{ (pObj)->Release();	(pObj) = nullptr; }
#define SAFE_DELARR(pObj)	if ((pObj) != nullptr)	{ delete[] (pObj);		(pObj) = nullptr; }
#define SAFE_DELETE(pObj)	if ((pObj) != nullptr)	{ delete (pObj);		(pObj) = nullptr; }