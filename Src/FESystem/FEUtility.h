#pragma once

#include <FEDefine.h>

LPCTSTR FileNameExtension(LPCTSTR i_fileName, LPTSTR o_extension, UINT i_size);
void GetFilePath(const tstring& filePath, tstring& o_name);
void GetFileName(const tstring& filePath, tstring& o_name);
void GetFileNameWithExtension(const tstring& filePath, tstring& o_name);