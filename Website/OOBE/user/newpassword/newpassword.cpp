// newpassword.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/OOBE/user/newpassword.exe"

#include <iostream>
#include "../../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\..\\");

    if (argc != 3)
        return 0;

    HTMLFrame HTML;
    HTML.Register(PT_EMPTY, ADDRESS, "newpassword", "newpassword", "", true);

    std::string NewPassword = RCAuth("ChangePassword", HTML.User.GetGUID() + " " + argv[1] + " " + argv[2]);

    if (NewPassword == "No")
    {
        AJAXOutput("No");
        return 0;
    }

    std::ofstream fout(UserFile(HTML.User.GetGUID(), "info", "md5"));
    fout << NewPassword;
    fout.close();

    RCAuth("RefreshUser", HTML.User.GetGUID());

    AJAXOutput("Yes");

    return 0;

}
