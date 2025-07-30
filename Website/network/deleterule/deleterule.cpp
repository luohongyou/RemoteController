// deleterule.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/network/deleterule.exe"

#include <iostream>
#include <vector>
#include <Windows.h>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

std::vector<std::string> List;
int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\");

    if (argc != 2)
        return 0;

    HTMLFrame HTML;
    HTML.Register(PT_EMPTY, ADDRESS, "deleterule", "deleterule", "", true);

    if (HTML.User.GetUserType() == UT_GUEST)
        return 0;

    std::string DelDomain = argv[1];

    char szPath[MAX_PATH];
    GetSystemDirectoryA(szPath, MAX_PATH);
    std::string Path = szPath, temp;
    Path += "\\drivers\\etc\\hosts";
    std::ifstream fin(Path);
    int cnt = 0, DelK = 0;
    for (int i = 0; !fin.eof(); i++, cnt++)
    {
        getline(fin, temp);
        List.push_back(temp);
        bool flag = false;
        for (int j = 0; j < List.back().size(); j++)
            if (List.back()[j] != ' ' && List.back()[j] != '\n' && List.back()[j] != '\0' && List.back()[j] != '\t')
            {
                if (List.back()[j] != '#')
                    flag = true;
                break;
            }
        if (flag)
        {
            std::string Domain, IP;
            std::stringstream tempIO;
            tempIO << List.back();
            tempIO >> IP >> Domain;
            if (Domain == DelDomain)
                DelK = i;
        }
    }
    fin.close();

    if (DelK == 0)
    {
        AJAXOutput("Err");
        HTML.Log("在删除关于域名 " + DelDomain + " 的规则时出现错误：未找到规则", "network", LL_WARNING);
        return 0;
    }

    std::ofstream fout(Path);
    for (int i = 0; i < cnt; i++)
    {
        if (i != DelK)
            fout << List[i];
        if (i != cnt - 1)
            fout << std::endl;
    }
    fout.close();

    WinExec("ipconfig.exe /flushdns", SW_HIDE);

    AJAXOutput("Yes");
    HTML.Log("成功删除了关于域名 " + DelDomain + " 的规则", "network", LL_INFO);

    return 0;
}
