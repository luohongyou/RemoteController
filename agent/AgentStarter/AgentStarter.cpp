// AgentStarter.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <comdef.h>
using namespace std;
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
int CheckProcess(string s);
int main()
{
	char strModule[256];
	GetModuleFileNameA(NULL, strModule, 256);
	std::string a;
	a.assign(strModule);
	a.append("\\..\\");
	SetCurrentDirectoryA(a.c_str());

    if (!CheckProcess("CoralRemoteAgent.exe"))
		ShellExecuteA(NULL, "open", "CoralRemoteAgent.exe", "", NULL, SW_HIDE);
	return 0;
}

int CheckProcess(string s)
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
	int stdexe = 0;
	while (choose)
	{
		char szEXE[100];
		_bstr_t b(pd.szExeFile);
		strcpy_s(szEXE, b);
		if (!strcmp(szEXE, s.c_str()))
			++stdexe;
		choose = ::Process32Next(hProcessSnap, &pd);
	}
	return stdexe;
}
