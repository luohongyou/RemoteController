// datatracker.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define _CRT_SECURE_NO_DEPRECATE

#define ADDRESS "/login/datatracker.exe"

#include <iostream>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\");

    if (argc != 4) // Mode: User / Cloud / None; Base64; Fingerprint
        return 0;

    if (!strcmp(argv[1], "User"))
        ;
    else if (!strcmp(argv[1], "Cloud"))
        ;
    else
        ;


    
    puts("Content-type: text/html\n");


    return 0;

}
