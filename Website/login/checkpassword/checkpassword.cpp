// checkpassword.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

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

    std::string Result = RCAuth("RequestLogin", (std::string)argv[1] + " " + argv[2]);
    if (Result == "")
    {
        puts("Content-type: text/html\n");
        printf("false");
        return 0;
    }
    std::stringstream tempIO;
    std::string _GUID, NewToken;
    tempIO << Result;
    tempIO >> _GUID >> NewToken;

    CUser User;
    User.Assign(_GUID, argv[1]);
    if (User.GetGUID() == "")
    {
        puts("Content-type: text/html\n");
        printf("false");
        return 0;
    }

    std::ofstream fout;
    fout.open(User.Data("UserAgent"));
    fout << getenv("HTTP_USER_AGENT");
    fout.close();

    fout.open(User.Data("IPAddress"));
    fout << getenv("REMOTE_ADDR");
    fout.close();

    int IsLoginEvent = GetSettings("loginevent", true);
    if (IsLoginEvent)
        RCAgent("loginevent", User.GetName(), "");

    bool ChangePassword;
    std::ifstream fin(User.Data("ChangePassword"));
    ChangePassword = !fin.fail();
    fin.close();

    puts(("Set-Cookie:CORAL_REMOTE_CONTROLLER_MANAGE=" + NewToken + "; path=/").c_str());
    puts("Content-type: text/html\n");
    if (ChangePassword)
        printf("change");
    else
        printf("true");

    return 0;

}
