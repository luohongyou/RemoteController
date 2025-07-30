// clearclipboardhistory.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/clipboard/clearclipboardhistory.exe"

#include <iostream>
#include <Windows.h>
#include <stack>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

std::stack<std::pair<std::string, std::string>> List;

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\");

    HTMLFrame HTML;
    HTML.Register(PT_EMPTY, ADDRESS, "clearclipboardhistory", "clearclipboardhistory", "", true);
    if (HTML.User.GetUserType() >= UT_USER)
    {
        HTML.Log("用户没有权限清空远程计算机的剪贴板历史记录", "clipboard", LL_ERROR);
        return 0;
    }

    std::string FileName;
    FileName = RCAgent("getclipboardhistory", HTML.User.GetName(), "");

    remove(FileName.c_str());

    AJAXOutput("Yes");
    HTML.Log("已清空远程计算机的剪贴板历史记录", "clipboard", LL_INFO);

    return 0;
}