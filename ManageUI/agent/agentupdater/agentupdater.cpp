// agentupdater.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#include <iostream>
#include <Windows.h>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

int main()
{
	SetAppDirectory("\\..\\..\\..\\..\\");  // 复制到 temp\agentupdater.exe

	std::ifstream fin;
	while (true)
	{
		fin.open("Website\\htdocs\\temp\\Updated.ini");
		if (!fin.fail())
		{
			fin.close();
			break;
		}
		fin.close();
		if (!CheckProcess("rcupdater.exe"))
			break;

		Sleep(1000);
	}

	ShellExecuteA(NULL, "open", "ManageUI\\agent\\agentprotect.exe", "", NULL, SW_HIDE);

	return 0;
}
