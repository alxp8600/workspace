#include "StdAfx.h"
#include "DeviceXmlMnger.h"

#define UD_DEV_ID "id"
#define UD_DEV_SORT "sort"
#define UD_DEV_PRICE "price"
#define UD_DEV_ENTRY_DATE "entryprice"
#define UD_DEV_KEEPER "keeper"
#define UD_DEV_NAME "devName"
#define UD_IMG_PATH "imgpath"
#define UD_IMG_PATH2 "imgpath2"
#define UD_IMG_PATH3 "imgpath3"
#define UD_DEV_TYPE "devType"
#define UD_DEV_ORIGIN_PLACE "originPlace"
#define UD_DEV_NOW_PLACE "nowPlace"
#define UD_DEV_TECH_NORM "techNorm"
#define UD_DEV_PHONE "phone"
#define UD_DEV_EMAIL "email"
#define UD_DEV_FACTORY_DATE "dateFactory"
#define UD_DEV_ACCPTANCE_DATE "dateAccptance"
#define UD_DEV_SCRAP_DATE "dateScrap"
#define UD_DEV_REMARK "remark"
#define UD_UPLOAD_PATH "uploadPath"

DeviceXmlMnger::DeviceXmlMnger(void)
{
}


DeviceXmlMnger::~DeviceXmlMnger(void)
{
}

void DeviceXmlMnger::SetXmlPath( CString csPath )
{
	m_csPath = csPath;
}

bool DeviceXmlMnger::InsertNewDevice( DeviceInfo info )
{
	USES_CONVERSION;
	TiXmlDocument doc;
	TiXmlDeclaration dec("1.0","gb2312","");
	TiXmlElement ele("Dev");
	TiXmlElement root("Root");


	bool bOk;
	if(! (bOk = doc.LoadFile(T2A(m_csPath))))
	{
		doc.InsertEndChild(dec);
		doc.InsertEndChild(root);
	}
	TiXmlNode* rnode = doc.RootElement();

	if(rnode)
	{
		if(!IsSameDevice(rnode, info.csDevId))
		{
			ele.SetAttribute(UD_DEV_ID, T2A(info.csDevId));
			ele.SetAttribute(UD_DEV_NAME, T2A(info.csDevName));
			ele.SetAttribute(UD_DEV_SORT, T2A(info.csSortName));
			ele.SetAttribute(UD_DEV_PRICE, T2A(info.csPrice));
			ele.SetAttribute(UD_DEV_ENTRY_DATE, T2A(info.csEntryDate));
			ele.SetAttribute(UD_DEV_KEEPER, T2A(info.csKeeper));
			ele.SetAttribute(UD_IMG_PATH, T2A(info.csImgPath));
			ele.SetAttribute(UD_IMG_PATH2, T2A(info.csImgPath2));
			ele.SetAttribute(UD_IMG_PATH3, T2A(info.csImgPath3));
			ele.SetAttribute(UD_DEV_TYPE, T2A(info.csDevType));
			ele.SetAttribute(UD_DEV_ORIGIN_PLACE, T2A(info.csOriginPlace));
			ele.SetAttribute(UD_DEV_NOW_PLACE, T2A(info.csDevNowPlace));
			ele.SetAttribute(UD_DEV_TECH_NORM, T2A(info.csTechNorm));
			ele.SetAttribute(UD_DEV_PHONE, T2A(info.csPhone));
			ele.SetAttribute(UD_DEV_EMAIL, T2A(info.csEmail));
			ele.SetAttribute(UD_DEV_FACTORY_DATE, T2A(info.csFactoryDate));
			ele.SetAttribute(UD_DEV_ACCPTANCE_DATE, T2A(info.csAccptanceDate));
			ele.SetAttribute(UD_DEV_SCRAP_DATE, T2A(info.csScrapDate));
			ele.SetAttribute(UD_DEV_REMARK, T2A(info.csRemark));
			ele.SetAttribute(UD_UPLOAD_PATH, T2A(info.csUploadDataPath));
			rnode->InsertEndChild(ele);
			if(bOk)
				doc.SaveFile();
			else
				doc.SaveFile(T2A(m_csPath));
			return true;
		}
		else
		{
			AfxMessageBox(_T("插入设备重复！"));
			return false;
		}

	}
	return false;
}

bool DeviceXmlMnger::GetDeviceById(CString csID, DeviceInfo& info)
{
	USES_CONVERSION;
	TiXmlDocument doc;
	if(doc.LoadFile(T2A(m_csPath)))
	{
		TiXmlNode* root = doc.RootElement();
		if(root)
		{
			TiXmlNode* node = NULL;
			for( node = root->FirstChild();
				node;
				node = node->NextSibling() )
			{
				CString arrId = A2T(node->ToElement()->Attribute(UD_DEV_ID));
				if(csID == arrId)
				{
					info.csDevId = csID;
					info.csSortName = A2T(node->ToElement()->Attribute(UD_DEV_SORT));
					info.csPrice =  A2T(node->ToElement()->Attribute(UD_DEV_PRICE));
					info.csKeeper = A2T(node->ToElement()->Attribute(UD_DEV_KEEPER));
					info.csEntryDate = A2T(node->ToElement()->Attribute(UD_DEV_ENTRY_DATE));
					info.csDevName = A2T(node->ToElement()->Attribute(UD_DEV_NAME));
					info.csImgPath = A2T(node->ToElement()->Attribute(UD_IMG_PATH));
					info.csImgPath2 = A2T(node->ToElement()->Attribute(UD_IMG_PATH2));
					info.csImgPath3 = A2T(node->ToElement()->Attribute(UD_IMG_PATH3));
					info.csDevType = A2T(node->ToElement()->Attribute(UD_DEV_TYPE));
					info.csOriginPlace = A2T(node->ToElement()->Attribute(UD_DEV_ORIGIN_PLACE));
					info.csDevNowPlace = A2T(node->ToElement()->Attribute(UD_DEV_NOW_PLACE));
					info.csTechNorm = A2T(node->ToElement()->Attribute(UD_DEV_TECH_NORM));
					info.csPhone = A2T(node->ToElement()->Attribute(UD_DEV_PHONE));
					info.csEmail = A2T(node->ToElement()->Attribute(UD_DEV_EMAIL));
					info.csFactoryDate = A2T(node->ToElement()->Attribute(UD_DEV_FACTORY_DATE));
					info.csAccptanceDate = A2T(node->ToElement()->Attribute(UD_DEV_ACCPTANCE_DATE));
					info.csScrapDate = A2T(node->ToElement()->Attribute(UD_DEV_SCRAP_DATE));
					info.csRemark = A2T(node->ToElement()->Attribute(UD_DEV_REMARK));
					info.csUploadDataPath = A2T(node->ToElement()->Attribute(UD_UPLOAD_PATH));
					return true;
				}
			}
		}
	}
	return false;
}

DeviceInfo DeviceXmlMnger::GetDeviceByIndex( int index )
{
	USES_CONVERSION;
	int count = 0;
	TiXmlDocument doc;
	if(doc.LoadFile(T2A(m_csPath)))
	{
		TiXmlNode* root = doc.RootElement();
		if(root)
		{
			TiXmlNode* node = NULL;
			for( node = root->FirstChild();
				node;
				node = node->NextSibling() )
			{
				if(index == count)
				{
					DeviceInfo info;
					info.csDevId = A2T(node->ToElement()->Attribute(UD_DEV_ID));
					info.csSortName = A2T(node->ToElement()->Attribute(UD_DEV_SORT));
					info.csPrice =  A2T(node->ToElement()->Attribute(UD_DEV_PRICE));
					info.csKeeper = A2T(node->ToElement()->Attribute(UD_DEV_KEEPER));
					info.csEntryDate = A2T(node->ToElement()->Attribute(UD_DEV_ENTRY_DATE));
					info.csDevName = A2T(node->ToElement()->Attribute(UD_DEV_NAME));
					info.csImgPath = A2T(node->ToElement()->Attribute(UD_IMG_PATH));
					info.csImgPath2 = A2T(node->ToElement()->Attribute(UD_IMG_PATH2));
					info.csImgPath3 = A2T(node->ToElement()->Attribute(UD_IMG_PATH3));
					info.csDevType = A2T(node->ToElement()->Attribute(UD_DEV_TYPE));
					info.csOriginPlace = A2T(node->ToElement()->Attribute(UD_DEV_ORIGIN_PLACE));
					info.csDevNowPlace = A2T(node->ToElement()->Attribute(UD_DEV_NOW_PLACE));
					info.csTechNorm = A2T(node->ToElement()->Attribute(UD_DEV_TECH_NORM));
					info.csPhone = A2T(node->ToElement()->Attribute(UD_DEV_PHONE));
					info.csEmail = A2T(node->ToElement()->Attribute(UD_DEV_EMAIL));
					info.csFactoryDate = A2T(node->ToElement()->Attribute(UD_DEV_FACTORY_DATE));
					info.csAccptanceDate = A2T(node->ToElement()->Attribute(UD_DEV_ACCPTANCE_DATE));
					info.csScrapDate = A2T(node->ToElement()->Attribute(UD_DEV_SCRAP_DATE));
					info.csRemark = A2T(node->ToElement()->Attribute(UD_DEV_REMARK));
					info.csUploadDataPath = A2T(node->ToElement()->Attribute(UD_UPLOAD_PATH));
					return info;
				}
				count++;
			}
		}
	}
	return DeviceInfo();
}

int DeviceXmlMnger::GetDeviceNum(DWORD* dwAllPrice)
{
	USES_CONVERSION;
	int count = 0;
	TiXmlDocument doc;
	if(doc.LoadFile(T2A(m_csPath)))
	{
		TiXmlNode* root = doc.RootElement();
		if(root)
		{
			if(dwAllPrice)
			{
				*dwAllPrice = 0;
			}
			TiXmlNode* node = NULL;
			for( node = root->FirstChild();
				node;
				node = node->NextSibling() )
			{
				if(dwAllPrice)
				{
					int nPrice = atoi(node->ToElement()->Attribute("price"));
					*dwAllPrice += nPrice;
				}
				count++;
			}
		}
	}
	
	return count;
}

bool DeviceXmlMnger::IsSameDevice( TiXmlNode* root, CString csID )
{
	if(root)
	{
		USES_CONVERSION;
		TiXmlNode* node = NULL;
		for( node = root->FirstChild();
			node;
			node = node->NextSibling() )
		{
			CString csNodeName(A2T(node->ToElement()->FirstAttribute()->Value()));
			if(csID == csNodeName)
			{
				return true;
			}
		}
	}
	return false;
}

bool DeviceXmlMnger::ModifyDevice( DeviceInfo info, int index)
{
	USES_CONVERSION;
	int count = 0;
	TiXmlDocument doc;
	if(doc.LoadFile(T2A(m_csPath)))
	{
		TiXmlNode* root = doc.RootElement();
		if(root)
		{
			TiXmlNode* node = NULL;
			for( node = root->FirstChild();
				node;
				node = node->NextSibling() )
			{
				if(index == count)
				{
					node->ToElement()->SetAttribute(UD_DEV_ID, T2A(info.csDevId));
					node->ToElement()->SetAttribute(UD_DEV_NAME, T2A(info.csDevName));
					node->ToElement()->SetAttribute(UD_DEV_SORT, T2A(info.csSortName));
					node->ToElement()->SetAttribute(UD_DEV_PRICE, T2A(info.csPrice));
					node->ToElement()->SetAttribute(UD_DEV_ENTRY_DATE, T2A(info.csEntryDate));
					node->ToElement()->SetAttribute(UD_DEV_KEEPER, T2A(info.csKeeper));
					node->ToElement()->SetAttribute(UD_IMG_PATH, T2A(info.csImgPath));
					node->ToElement()->SetAttribute(UD_IMG_PATH2, T2A(info.csImgPath2));
					node->ToElement()->SetAttribute(UD_IMG_PATH3, T2A(info.csImgPath3));
					node->ToElement()->SetAttribute(UD_DEV_TYPE, T2A(info.csDevType));
					node->ToElement()->SetAttribute(UD_DEV_ORIGIN_PLACE, T2A(info.csOriginPlace));
					node->ToElement()->SetAttribute(UD_DEV_NOW_PLACE, T2A(info.csDevNowPlace));
					node->ToElement()->SetAttribute(UD_DEV_TECH_NORM, T2A(info.csTechNorm));
					node->ToElement()->SetAttribute(UD_DEV_PHONE, T2A(info.csPhone));
					node->ToElement()->SetAttribute(UD_DEV_EMAIL, T2A(info.csEmail));
					node->ToElement()->SetAttribute(UD_DEV_FACTORY_DATE, T2A(info.csFactoryDate));
					node->ToElement()->SetAttribute(UD_DEV_ACCPTANCE_DATE, T2A(info.csAccptanceDate));
					node->ToElement()->SetAttribute(UD_DEV_SCRAP_DATE, T2A(info.csScrapDate));
					node->ToElement()->SetAttribute(UD_DEV_REMARK, T2A(info.csRemark));
					node->ToElement()->SetAttribute(UD_UPLOAD_PATH, T2A(info.csUploadDataPath));
					doc.SaveFile();
					return true;
				}
				count++;
			}
		}
	}
	return false;
}

bool DeviceXmlMnger::DeleteDevice( int index )
{
	USES_CONVERSION;
	int count = 0;
	TiXmlDocument doc;
	if(doc.LoadFile(T2A(m_csPath)))
	{
		TiXmlNode* root = doc.RootElement();
		if(root)
		{
			TiXmlNode* node = NULL;
			for( node = root->FirstChild();
				node;
				node = node->NextSibling() )
			{
				if(count == index)
				{
					root->RemoveChild(node);
					doc.SaveFile();
					return true;
				}
				count++;
			}
		}
	}
	return false;
}