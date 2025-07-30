// getwallpaper.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/power/getwallpaper.exe"

#include <iostream>
#include <string>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\");

    HTMLFrame HTML;
    HTML.Register(PT_EMPTY, ADDRESS, "getwallpaper", "getwallpaper", "", true);

    std::string WallpaperResult = RCAgent("getwallpaper", HTML.User.GetName(), "");
    std::string Code;

    if (WallpaperResult == "Yes")
    {
        time_t now = time(0);
        std::stringstream tempIO;
        tempIO << now;

        Code = R"(<center><img src="\temp\wallpaper\currentwallpaper.jpg?)" + tempIO.str() + R"(" width="100%"/></center>)";
    }
    else
    {
        Code = "<br><br><br><center><h2>暂无有效的壁纸</h2></center>";
        if (WallpaperResult != "No")
            HTML.Log("获取壁纸时出现错误：" + WallpaperResult, "power", LL_ERROR);
    }

    HTML.Custom(Code);

    HTML.Output();

    return 0;

}
