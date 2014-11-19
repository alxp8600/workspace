// DevInfoEdtDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UniversityDeviceEdit.h"
#include "DevInfoEdtDlg.h"
#include "afxdialogex.h"


// CDevInfoEdtDlg 对话框

IMPLEMENT_DYNAMIC(CDevInfoEdtDlg, CDialog)

CDevInfoEdtDlg::CDevInfoEdtDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDevInfoEdtDlg::IDD, pParent)
	, m_nDevID(0)
	, m_csSortName(_T(""))
	, m_nPrice(0)
	, m_csKeeper(_T(""))
	, m_csDevName(_T(""))
	, m_csImgPath(_T(""))
	, m_csImgPath2(_T(""))
	, m_csImgPath3(_T(""))
	, m_bDlgFlag(TRUE)
	, m_csDevType(_T(""))
	, m_csOriginPlace(_T(""))
	, m_csNowPlace(_T(""))
	, m_csDevTechNorm(_T(""))
	, m_csPhone(_T(""))
	, m_csEmail(_T(""))
	, m_csRemark(_T(""))
	, m_csAccptanceDate(_T(""))
	, m_csScrapDate(_T(""))
	, m_csUploadDataPath(_T(""))
{

}

CDevInfoEdtDlg::~CDevInfoEdtDlg()
{
}

void CDevInfoEdtDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DEV_ID, m_nDevID);
	DDX_Text(pDX, IDC_EDIT_DEV_SORT, m_csSortName);
	DDX_Text(pDX, IDC_EDIT_DEV_PRICE, m_nPrice);
	DDX_Control(pDX, IDC_DATETIMEPICKER_DEV_DATE, m_dtcDate);
	DDX_Text(pDX, IDC_EDIT_DEV_KEEPER, m_csKeeper);
	DDX_Text(pDX, IDC_EDIT_DEV_NAME, m_csDevName);
	DDX_Text(pDX, IDC_EDIT_DEV_TYPE, m_csDevType);
	DDX_Text(pDX, IDC_EDIT_DEV_ORIGIN_PLACE, m_csOriginPlace);
	DDX_Text(pDX, IDC_EDIT_DEV_NOW_PLACE, m_csNowPlace);
	DDX_Text(pDX, IDC_EDIT_DEV_TECH_NORM, m_csDevTechNorm);
	DDX_Text(pDX, IDC_EDIT_DEV_PHONE, m_csPhone);
	DDX_Text(pDX, IDC_EDIT_DEV_EMAIL, m_csEmail);
	DDX_Control(pDX, IDC_DATETIMEPICKER_DEV_DATE_FACTORY, m_dtcFactoryDate);
	DDX_Control(pDX, IDC_DATETIMEPICKER_DEV_DATE_ACCEPTANCE, m_dtcAccptanceDate);
	DDX_Control(pDX, IDC_DATETIMEPICKER_DEV_DATE_SCRAP, m_dtcScrapDate);
	DDX_Text(pDX, IDC_EDIT_DEV_REMARK, m_csRemark);
}


BEGIN_MESSAGE_MAP(CDevInfoEdtDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_DEV_IMG_BROWSE, &CDevInfoEdtDlg::OnBnClickedBtnDevImgBrowse)
	ON_BN_CLICKED(IDOK, &CDevInfoEdtDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDevInfoEdtDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_DEV_IMG_BROWSE2, &CDevInfoEdtDlg::OnBnClickedBtnDevImgBrowse2)
	ON_BN_CLICKED(IDC_BTN_DEV_IMG_BROWSE3, &CDevInfoEdtDlg::OnBnClickedBtnDevImgBrowse3)
	ON_BN_CLICKED(IDC_BTN_UPLOAD_DATA, &CDevInfoEdtDlg::OnBnClickedBtnUploadData)
	ON_BN_CLICKED(IDC_BTN_DELETE_DATA, &CDevInfoEdtDlg::OnBnClickedBtnDeleteData)
END_MESSAGE_MAP()


BOOL CDevInfoEdtDlg::OnInitDialog()
{
	BOOL bRet = CDialog::OnInitDialog();
	m_dtcDate.SetFormat(_T("yyyy-MM-dd"));

	m_stcImgShow.SubclassWindow(GetDlgItem(IDC_PIC_DEV)->m_hWnd);
	m_stcImgShow2.SubclassWindow(GetDlgItem(IDC_PIC_DEV2)->m_hWnd);
	m_stcImgShow3.SubclassWindow(GetDlgItem(IDC_PIC_DEV3)->m_hWnd);
	CRect rc;
	m_stcImgShow.GetWindowRect(rc);
	rc.right = rc.left + 160;
	rc.bottom = rc.top + 120;
	ScreenToClient(rc);
	m_stcImgShow.MoveWindow(rc);

	m_stcImgShow2.GetWindowRect(rc);
	rc.right = rc.left + 160;
	rc.bottom = rc.top + 120;
	ScreenToClient(rc);
	m_stcImgShow2.MoveWindow(rc);

	m_stcImgShow3.GetWindowRect(rc);
	rc.right = rc.left + 160;
	rc.bottom = rc.top + 120;
	ScreenToClient(rc);
	m_stcImgShow3.MoveWindow(rc);

	if(!m_bDlgFlag)
	{
		COleDateTime time;
		if(!m_csDate.IsEmpty())
		{
			time.ParseDateTime(m_csDate);
			m_dtcDate.SetTime(time);
		}
		
		if(!m_csFactoryDate.IsEmpty())
		{
			time.ParseDateTime(m_csFactoryDate);
			m_dtcFactoryDate.SetTime(time);
		}
		
		if(!m_csAccptanceDate.IsEmpty())
		{
			time.ParseDateTime(m_csAccptanceDate);
			m_dtcAccptanceDate.SetTime(time);
		}
		
		if(!m_csScrapDate.IsEmpty())
		{
			time.ParseDateTime(m_csScrapDate);
			m_dtcScrapDate.SetTime(time);
		}
		

		CString str = PathManager::GetCurrentModelPath();
		CString str1 = str + m_csImgPath;
		m_stcImgShow.SetStaticBitmap(str1);
		str1 = str + m_csImgPath2;
		m_stcImgShow2.SetStaticBitmap(str1);
		str1 = str + m_csImgPath3;
		m_stcImgShow3.SetStaticBitmap(str1);

		UpdateData(FALSE);
	}

	return bRet;
}
// CDevInfoEdtDlg 消息处理程序


void CDevInfoEdtDlg::OnBnClickedBtnDevImgBrowse()
{
	UpdateData(TRUE);
	CFileDialog  dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("png文件(*.png)|*.png|jpg文件(*.jpg;*.jpeg)|*.jpg;*.jpeg|bmp文件(*.bmp)|*.bmp|所有图像文件(*.*)|*.*||"));
	if(dlg.DoModal() == IDOK)
	{
		CString strOld =dlg.GetPathName();
		CString csSuffix = strOld.Right(strOld.GetLength() - strOld.ReverseFind(_T('.')) - 1);
		CString csName;
		csName.Format(_T("%d"), m_nDevID);
		csName += _T(".");
		csName += csSuffix;
		CString fullPath = PathManager::GetCurrentModelPath() + m_csDevPath + csName;
		if(PathFileExists(fullPath))
		{
			if(IDNO == MessageBox(_T("图片已经存在，是否替换？"), _T("警告"), MB_YESNO))
			{
				return;
			}
			else
			{
				m_stcImgShow.CloseStaticBitmap();
				if(!DeleteFile(fullPath))
				{
				}	
			}
		}

		if(CopyFile(strOld, fullPath, TRUE))
		{
			m_stcImgShow.SetStaticBitmap(fullPath);
			m_stcImgShow.Invalidate();
		}
		m_csImgPath = m_csDevPath + csName;
		
	}
}

void CDevInfoEdtDlg::OnBnClickedBtnDevImgBrowse2()
{
	UpdateData(TRUE);
	CFileDialog  dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("png文件(*.png)|*.png|jpg文件(*.jpg;*.jpeg)|*.jpg;*.jpeg|bmp文件(*.bmp)|*.bmp|所有图像文件(*.*)|*.*||"));
	if(dlg.DoModal() == IDOK)
	{
		CString strOld =dlg.GetPathName();
		CString csSuffix = strOld.Right(strOld.GetLength() - strOld.ReverseFind(_T('.')) - 1);
		CString csName;
		csName.Format(_T("%d_2"), m_nDevID);
		csName += _T(".");
		csName += csSuffix;
		CString fullPath = PathManager::GetCurrentModelPath() + m_csDevPath + csName;
		if(PathFileExists(fullPath))
		{
			if(IDNO == MessageBox(_T("图片已经存在，是否替换？"), _T("警告"), MB_YESNO))
			{
				return;
			}
			else
			{
				m_stcImgShow2.CloseStaticBitmap();
				DeleteFile(fullPath);	
			}
		}

		if(CopyFile(strOld, fullPath, TRUE))
		{
			m_stcImgShow2.SetStaticBitmap(fullPath);
			m_stcImgShow2.Invalidate();
		}
		m_csImgPath2 = m_csDevPath + csName;

	}
}


void CDevInfoEdtDlg::OnBnClickedBtnDevImgBrowse3()
{
	UpdateData(TRUE);
	CFileDialog  dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("png文件(*.png)|*.png|jpg文件(*.jpg;*.jpeg)|*.jpg;*.jpeg|bmp文件(*.bmp)|*.bmp|所有图像文件(*.*)|*.*||"));
	if(dlg.DoModal() == IDOK)
	{
		CString strOld =dlg.GetPathName();
		CString csSuffix = strOld.Right(strOld.GetLength() - strOld.ReverseFind(_T('.')) - 1);
		CString csName;
		csName.Format(_T("%d_3"), m_nDevID);
		csName += _T(".");
		csName += csSuffix;
		CString fullPath = PathManager::GetCurrentModelPath() + m_csDevPath + csName;
		if(PathFileExists(fullPath))
		{
			if(IDNO == MessageBox(_T("图片已经存在，是否替换？"), _T("警告"), MB_YESNO))
			{
				return;
			}
			else
			{
				m_stcImgShow3.CloseStaticBitmap();
				DeleteFile(fullPath);	
			}
		}

		if(CopyFile(strOld, fullPath, TRUE))
		{
			m_stcImgShow3.SetStaticBitmap(fullPath);
			m_stcImgShow3.Invalidate();
		}
		m_csImgPath3 = m_csDevPath + csName;

	}
}

void CDevInfoEdtDlg::OnBnClickedBtnUploadData()
{
	UpdateData(TRUE);
	CFileDialog  dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("所有文件(*.*)|*.*||"));
	if(dlg.DoModal() == IDOK)
	{
		CString strOld =dlg.GetPathName();
		CString csSuffix = strOld.Right(strOld.GetLength() - strOld.ReverseFind(_T('.')) - 1);
		CString csName;
		csName.Format(_T("%d_data"), m_nDevID);
		csName += _T(".");
		csName += csSuffix;
		CString fullPath = PathManager::GetCurrentModelPath() + m_csDevPath + csName;
		if(PathFileExists(fullPath))
		{
			if(IDNO == MessageBox(_T("资料以及存在，是否替换？"), _T("警告"), MB_YESNO))
			{
				return;
			}
			else
			{
				DeleteFile(fullPath);	
			}
		}

		if(CopyFile(strOld, fullPath, TRUE))
		{
			MessageBox(_T("上传成功！"));
		}
		m_csUploadDataPath = m_csDevPath + csName;

	}
}


void CDevInfoEdtDlg::OnBnClickedBtnDeleteData()
{
	CString fullPath = PathManager::GetCurrentModelPath() + m_csUploadDataPath;
	if(DeleteFile(fullPath))
		MessageBox(_T("删除成功！"));
	else
		MessageBox(_T("删除失败！"));
}

void CDevInfoEdtDlg::OnBnClickedOk()
{
	UpdateData();
	CTime time;
	DWORD dwResult = m_dtcDate.GetTime(time);
	if (dwResult == GDT_VALID)
	{
		m_csDate = time.Format(TEXT("%Y-%m-%d"));
	}
	dwResult = m_dtcFactoryDate.GetTime(time);
	if (dwResult == GDT_VALID)
	{
		m_csFactoryDate = time.Format(TEXT("%Y-%m-%d"));
	}
	dwResult = m_dtcAccptanceDate.GetTime(time);
	if (dwResult == GDT_VALID)
	{
		m_csAccptanceDate = time.Format(TEXT("%Y-%m-%d"));
	}
	dwResult = m_dtcScrapDate.GetTime(time);
	if (dwResult == GDT_VALID)
	{
		m_csScrapDate = time.Format(TEXT("%Y-%m-%d"));
	}

	CDialog::OnOK();
}

DeviceInfo CDevInfoEdtDlg::GetDeviceInfo()
{
	DeviceInfo info;
	info.csDevId.Format(_T("%d"), m_nDevID);
	info.csSortName = m_csSortName;
	info.csPrice.Format(_T("%d"), m_nPrice);
	info.csEntryDate = m_csDate;
	info.csKeeper = m_csKeeper;
	info.csDevName = m_csDevName;
	info.csImgPath = m_csImgPath;
	info.csImgPath2 = m_csImgPath2;
	info.csImgPath3 = m_csImgPath3;
	info.csAccptanceDate = m_csAccptanceDate;
	info.csScrapDate = m_csScrapDate;
	info.csDevNowPlace = m_csNowPlace;
	info.csDevType = m_csDevType;
	info.csEmail = m_csEmail;
	info.csFactoryDate = m_csFactoryDate;
	info.csOriginPlace = m_csOriginPlace;
	info.csPhone = m_csPhone;
	info.csRemark = m_csRemark;
	info.csTechNorm = m_csDevTechNorm;
	info.csUploadDataPath = m_csUploadDataPath;
	return info;
}


void CDevInfoEdtDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}

void CDevInfoEdtDlg::SetDeviceInfo( DeviceInfo info )
{
	m_nDevID = _ttoi(info.csDevId);
	m_csSortName = info.csSortName;
	m_csDevName = info.csDevName;
	m_csKeeper = info.csKeeper;
	m_nPrice = _ttoi(info.csPrice);
	m_csDate = info.csEntryDate;
	m_csImgPath = info.csImgPath;
	m_csImgPath2 = info.csImgPath2;
	m_csImgPath3 = info.csImgPath3;
	m_csAccptanceDate = info.csAccptanceDate;
	m_csDevTechNorm = info.csTechNorm;
	m_csDevType = info.csDevType;
	m_csEmail = info.csEmail;
	m_csFactoryDate = info.csFactoryDate;
	m_csOriginPlace = info.csOriginPlace;
	m_csPhone = info.csPhone;
	m_csRemark = info.csRemark;
	m_csScrapDate = info.csScrapDate;
	m_csNowPlace = info.csDevNowPlace;
	m_csUploadDataPath = info.csUploadDataPath;
}

void CDevInfoEdtDlg::SetCurrentDevPath( CString csPath )
{
	m_csDevPath = csPath;
}

void CDevInfoEdtDlg::SetAddOrModifyType( BOOL bFlag /*= TRUE*/ )
{
	m_bDlgFlag = bFlag;
}


