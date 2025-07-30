// modifyclipboard.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/clipboard/modifyclipboard.exe"

#include <iostream>
#include <Windows.h>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\");

    if (argc != 2)
        return 0;

    HTMLFrame HTML;
    HTML.Register(PT_EMPTY, ADDRESS, "modifyclipboard", "modifyclipboard", "", true);

    if (HTML.User.GetUserType() == UT_GUEST)
    {
        HTML.Log("用户没有权限修改远程计算机的剪贴板", "clipboard", LL_ERROR);
        return 0;
    }

    RCAgent("modifyclipboard", HTML.User.GetName(), argv[1]);
    AJAXOutput("Yes");

    HTML.Log("已尝试修改远程计算机的剪贴板内容", "clipboard", LL_INFO);

    return 0;
}