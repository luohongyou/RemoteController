// listrule.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/network/listrule.exe"

#include <iostream>
#include <Windows.h>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\");

    HTMLFrame HTML;
    HTML.Register(PT_EMPTY, ADDRESS, "listrule", "listrule", "", true);

    std::string Code = R"(
<center>
<table class="table table-hover">
<tr>
<th>域名</th>
<th>链接的IP地址</th>
<th>操作</th>
</tr>
)";
    char szPath[MAX_PATH];
    GetSystemDirectoryA(szPath, MAX_PATH);
    std::string Path = szPath, line;
    Path += "\\drivers\\etc\\hosts";
    std::ifstream fin(Path);
    while (!fin.eof())
    {
        getline(fin, line);
        bool flag = false;
        for (int i = 0; i < line.size(); i++)
            if (line[i] != ' ' && line[i] != '\n' && line[i] != '\0' && line[i] != '\t')
            {
                if (line[i] != '#')
                    flag = true;
                break;
            }
        if (flag)
        {
            std::string Domain, IP;
            std::stringstream tempIO;
            tempIO << line;
            tempIO >> IP >> Domain;
            Code += "<tr>\n";
            Code += "<td>" + Domain + "</td>\n";
            Code += "<td>" + IP + "</td>\n";
            if (HTML.User.GetUserType() != UT_GUEST)
                Code += "<td><a href=\"javascript:void(0);\" onclick=\"Delete('" + Domain + "')\">删除</a></td>\n";
            else
                Code += "<td></td>\n";
            Code += "</tr>\n";
        }
    }
    fin.close();
    Code += R"(
</table>
</center>
)";

    AJAXOutput(Code);

    return 0;
}
