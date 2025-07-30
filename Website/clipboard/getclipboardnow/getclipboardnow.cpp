// getclipboardnow.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/clipboard/getclipboardnow.exe"

#include <iostream>
#include <Windows.h>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\");

    HTMLFrame HTML;
    HTML.Register(PT_EMPTY, ADDRESS, "getclipboardnow", "getclipboardnow", "", true);

    std::string clipboardText;
    clipboardText = RCAgent("getclipboard", HTML.User.GetName(), "");
    AJAXOutput(clipboardText);

    return 0;
}