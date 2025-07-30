// addrule.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/network/addrule.exe"

#include <iostream>
#include <Windows.h>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\");

    if (argc != 3)
        return 0;

    HTMLFrame HTML;
    HTML.Register(PT_EMPTY, ADDRESS, "addrule", "addrule", "", true);

    if (HTML.User.GetUserType() == UT_GUEST)
        return 0;

    std::string NewDomain = argv[1], NewIP = argv[2];

    char szPath[MAX_PATH];
    GetSystemDirectoryA(szPath, MAX_PATH);
    std::string Path = szPath, temp;
    Path += "\\drivers\\etc\\hosts";
    std::ifstream fin(Path);
    int cnt = 0;
    while (!fin.eof())
    {
        getline(fin, temp);
        bool flag = false;
        for (int j = 0; j < temp.size(); j++)
            if (temp[j] != ' ' && temp[j] != '\n' && temp[j] != '\0' && temp[j] != '\t')
            {
                if (temp[j] != '#')
                    flag = true;
                break;
            }
        if (flag)
        {
            std::string Domain, IP;
            std::stringstream tempIO;
            tempIO << temp;
            tempIO >> IP >> Domain;
            if (Domain == NewDomain)
            {
                AJAXOutput("Err");
                HTML.Log("在添加关于域名 " + NewDomain + " 的规则时出现错误：规则冲突", "network", LL_WARNING);
                return 0;
            }
        }
    }
    fin.close();

    std::ofstream fout(Path, std::ios::app);
    fout << '\n' << NewIP << ' ' << NewDomain;
    fout.close();

    WinExec("ipconfig.exe /flushdns", SW_HIDE);

    AJAXOutput("Yes");
    HTML.Log("成功添加了关于域名 " + NewDomain + " 的规则", "network", LL_INFO);

    return 0;
}
