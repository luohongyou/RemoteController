// clearlog.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/manage/main/log/clearlog.exe"

#include <iostream>
#include <vector>
#include "../../../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
	SetAppDirectory("\\..\\..\\..\\..\\");

	if (argc != 2)
		return 0;

	int LogType = String2Num(argv[1]);

	HTMLFrame HTML;
	HTML.Register(PT_EMPTY, ADDRESS, "clearlog", "clearlog", "", true);
	HTML.ManageInit();
	if (HTML.User.GetUserType() != UT_ROOT)
		return 0;

	std::string Type;
	if (LogType == 1)        // 基本日志
		remove((DatabasePath() + "Log\\main.log").c_str()), Type = "基本日志";
	else if (LogType == 2)   // Website 日志
		remove((DatabasePath() + "Log\\full.log").c_str()), Type = "Website 日志";
	else if (LogType == 3)   // Agent 日志
		RCAgent("removelogfile", HTML.User.GetName(), ""), Type = "Agent 日志";

	AJAXOutput("Yes");
	HTML.Log("已清空 " + Type, "manage", LL_WARNING);

	return 0;
}

