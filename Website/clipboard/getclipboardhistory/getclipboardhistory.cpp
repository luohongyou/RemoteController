// getclipboardhistory.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/clipboard/getclipboardhistory.exe"

#include <iostream>
#include <Windows.h>
#include <stack>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

std::stack<std::pair<std::string, std::string>> List;

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\");

    HTMLFrame HTML;
    HTML.Register(PT_EMPTY, ADDRESS, "getclipboardhistory", "getclipboardhistory", "", true);

    std::string Code, FileName;
    FileName = RCAgent("getclipboardhistory", HTML.User.GetName(), "");

    std::string temp, Time, Data, Label = "";
    std::ifstream fin(FileName);
    if (!fin.fail())
        while (!fin.eof())
        {
            getline(fin, temp);
            if (Label == "")
            {
                if (temp.substr(0, 6) == "RCDATA")
                {
                    Label = temp;
                    getline(fin, Time);
                    Data = "";
                }
                continue;
            }
            if (temp == Label)
            {
                List.push(std::make_pair(Time, Data));
                Label = "";
            }

            Data += temp + '\n';
        }

    Code = R"(
<center>
<table class="table">
<tr>
<th>时间</th>
<th>内容</th>
</tr>
)";

    while (!List.empty())
    {
        Code += "<tr>\n";
        Code += "<td>" + List.top().first + "</td>\n";
        Code += "<td><xmp style=\"margin:0px\">" + List.top().second + "</xmp></td>\n";
        Code += "</tr>\n";

        List.pop();
    }

    Code += R"(
</table>
</center>
)";

    AJAXOutput(Code);

    return 0;
}