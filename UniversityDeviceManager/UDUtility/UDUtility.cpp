#include "stdafx.h"
#include "UDUtility.h"

CString PathManager::GetCurrentModelPath( HMODULE hModel /*= NULL*/ )
{
	TCHAR tPath[MAX_PATH] = {0};
	GetModuleFileName(hModel, tPath, MAX_PATH);
	CString csModelPath(tPath);
	csModelPath.Replace(_T("/"), _T("\\"));
	int nPos = csModelPath.ReverseFind(_T('\\'));
	if(nPos > 0)
	{
		csModelPath = csModelPath.Left(nPos+1);
	}
	return csModelPath;
}

BOOL PathManager::CopyDir(CString strSrcPath, CString strDstPath)
{ // ����Ŀ���ļ���
	if (strSrcPath.Right(1) == "\\")
	{
		strSrcPath = strSrcPath.Left(strSrcPath.GetLength() - 1);
	}
	if (strDstPath.Right(1) == "\\")
	{
		strDstPath = strDstPath.Left(strDstPath.GetLength() - 1);
	}
	CreateDirectory(strDstPath,NULL);
	CFileFind finder;
	// ��ָ�����ļ��н�������
	BOOL bWorking = finder.FindFile(strSrcPath + "\\" + "*.*"); 
	while(bWorking) 
	{
		// �ӵ�ǰĿ¼�����ļ�
		bWorking = finder.FindNextFile();
		CString strFileName = finder.GetFileName(); 
		CString strSrc = strSrcPath + "\\" + strFileName;
		CString strDst = strDstPath + "\\" + strFileName;
		// �ж����������ǲ���"."��".."Ŀ¼

		if(!finder.IsDots())

		{

			// �ж���������Ŀ¼�Ƿ����ļ���

			if(finder.IsDirectory())

			{

				// ������ļ��еĻ������еݹ�

				if(!CopyDir(strSrc, strDst)) 
				{
					return false;
				}
			}
			else
			{
				// ������ļ������и���
				if(!CopyFile(strSrc, strDst, FALSE))
				{
					return false;
				} 
			} 
		}
	}       
	return true;
} 

void PathManager::DeleteDirectory( CString strDir )
{
	if(strDir.IsEmpty())   
	{
		RemoveDirectory(strDir);  
		return;  
	}

	//   ����ɾ���ļ������ļ���  
	CFileFind   ff;  
	BOOL   bFound   =   ff.FindFile(strDir+_T("\\*"),   0);  
	while(bFound)  
	{  
		bFound   =   ff.FindNextFile();  
		if(ff.GetFileName()==_T(".")||ff.GetFileName()==_T(".."))  
			continue;  
		//   ȥ���ļ�(��)ֻ��������  
		SetFileAttributes(ff.GetFilePath(),   FILE_ATTRIBUTE_NORMAL);  
		if(ff.IsDirectory())  
		{  
			//   �ݹ�ɾ�����ļ���  
			DeleteDirectory(ff.GetFilePath());  
			RemoveDirectory(ff.GetFilePath());  
		}  
		else  
		{  
			//   ɾ���ļ�  
			DeleteFile(ff.GetFilePath());  
		}  
	}  
	ff.Close();  

	//   Ȼ��ɾ�����ļ���  
	RemoveDirectory(strDir);
}