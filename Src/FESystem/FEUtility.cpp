#include "FEUtility.h"

LPCTSTR FileNameExtension(LPCTSTR i_fileName, LPTSTR o_extension, UINT i_size)
{
	UINT count = 0;
	UINT comma = 0;

	while (i_fileName[count++] != '\0') {}
	comma = --count;
	while (i_fileName[--comma] != '.') {}

	_tcsncpy_s(o_extension, i_size, &i_fileName[comma], count - comma + 1 >= i_size ? i_size : count - comma + 1);

	return o_extension;
}

void GetFilePath(const tstring& filePath, tstring& o_name)
{
	if (filePath.back() == FE_TEXT('"'))
		o_name = filePath.substr(0, filePath.rfind(FE_TEXT("/"), filePath.length() - 1) + 1);
	else
		o_name = filePath.substr(0, filePath.rfind(FE_TEXT("/"), filePath.length() - 1) + 1);
}

void GetFileName(const tstring& filePath, tstring& o_name)
{
	o_name = filePath.substr((filePath.rfind(FE_TEXT("/"), filePath.length() - 1) + 1), (filePath.rfind(FE_TEXT("."), filePath.length() - 1) + 1));
}

void GetFileNameWithExtension(const tstring& filePath, tstring& o_name)
{
	o_name = filePath.substr((filePath.rfind(FE_TEXT("/"), filePath.length() - 1) + 1), filePath.size());
}