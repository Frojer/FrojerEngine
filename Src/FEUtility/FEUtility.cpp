#include "FEUtility.h"

#include <fstream>

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
	auto rpos = filePath.rfind(FE_TEXT("/"), filePath.size() - 1) + 1;
	if (rpos == 0) rpos = filePath.rfind(FE_TEXT("\\"), filePath.size() - 1) + 1;

	if (filePath.back() == FE_TEXT('"'))
		return filePath.substr(1, rpos);
	else
		return filePath.substr(0, rpos);
}
tstring GetFileName(const tstring& filePath)
{
	auto lpos = filePath.rfind(FE_TEXT("/"), filePath.length() - 1) + 1;
	if (lpos == 0) lpos = filePath.rfind(FE_TEXT("\\"), filePath.length() - 1) + 1;
	auto rpos = filePath.rfind(FE_TEXT("."), filePath.length() - 1);
	return filePath.substr(lpos, rpos - lpos);
}
tstring GetFileNameWithExtension(const tstring& filePath)
{
	auto lpos = filePath.rfind(FE_TEXT("/"), filePath.length() - 1) + 1;
	if (lpos == 0) lpos = filePath.rfind(FE_TEXT("\\"), filePath.length() - 1) + 1;
	auto rpos = filePath.size();
	return filePath.substr(lpos, rpos - lpos);
}
tstring StripQuotes(const tstring& str)
{
	auto lpos = str.find(FE_TEXT('"'), 0) + 1;
	auto rpos = str.rfind(FE_TEXT('"'), str.length() - 1);

	return str.substr(lpos, rpos - lpos);
}

bool FECopyFile(tifstream& orig, tofstream& dest)
{
	// 파일을 담을 문자열
	tstring s;
	// 파일의 크기
	DWORD size;
	
	if (!(orig.is_open() && dest.is_open())) return false;

	orig.seekg(0, tios::end);
	size = orig.tellg();
	s.resize(size);
	orig.seekg(0, tios::beg);
	orig.read(&s[0], size);

	dest << s;

	return true;
}


#ifdef _WIN32
#include <Windows.h>
#include <rpcdce.h>
#pragma comment(lib, "Rpcrt4.lib")
INT64 CreateUUIDHashCode64()
{
	UUID uuid;
	UuidCreate(&uuid);
	long long f = *((long long*)&uuid);
	long long b = *((long long*)uuid.Data4);

	return (f ^ b);
}
#elif
#endif