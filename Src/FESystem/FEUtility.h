#pragma once

#include <FEDefine.h>

void FileNameExtension(const tstring& i_fileName, tstring& o_extension);
void GetFilePath(const tstring& filePath, tstring& o_name);
void GetFileName(const tstring& filePath, tstring& o_name);
void GetFileNameWithExtension(const tstring& filePath, tstring& o_name);