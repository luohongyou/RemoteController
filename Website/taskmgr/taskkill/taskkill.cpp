// taskkill.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/taskmgr/taskkill.exe"

#include <iostream>
#include <vector>
#include <Windows.h>
#include <tlhelp32.h>
#include <comdef.h>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\");

    if (argc != 3)
        return 0;

    int PID = String2Num(argv[1]);
    bool IsAll = String2Num(argv[2]);
    std::string ProcessName;

    HTMLFrame HTML;
    HTML.Register(PT_EMPTY, ADDRESS, "taskill", "taskkill", "", true);

    if (HTML.User.GetUserType() == UT_GUEST)
        return 0;

    if (!IsAll)
    {
        HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, PID);
        ::TerminateProcess(hProcess, 0);
        CloseHandle(hProcess);
    }
    else
    {
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
                if (PID == (int)pd.th32ProcessID)
                {
                    _bstr_t ExeFile(pd.szExeFile);
                    ProcessName = ExeFile;
                    break;
                }
                choose = ::Process32Next(hProcessSnap, &pd);
            }
        }
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
                _bstr_t ExeFile(pd.szExeFile);
                if (!strcmp(ExeFile, ProcessName.c_str()))
                {
                    HANDLE std = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pd.th32ProcessID);
                    ::TerminateProcess(std, 0);
                }
                choose = ::Process32Next(hProcessSnap, &pd);
            }
        }

    }

    AJAXOutput("Yes");
    HTML.Log((std::string)"已尝试结束" + (IsAll ? "全部 " + ProcessName : (std::string)"PID为" + argv[1] + "的进程"), "taskmgr", LL_INFO);

    return 0;
}