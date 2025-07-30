// softwareinfo.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/sysinfo/softwareinfo.exe"

#include <iostream>
#include "SoftwareInfo.h"
#include "../../../HTMLFrame/HTMLFrame.h"
#include "../../../HTMLFrame/NTVersion.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

using namespace std;
int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\");

    HTMLFrame HTML;
    HTML.Register(PT_INDEX, ADDRESS, "softwareinfo", "softwareinfo", "", true);

    string Code = R"(
    <center>
)";

    Code += "<h4> Windows 信息 </h4>\n";
    NTVERSIONINFO Info;
    GetWindowsNTVersionEx(Info, false);
    Code += (string)"<div> " + Info.OSName + "</div>\n";
    Code += "<div> 版本 " + GetBuildLabEx() + "</div>\n";
    Code += "<div> 启动时间：" + GetBootTime() + " </div>\n";
    Code += "<div> 计算机名：" + GetHostName() + " </div>\n";
    Code += "<br>";

    Code += R"(
    </center>
)";

    AJAXOutput(Code);

    return 0;
}
