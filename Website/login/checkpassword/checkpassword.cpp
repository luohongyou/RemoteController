// checkpassword.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2024 Coral Studio. All Rights Reserved.

#define _CRT_SECURE_NO_DEPRECATE

#define ADDRESS "/login/checkpassword.exe"

#include <iostream>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\");

    if (argc != 3)
        return 0;

    CUser User;
    User.Assign(argv[1], argv[2]);
    if (User.GetToken() == "")
    {
        puts("Content-type: text/html\n");
        printf("false");
        return 0;
    }

    std::ofstream fout(UserFile(User.GetGUID(), "info", "token"));
    fout << NewToken();
    fout.close();

    User.Assign(argv[1], argv[2]);

    fout.open(User.Data("UserAgent"));
    fout << getenv("HTTP_USER_AGENT");
    fout.close();

    bool ChangePassword;
    std::ifstream fin(User.Data("ChangePassword"));
    ChangePassword = !fin.fail();
    fin.close();

    puts(("Set-Cookie:CORAL_REMOTE_CONTROLLER_MANAGE=" + User.GetToken() + "; path=/").c_str());
    puts("Content-type: text/html\n");
    if (ChangePassword)
        printf("change");
    else
        printf("true");

    return 0;

}
