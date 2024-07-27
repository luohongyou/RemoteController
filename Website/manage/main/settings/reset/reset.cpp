// reset.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2024 Coral Studio. All Rights Reserved.

#define ADDRESS "/manage/main/settings/reset.exe"

#include <iostream>
#include "../../../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
	SetAppDirectory("\\..\\..\\..\\..\\");

	HTMLFrame HTML;
	HTML.Register(PT_EMPTY, ADDRESS, "reset", "reset", "", true);
	HTML.ManageInit();
	if (HTML.User.GetUserType() != UT_ROOT)
		return 0;

	// 删除 htdocs\\temp
	RemoveDir("temp");
	CreateDirectoryA("temp", NULL);
	CreateDirectoryA("temp\\download", NULL);
	CreateDirectoryA("temp\\wallpaper", NULL);

	// 删除 htdocs\\screenshoot\cache
	RemoveDir("screenshoot\\cache");
	CreateDirectoryA("screenshoot\\cache", NULL);

	// 删除 ManageUI\\update\\packages
	RemoveDir("..\\..\\ManageUI\\update\\packages");
	CreateDirectoryA("..\\..\\ManageUI\\update\\packages", NULL);

	// 删除 ProgramData\\Coral Remote Controller
	RemoveDir(DatabasePath().c_str());

	// 删除 %localappdata%\\Coral Remote Controller
	RCAgent("reset", HTML.User.GetName(), "");

	AJAXOutput("Yes");
	HTML.Log("成功重置了系统", "manage", LL_INFO);

	return 0;
}
