#pragma once

#include <vector>

#include "../TinyXml/tinyxml.h"

typedef std::vector<CString> CSNameVec;


#ifdef WIN32
#pragma comment(lib, "../lib/win32/TinyXml.lib")
#else
#pragma comment(lib, "../lib/win64/TinyXml.lib")
#endif

#ifdef UD_EXPORT_MACRO
#define UD_EXPORT_CLASS __declspec(dllexport)
#else
#define UD_EXPORT_CLASS __declspec(dllimport)
#endif