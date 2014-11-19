#include "StdAfx.h"
#include "AcademyXmlMnger.h"


AcademyXmlMnger::AcademyXmlMnger(void)
{
}


AcademyXmlMnger::~AcademyXmlMnger(void)
{
}

bool AcademyXmlMnger::GetAcademyList(CSNameVec& lstVec)
{
	USES_CONVERSION;
	TiXmlDocument doc;
	if(doc.LoadFile(T2A(m_csXmlPath)))
	{
		TiXmlNode* rnode = doc.RootElement();
		if(rnode)
		{
			TiXmlNode* node = NULL;
			USES_CONVERSION;
			lstVec.clear();
			for( node = rnode->FirstChild();
				node;
				node = node->NextSibling() )
			{
				CString csName(A2T(node->ToElement()->Value()));
				lstVec.push_back(csName);
			}
			return true;
		}
	}
	return false;
}

CString AcademyXmlMnger::GetAcademyNameByIndex( int index )
{
	USES_CONVERSION;
	TiXmlDocument doc;
	if(doc.LoadFile(T2A(m_csXmlPath)))
	{
		TiXmlNode* rnode = doc.RootElement();

		if(rnode)
		{
			TiXmlNode* node = NULL;
			int nCurIndex = 0;
			for( node = rnode->FirstChild();
				node;
				node = node->NextSibling() )
			{
				if(nCurIndex == index)
				{
					return A2T(node->ToElement()->Value());
				}
				nCurIndex++;
			}
		}
	}
	return _T("");
}

bool AcademyXmlMnger::InsertAcademyName( CString csName )
{
	USES_CONVERSION;
	TiXmlDocument doc;
	TiXmlDeclaration dec("1.0","gb2312","");
	TiXmlElement ele(T2A(csName));
	TiXmlElement root("Root");
	bool bOk;
	if(! (bOk = doc.LoadFile(T2A(m_csXmlPath))))
	{
		doc.InsertEndChild(dec);
		doc.InsertEndChild(root);
	}
	TiXmlNode* rnode = doc.RootElement();

	if(rnode)
	{
		if(!rnode->FirstChild(T2A(csName)))
		{
			rnode->InsertEndChild(ele);
			if(bOk)
				doc.SaveFile();
			else
				doc.SaveFile(T2A(m_csXmlPath));
			return true;
		}
		else
		{
			AfxMessageBox(_T("插入学院重复！"));
			return false;
		}

	}
	return false;
}

bool AcademyXmlMnger::DeleteAcademyName( CString csName )
{
	USES_CONVERSION;
	TiXmlDocument doc;
	if(doc.LoadFile(T2A(m_csXmlPath)))
	{
		TiXmlNode* rnode = doc.RootElement();

		if(rnode)
		{
			TiXmlNode* node = NULL;
			for( node = rnode->FirstChild();
				node;
				node = node->NextSibling() )
			{
				CString csNodeName(A2T(node->ToElement()->Value()));
				if(csName == csNodeName)
				{
					rnode->RemoveChild(node);
					doc.SaveFile();
					return true;
				}
			}
		}
	}
	return false;
}

bool AcademyXmlMnger::DeleteAcademyName( int index )
{
	USES_CONVERSION;
	TiXmlDocument doc;
	if(doc.LoadFile(T2A(m_csXmlPath)))
	{
		TiXmlNode* rnode = doc.RootElement();

		if(rnode)
		{
			TiXmlNode* node = NULL;
			int curIndex = 0;
			for( node = rnode->FirstChild();
				node;
				node = node->NextSibling() )
			{
				if(curIndex == index)
				{
					rnode->RemoveChild(node);
					doc.SaveFile();
					return true;
				}
				curIndex++;
			}
		}
	}
	return false;
}

bool AcademyXmlMnger::ModifyAcademyName( CString csName )
{
	USES_CONVERSION;
	TiXmlDocument doc;
	if(doc.LoadFile(T2A(m_csXmlPath)))
	{
		TiXmlNode* rnode = doc.RootElement();

		if(rnode)
		{
			TiXmlNode* node = NULL;
			for( node = rnode->FirstChild();
				node;
				node = node->NextSibling() )
			{
				CString csNodeName(A2T(node->ToElement()->Value()));
				if(csName == csNodeName)
				{
					node->ToElement()->SetValue(T2A(csName));
					doc.SaveFile();
					return true;
				}
			}
		}
	}
	return false;
}

bool AcademyXmlMnger::ModifyAcademyName( int index, CString csName )
{
	USES_CONVERSION;
	TiXmlDocument doc;
	if(doc.LoadFile(T2A(m_csXmlPath)))
	{
		TiXmlNode* rnode = doc.RootElement();

		if(rnode)
		{
			TiXmlNode* node = NULL;
			int curIndex = 0;
			for( node = rnode->FirstChild();
				node;
				node = node->NextSibling() )
			{
				if(curIndex == index)
				{
					node->ToElement()->SetValue(T2A(csName));
					doc.SaveFile();
					return true;
				}
				curIndex++;
			}
		}
	}
	return false;
}

void AcademyXmlMnger::SetXmlPath( CString csPath )
{
	m_csXmlPath = csPath;
}