// legacycheckroot.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/OOBE/update/legacycheckroot.exe"

#include <iostream>
#include "../../../../HTMLFrame/HTMLFrame.h"
#include "../index_OOBE_update/LegacyFormat.h"
#pragma comment(lib, "../../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\..\\");

    if (argc != 2)
        return 0;

    LegacyUser User;
    User.LegacyLoadUser("root", argv[1]);
    if (User.GetToken() == "")
    {
        puts("Content-type: text/html\n");
        printf("false");
        return 0;
    }

    puts("Content-type: text/html\n");
    printf(User.GetToken().c_str());

    return 0;

}
