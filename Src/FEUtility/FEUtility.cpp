#include "FEUtility.h"

tstring GetFileNameExtension(const tstring& i_fileName)
{
	tstring extension;
	size_t comma = i_fileName.rfind(FE_TEXT("."), i_fileName.size() - 1);
	if (comma == 0xffffffffffffffff) extension.clear();
	else extension = i_fileName.substr(comma + 1, i_fileName.size() - comma);

	return extension;
}
tstring GetFilePath(const tstring& filePath)
{
	if (filePath.back() == FE_TEXT('"'))
		return filePath.substr(1, filePath.rfind(FE_TEXT("/"), filePath.size() - 2) + 1);
	else
		return filePath.substr(0, filePath.rfind(FE_TEXT("/"), filePath.size() - 1) + 1);
}
tstring GetFileName(const tstring& filePath)
{
	auto lpos = filePath.rfind(FE_TEXT("/"), filePath.length() - 1) + 1;
	auto rpos = filePath.rfind(FE_TEXT("."), filePath.length() - 1);
	return filePath.substr(lpos, rpos - lpos);
}
tstring GetFileNameWithExtension(const tstring& filePath)
{
	return filePath.substr((filePath.rfind(FE_TEXT("/"), filePath.length() - 1) + 1), filePath.size());
}