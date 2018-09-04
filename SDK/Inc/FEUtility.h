#pragma once
#ifndef _FE_UTILITY
#define _FE_UTILITY

#include <FEDefine.h>

tstring GetFileNameExtension(const tstring& i_fileName);
tstring GetFilePath(const tstring& filePath);
tstring GetFileName(const tstring& filePath);
tstring GetFileNameWithExtension(const tstring& filePath);
tstring StripQuotes(const tstring& str);

bool FECopyFile(tifstream& orig, tofstream& dest);
INT64 CreateUUIDHashCode64();
#endif