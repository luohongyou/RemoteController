// clearstorage.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/manage/main/storage/clearstorage.exe"

#include <iostream>
#include "../../../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
	SetAppDirectory("\\..\\..\\..\\..\\");

	if (argc != 2)
		return 0;

	HTMLFrame HTML;
	HTML.Register(PT_EMPTY, ADDRESS, "clearstorage", "clearstorage", "", true);
	HTML.ManageInit();

	int ID = String2Num(argv[1]);

	if (ID == 1)
	{
		// 删除 htdocs\\temp
		RemoveDir("temp");
		CreateDirectoryA("temp", NULL);
		CreateDirectoryA("temp\\data", NULL);
		CreateDirectoryA("temp\\download", NULL);
		CreateDirectoryA("temp\\wallpaper", NULL);

		HTML.Log("成功清空了所有临时文件", "manage", LL_INFO);
	}
	else if (ID == 2)
	{
		// 删除 htdocs\\storage\\screenshot
		RemoveDir("storage\\screenshot");
		CreateDirectoryA("storage\\screenshot", NULL);

		// 清空屏幕截图列表
		std::ifstream fin(DatabasePath() + "User\\list");

		int tot;
		fin >> tot;
		std::string _uname, _guid;
		for (int i = 1; i <= tot; i++)
		{
			fin >> _uname >> _guid;
			remove((DatabasePath() + "User\\" + _guid + "\\data\\screenshotlist").c_str());
		}
		fin.close();

		remove((DatabasePath() + "User\\00000000-0000-0000-0000-000000000000\\data\\screenshotlist").c_str());

		HTML.Log("成功清空了所有屏幕截图", "manage", LL_INFO);
	}
	else if (ID == 3)
	{
		if (HTML.User.GetUserType() != UT_ROOT)
			return 0;

		// 删除 ManageUI\\update\\packages
		RemoveDir("..\\..\\ManageUI\\update\\packages");
		CreateDirectoryA("..\\..\\ManageUI\\update\\packages", NULL);

		HTML.Log("成功清空了上传的更新程序包", "manage", LL_INFO);
	}
	else if (ID == 4)
	{
		// 删除 htdocs\\storage\\sharedzone
		RemoveDir("storage\\sharedzone");
		CreateDirectoryA("storage\\sharedzone", NULL);

		HTML.Log("成功清空了共享云盘的所有文件", "manage", LL_INFO);
	}

	AJAXOutput("Yes");

	return 0;
}
