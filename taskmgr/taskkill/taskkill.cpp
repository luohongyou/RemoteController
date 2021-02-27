// taskkill.cpp : Coral Remote Controller 任务管理 结束任务模块

// 版权声明：
// Copyright (C) 2020-2021 Coral Studio. All Rights Reserved.

// 调用方式：taskkill.exe PID [all]

#include <iostream>
#include <Windows.h>
#include <tlhelp32.h>
#include <comdef.h>
#include <sstream>
using namespace std;

bool IsAll = 0;              // 是否结束所有该名称进程
string ProcessName;          // 结束的进程名称
int main(int argc, char* argv[])
{
    // 输出HTML
    puts("Content-type: text/html\n");
    string Code1 = R"(
<!DOCTYPE html>
<html lang="zh">
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=gb2312">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>taskkill - Coral Remote Controller</title>
    <!-- Bootstrap -->
    <link href="/css/bootstrap-4.4.1.css" rel="stylesheet">
  </head>
  <body>
    <script>
)";
    string Code2 = R"(
window.location.href = "/taskmgr/tasklist.exe";
    </script>
    <!-- jQuery (necessary for Bootstrap's JavaScript plugins) --> 
    <script src="/js/jquery-3.4.1.min.js"></script> 
    <!-- Include all compiled plugins (below), or include individual files as needed --> 
    <script src="/js/popper.min.js"></script> 
    <script src="/js/bootstrap-4.4.1.js"></script>
  </body>
</html>
)";

    // 检查参数
    if (argc == 3)
    {
        if (strcmp(argv[2], "all"))
        {
            Code1 += "alert(\'Web Application 调用格式错误！\')\n";
            Code1 += Code2;
            puts(Code1.c_str());
            return 0;
        }
        IsAll = 1;
    }
    else if (argc != 2)
    {
        Code1 += "alert(\'Web Application 调用格式错误！\')\n";
        Code1 += Code2;
        puts(Code1.c_str());
        return 0;
    }

    // 取出PID
	string sPID = argv[1];
	int PID;
	stringstream tempIO;
	tempIO << sPID;
	tempIO >> PID;

    // 第一次扫描，根据PID找到进程
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
                if (!IsAll)
                {
                    HANDLE std = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pd.th32ProcessID);
                    ::TerminateProcess(std, 0);
                }
                break;
            }
            choose = ::Process32Next(hProcessSnap, &pd);
        }
    }

    // 检查PID
    if (ProcessName == "")
    {
        Code1 += "alert(\'找不到PID！\')\n";
        Code1 += Code2;
        puts(Code1.c_str());
        return 0;
    }
    
    // 第二次扫描，根据进程名找到进程
    if (IsAll)
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

    // 第三次扫描，检查是否成功结束
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
            if (!IsAll)
            {
                if (PID == (int)pd.th32ProcessID)
                {
                    Code1 += "alert(\'结束" + ProcessName + "失败！\')\n";
                    Code1 += Code2;
                    puts(Code1.c_str());
                    return 0;
                }
            }
            else
            {
                _bstr_t ExeFile(pd.szExeFile);
                if (!strcmp(ExeFile, ProcessName.c_str()))
                {
                    Code1 += "alert(\'结束" + ProcessName + "失败！\')\n";
                    Code1 += Code2;
                    puts(Code1.c_str());
                    return 0;
                }
            }
            
            choose = ::Process32Next(hProcessSnap, &pd);
        }
    }

    Code1 += Code2;
    puts(Code1.c_str());
    return 0;
}
