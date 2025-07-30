// onewaymessage.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/message/onewaymessage.exe"

#include <iostream>
#include <vector>
#include <Windows.h>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\");

    if (argc != 5)
        return 0;

    HTMLFrame HTML;
    HTML.Register(PT_EMPTY, ADDRESS, "onewaymessage", "onewaymessage", "", true);

    if (HTML.User.GetUserType() == UT_GUEST)
        return 0;

    std::string Ans = RCAgent("onewaymessage", HTML.User.GetName(), (std::string)argv[1] + " " + argv[2] + " " + argv[3] + " " + argv[4]);

    if (Ans != "CORAL_RC_AGENT_FAILED")
    {
        AJAXOutput("Yes");
        HTML.Log("已发送自定义的单向消息", "message", LL_INFO);
    }
    else
    {
        AJAXOutput("Failed!");
        HTML.Log("向远程计算机发送消息时出现错误", "message", LL_ERROR);
    }

    return 0;
}