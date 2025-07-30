// closewindow.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/windowmgr/closewindow.exe"

#include <iostream>
#include <vector>
#include <Windows.h>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\");

    if (argc != 3)
        return 0;

    std::string WindowID = argv[1];
    bool IsForce = String2Num(argv[2]);

    HTMLFrame HTML;
    HTML.Register(PT_EMPTY, ADDRESS, "closewindow", "closewindow", "", true);

    if (HTML.User.GetUserType() == UT_GUEST)
        return 0;

    std::string PID = RCAgent("closewindow", HTML.User.GetName(), WindowID + " " + argv[2]);
    if (IsForce)
    {
        HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, String2Num(PID));
        ::TerminateProcess(hProcess, 0);
        CloseHandle(hProcess);
    }

    AJAXOutput("Yes");
    HTML.Log((std::string)"已尝试" + (IsForce ? "强制" : "") + "关闭窗口，ID:" + argv[1], "windowmgr", LL_INFO);

    return 0;
}