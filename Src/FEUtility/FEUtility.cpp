#include "FEUtility.h"

void FileNameExtension(const tstring& i_fileName, tstring& o_extension)
{
	size_t comma = i_fileName.rfind(FE_TEXT("."), i_fileName.size() - 1);
	if (comma == 0xffffffffffffffff) o_extension.clear();
	else o_extension = i_fileName.substr(comma + 1, i_fileName.size() - comma);
}

void GetFilePath(const tstring& filePath, tstring& o_name)
{
	if (filePath.back() == FE_TEXT('"'))
		o_name = filePath.substr(1, filePath.rfind(FE_TEXT("/"), filePath.size() - 2) + 1);
	else
		o_name = filePath.substr(0, filePath.rfind(FE_TEXT("/"), filePath.size() - 1) + 1);
}

void GetFileName(const tstring& filePath, tstring& o_name)
{
	o_name = filePath.substr((filePath.rfind(FE_TEXT("/"), filePath.length() - 1) + 1), (filePath.rfind(FE_TEXT("."), filePath.length() - 1) + 1));
}

void GetFileNameWithExtension(const tstring& filePath, tstring& o_name)
{
	o_name = filePath.substr((filePath.rfind(FE_TEXT("/"), filePath.length() - 1) + 1), filePath.size());
}