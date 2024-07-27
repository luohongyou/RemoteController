﻿// forcechangepw.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2024 Coral Studio. All Rights Reserved.

#define ADDRESS "/OOBE/user/changepassword/forcechangepw.exe"

#include <iostream>
#include "../../../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\..\\..\\");

    if (argc != 3)
        return 0;

    std::string OldPassword = argv[1];
    std::string NewPassword = argv[2];

    HTMLFrame HTML;
    HTML.Register(PT_EMPTY, ADDRESS, "forcechangepw", "forcechangepw", "", true);

    if (HTML.User.GetPasswordMD5() != OldPassword)
    {
        AJAXOutput("No");
        return 0;
    }

    std::ofstream fout(UserFile(HTML.User.GetGUID(), "info", "md5"));
    fout << NewPassword;
    fout.close();

    remove((DatabasePath() + "User\\" + HTML.User.GetGUID() + "\\data\\ChangePassword").c_str());

    AJAXOutput("Yes");
    HTML.Log("用户已完成强制密码修改", "OOBE", LL_INFO);

    return 0;

}
