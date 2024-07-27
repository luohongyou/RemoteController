// index_manage_main.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2024 Coral Studio. All Rights Reserved.

#define ADDRESS "/manage/main/"

#include <iostream>
#include "../../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\..\\");

    HTMLFrame HTML;
    HTML.Register(PT_INDEX, ADDRESS, "后台管理", "后台管理", R"(<li><a href="/">主页</a></li><li>后台管理</li>)", true);
    HTML.ManageInit();

    std::string Code;

    HTMLToolCards ToolCards;
    ToolCards.Add("系统设置", "对Coral Remote Controller进行全方位的详细配置，满足您的要求。", "/manage/main/settings");
    ToolCards.Add("用户管理", "对有权限访问Coral Remote Controller的用户进行控制和管理。", "/manage/main/user");
    ToolCards.Add("存储管理", "管理Coral Remote Controller用户数据和临时文件占用的磁盘空间。", "/manage/main/storage");
    ToolCards.Add("日志查看", "查看Coral Remote Controller各组件生成的日志，以进行错误排查和事故调查。", "/manage/main/log");
    if (HTML.User.GetUserType() == UT_ROOT)
        ToolCards.Add("程序更新", "上传Coral Remote Controller的更新包，对程序进行远程更新。", "/manage/main/update");
    Code += ToolCards.Output();

    HTML.Custom(Code);

    HTML.Output();
    return 0;
}
