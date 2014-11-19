#pragma once

#include "HeaderData.h"

struct DeviceInfo
{
	CString csDevId;
	CString csSortName;
	CString csPrice;
	CString csEntryDate;
	CString csKeeper;
	CString csDevName;
	CString csImgPath;
	CString csImgPath2;
	CString csImgPath3;
	CString csDevType;
	CString csOriginPlace;
	CString csDevNowPlace;
	CString csTechNorm;
	CString csPhone;
	CString csEmail;
	CString csFactoryDate;
	CString csAccptanceDate;
	CString csScrapDate;
	CString csRemark;
	CString csUploadDataPath;
};

class UD_EXPORT_CLASS DeviceXmlMnger
{
public:
	DeviceXmlMnger(void);
	~DeviceXmlMnger(void);

	void SetXmlPath(CString csPath);
	bool InsertNewDevice(DeviceInfo info);
	bool GetDeviceById(CString csID, DeviceInfo& info);
	DeviceInfo GetDeviceByIndex(int index);
	int GetDeviceNum(DWORD* dwAllPrice = NULL);
	bool ModifyDevice(DeviceInfo info, int index);
	bool DeleteDevice(int index);

private:
	bool IsSameDevice(TiXmlNode* root, CString csID);

private:
	CString m_csPath;
};

