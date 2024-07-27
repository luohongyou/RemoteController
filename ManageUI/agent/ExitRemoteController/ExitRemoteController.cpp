// ExitRemoteController.cpp : 退出Coral Remote Controller。

// 版权声明：
// Copyright (C) 2020-2024 Coral Studio. All Rights Reserved.

#include <iostream>
#include <Windows.h>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

int main()
{
    SetAppDirectory("\\..\\..\\..\\");  // '\'

    // 退出 Remote Agent
    StopProcess("agentprotect.exe");
    RCAgent("exit", "ForegroundUser", "");

    // 关闭 Website
    ShellExecuteA(NULL, "runas", "uninstall.bat", "", NULL, SW_HIDE);

    return 0;
}
