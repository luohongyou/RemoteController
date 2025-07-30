// powercfg.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/power/powercfg.exe"

#include <iostream>
#include <Windows.h>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\");

    if (argc != 4)
        return 0;

    HTMLFrame HTML;
    HTML.Register(PT_EMPTY, ADDRESS, "powercfg", "powercfg", "", true);

    if (HTML.User.GetUserType() == UT_GUEST)
        return 0;

    std::string Ans = RCAgent("shutdown", HTML.User.GetName(), (std::string)argv[1] + " " + argv[2] + " " + argv[3]);

    if (Ans != "CORAL_RC_AGENT_FAILED")
    {
        AJAXOutput("Yes");

        int sysAction = String2Num(argv[1]);
        std::string Action;
        if (sysAction == 0) Action = "-a"; 
        else if (sysAction == 1) Action = "-s"; 
        else if (sysAction == 2) Action = "-r"; 
        else Action = "-h";
        HTML.Log("成功执行了电源操作<br>操作：" + Action + "，强制：" + argv[2] + "，延迟：" + argv[3], "power", LL_INFO);
    }
    else
    {
        AJAXOutput("Failed!");
        HTML.Log("尝试执行电源操作失败", "power", LL_ERROR);
    }

    return 0;
}