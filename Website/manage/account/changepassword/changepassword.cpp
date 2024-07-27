﻿// changepassword.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2024 Coral Studio. All Rights Reserved.

#define ADDRESS "/manage/account/changepassword.exe"

#include <iostream>
#include "../../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\..\\");

    if (argc != 3)
        return 0;

    std::string OldPassword = argv[1];
    std::string NewPassword = argv[2];

    HTMLFrame HTML;
    HTML.Register(PT_EMPTY, ADDRESS, "changepassword", "changepassword", "", true);

    if (HTML.User.GetPasswordMD5() != OldPassword)
    {
        AJAXOutput("No");
        return 0;
    }

    std::ofstream fout(UserFile(HTML.User.GetGUID(), "info", "md5"));
    fout << NewPassword;
    fout.close();

    fout.open(UserFile(HTML.User.GetGUID(), "info", "token"));
    fout << NewToken();
    fout.close();

    AJAXOutput("Yes");
    HTML.Log("用户已更改密码", "manage-account", LL_INFO);

    return 0;

}
