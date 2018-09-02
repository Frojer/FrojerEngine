#pragma once

#ifndef _FE_DEFINE
#define _FE_DEFINE
#endif

#include <tchar.h>
#include <string>
#include <fstream>
#include <sstream>

typedef unsigned char		BYTE;
typedef unsigned long       DWORD;
typedef unsigned short      WORD;
typedef unsigned int		UINT;


///////////////////////
//    ���ڿ� ������    //
///////////////////////
typedef TCHAR*			LPTSTR;
typedef const TCHAR*	LPCTSTR;

#ifdef _UNICODE
typedef std::wstring tstring;
typedef std::wfstream tfstream;
typedef std::wifstream tifstream;
typedef std::wofstream tofstream;
typedef std::wstringstream tstringstream;
typedef std::wistringstream tistringstream;
typedef std::wostringstream tostringstream;
typedef std::wios tios;
#else
typedef std::string tstring;
typedef std::fstream tfstream;
typedef std::ifstream tifstream;
typedef std::ofstream tofstream;
typedef std::stringstream tstringstream;
typedef std::istringstream tistringstream;
typedef std::ostringstream tostringstream;
typedef std::ios tios;
#endif

#define FE_TEXT(x) _TEXT(x)
#define TCSCMP_SAME(buf, str) _tcsnicmp((buf), (str), _tcslen((str)) + 1) == 0

/////////////////////
//    ��ũ�� ����    //
/////////////////////
#define SAFE_RELEASE(pObj)	if ((pObj) != nullptr)	{ (pObj)->Release();	(pObj) = nullptr; }
#define SAFE_DELARR(pObj)	if ((pObj) != nullptr)	{ delete[] (pObj);		(pObj) = nullptr; }
#define SAFE_DELETE(pObj)	if ((pObj) != nullptr)	{ delete (pObj);		(pObj) = nullptr; }
#define FOR_STL(stl) for (auto iter = (stl).begin(); iter != (stl).end(); iter++)