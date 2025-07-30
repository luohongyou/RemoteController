// agentprotect.cpp : Coral Remote Controller 后台操作响应程序自动启动程序

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#include <iostream>
#include <Windows.h>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

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
		{
			ReleaseMutex(hMutex);
			CloseHandle(hMutex);
		}

		if (!IsExt)
			ShellExecuteA(NULL, "open", "rcagent.exe", "", NULL, SW_HIDE);

		IsExt = 0;
		hMutex = CreateMutexA(NULL, FALSE, "__CoralRemoteControllerAuthManager");
		if (hMutex != NULL)
			if (ERROR_ALREADY_EXISTS == GetLastError())
				IsExt = 1;
			else
				IsExt = 0;
		else
			IsExt = Fast_CheckProcess("rcauth.exe");

		if (hMutex != NULL)
		{
			ReleaseMutex(hMutex);
			CloseHandle(hMutex);
		}

		if (!IsExt)
			ShellExecuteA(NULL, "open", "rcauth.exe", "", NULL, SW_HIDE);

		Sleep(10000);
	} while (IsProtect);

    return 0;
}