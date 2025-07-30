// finishuserOOBE.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/OOBE/user/finishuserOOBE.exe"

#include <iostream>
#include "../../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\..\\");

    HTMLFrame HTML;
    HTML.Register(PT_INDEX, ADDRESS, "finishuserOOBE", "finishuserOOBE", "", true);

	std::ofstream fout(UserFile(HTML.User.GetGUID(), "info", "OOBE"));
    fout << OOBEVERSION;
    fout.close();

    AJAXOutput("Yes");
    HTML.Log("用户OOBE已完成，版本 " + Num2String(OOBEVERSION), "OOBE", LL_INFO);

    return 0;
}
