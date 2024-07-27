// newpassword.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2024 Coral Studio. All Rights Reserved.

#define ADDRESS "/OOBE/user/newpassword.exe"

#include <iostream>
#include "../../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\..\\");

    if (argc != 2)
        return 0;

    std::string OldPassword = "e10adc3949ba59abbe56e057f20f883e";
    std::string NewPassword = argv[1];

    HTMLFrame HTML;
    HTML.Register(PT_EMPTY, ADDRESS, "newpassword", "newpassword", "", true);

    if (HTML.User.GetPasswordMD5() != OldPassword)
    {
        AJAXOutput("No");
        return 0;
    }

    std::ofstream fout(UserFile(HTML.User.GetGUID(), "info", "md5"));
    fout << NewPassword;
    fout.close();

    AJAXOutput("Yes");

    return 0;

}
