#include "StdAfx.h"
#include "UniversityXmlMnger.h"


UniversityXmlMnger::UniversityXmlMnger(void) 
{
}


UniversityXmlMnger::~UniversityXmlMnger(void)
{
}

bool UniversityXmlMnger::GetUniversityList(CSNameVec& lstVec)
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

CString UniversityXmlMnger::GetUniversityNameByIndex( int index )
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

bool UniversityXmlMnger::InsertUniversityName( CString csName )
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
			AfxMessageBox(_T("�����ѧ�ظ���"));
			return false;
		}
		
	}
	return false;
}

bool UniversityXmlMnger::DeleteUniversityName( CString csName )
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

bool UniversityXmlMnger::DeleteUniversityName( int index )
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

bool UniversityXmlMnger::ModifyUniversityName( CString csName )
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

bool UniversityXmlMnger::ModifyUniversityName( int index, CString csName )
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

void UniversityXmlMnger::SetXmlPath( CString csPath )
{
	m_csXmlPath = csPath;
}

