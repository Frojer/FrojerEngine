#pragma once

#ifndef _FE_DEFINE
#define _FE_DEFINE
#endif

#include <tchar.h>
#include <string>
#include <fstream>

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
#else
typedef std::string tstring;
typedef std::fstream tfstream;
typedef std::ifstream tifstream;
typedef std::ofstream tofstream;
#endif



/////////////////////
//    ��ũ�� ����    //
/////////////////////
#define SAFE_RELEASE(pObj)	if ((pObj) != nullptr)	{ (pObj)->Release();	(pObj) = nullptr; }
#define SAFE_DELARR(pObj)	if ((pObj) != nullptr)	{ delete[] (pObj);		(pObj) = nullptr; }
#define SAFE_DELETE(pObj)	if ((pObj) != nullptr)	{ delete (pObj);		(pObj) = nullptr; }
#define FOR_STL(stl) for (auto iter = (stl).begin(); iter != (stl).end(); iter++)