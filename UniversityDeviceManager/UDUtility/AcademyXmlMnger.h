#pragma once
#include "HeaderData.h"


class UD_EXPORT_CLASS AcademyXmlMnger
{
public:
	AcademyXmlMnger(void);
	~AcademyXmlMnger(void);
	void SetXmlPath(CString csPath);

	bool GetAcademyList(CSNameVec& lstVec);
	CString GetAcademyNameByIndex(int index);
	bool InsertAcademyName(CString csName);
	bool DeleteAcademyName(CString csName);
	bool DeleteAcademyName(int index);
	bool ModifyAcademyName(CString csName);
	bool ModifyAcademyName(int index, CString csName);

private:
	CString m_csXmlPath;
};

