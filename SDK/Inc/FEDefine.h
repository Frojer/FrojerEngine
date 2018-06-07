#pragma once

#include <string>

#ifdef _UNICODE
typedef std::wstring tstring;
#elif
typedef std::string tstring;
#endif