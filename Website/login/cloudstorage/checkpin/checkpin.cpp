// checkpin.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define _CRT_SECURE_NO_DEPRECATE

#define ADDRESS "/login/cloudstorage/checkpin.exe"

#include <iostream>
#include "../../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\..\\");

    if (argc != 2)
        return 0;

    std::string Token = RCAuth("RequestCloudLogin", argv[1]);
    if (Token == "")
    {
        puts("Content-type: text/html\n");
        printf("false");
        return 0;
    }

    int IsLoginEvent = GetSettings("loginevent", true);
    if (IsLoginEvent)
    {
        std::string IPAddress;
        if (IPAddress == "::1") IPAddress = "localhost";
        RCAgent("pinloginevent", "CloudStorageUser", IPAddress);
    }

    puts(("Set-Cookie:CORAL_REMOTE_CONTROLLER_MANAGE=" + Token + "; path=/").c_str());
    puts("Content-type: text/html\n");
    printf("true");

    return 0;
}
