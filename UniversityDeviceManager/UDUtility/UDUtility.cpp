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
{ // 创建目标文件夹
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
	// 打开指定的文件夹进行搜索
	BOOL bWorking = finder.FindFile(strSrcPath + "\\" + "*.*"); 
	while(bWorking) 
	{
		// 从当前目录搜索文件
		bWorking = finder.FindNextFile();
		CString strFileName = finder.GetFileName(); 
		CString strSrc = strSrcPath + "\\" + strFileName;
		CString strDst = strDstPath + "\\" + strFileName;
		// 判断搜索到的是不是"."和".."目录

		if(!finder.IsDots())

		{

			// 判断搜索到的目录是否是文件夹

			if(finder.IsDirectory())

			{

				// 如果是文件夹的话，进行递归

				if(!CopyDir(strSrc, strDst)) 
				{
					return false;
				}
			}
			else
			{
				// 如果是文件，进行复制
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

	//   首先删除文件及子文件夹  
	CFileFind   ff;  
	BOOL   bFound   =   ff.FindFile(strDir+_T("\\*"),   0);  
	while(bFound)  
	{  
		bFound   =   ff.FindNextFile();  
		if(ff.GetFileName()==_T(".")||ff.GetFileName()==_T(".."))  
			continue;  
		//   去掉文件(夹)只读等属性  
		SetFileAttributes(ff.GetFilePath(),   FILE_ATTRIBUTE_NORMAL);  
		if(ff.IsDirectory())  
		{  
			//   递归删除子文件夹  
			DeleteDirectory(ff.GetFilePath());  
			RemoveDirectory(ff.GetFilePath());  
		}  
		else  
		{  
			//   删除文件  
			DeleteFile(ff.GetFilePath());  
		}  
	}  
	ff.Close();  

	//   然后删除该文件夹  
	RemoveDirectory(strDir);
}