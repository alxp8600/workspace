#pragma once

#include "UniversityXmlMnger.h"
#include "AcademyXmlMnger.h"
#include "DeviceXmlMnger.h"
#include "BmpTextStatic.h"
#include "CustomListCtrl.h"

class UD_EXPORT_CLASS PathManager
{
public:
	static CString GetCurrentModelPath(HMODULE hModel = NULL);
	static BOOL CopyDir(CString strSrcPath, CString strDstPath);
	static void DeleteDirectory( CString strDir );
};