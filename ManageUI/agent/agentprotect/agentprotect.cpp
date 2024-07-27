// agentprotect.cpp : Coral Remote Controller 后台操作响应程序自动启动程序

// 版权声明：
// Copyright (C) 2020-2024 Coral Studio. All Rights Reserved.

#include <iostream>
#include <Windows.h>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

int Fast_CheckProcess(std::string s);

int main()
{
	HANDLE _hMutex = NULL;
	_hMutex = CreateMutexA(NULL, FALSE, "__CoralRemoteAgentProtect");
	if (_hMutex != NULL)
		if (ERROR_ALREADY_EXISTS == GetLastError())
		{
			ReleaseMutex(_hMutex);
			return 0;
		}

    SetAppDirectory("\\..\\");

	int IsProtect = GetSettings("AgentProtect", true);

	HANDLE hMutex = NULL;
	bool IsExt;

	do
	{
		IsExt = 0;
		hMutex = CreateMutexA(NULL, FALSE, "__CoralRemoteAgent");
		if (hMutex != NULL)
			if (ERROR_ALREADY_EXISTS == GetLastError())
				IsExt = 1;
			else
				IsExt = 0;
		else
			IsExt = Fast_CheckProcess("rcagent.exe");

		if (hMutex != NULL)
			ReleaseMutex(hMutex);
		CloseHandle(hMutex);

		if (!IsExt)
			ShellExecuteA(NULL, "open", "rcagent.exe", "", NULL, SW_HIDE);

		Sleep(10000);
	} while (IsProtect);

    return 0;
}

int Fast_CheckProcess(std::string s)
{
	HANDLE hToken;
	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken);
	TOKEN_PRIVILEGES tp;
	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
	PROCESSENTRY32 pd;
	pd.dwSize = sizeof(pd);
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	BOOL choose = ::Process32First(hProcessSnap, &pd);
	while (choose)
	{
		char szEXE[100];
		_bstr_t b(pd.szExeFile);
		strcpy_s(szEXE, b);
		if (!strcmp(szEXE, s.c_str()))
			return true;
		choose = ::Process32Next(hProcessSnap, &pd);
	}
	return false;
}
