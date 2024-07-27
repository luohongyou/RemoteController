// settingswitcher.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2024 Coral Studio. All Rights Reserved.

#define ADDRESS "/manage/main/settings/settingswitcher.exe"

#include <iostream>
#include "../../../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
	SetAppDirectory("\\..\\..\\..\\..\\");

	if (argc != 3)
		return 0;

	std::string ID = argv[1];
	int Status = String2Num(argv[2]);

	HTMLFrame HTML;
	HTML.Register(PT_EMPTY, ADDRESS, "settingswitcher", "settingswitcher", "", true);
	HTML.ManageInit();

	if (ID == "SystemStatus")
		SetSettings("SystemStatus", Status);
	else if (ID == "Tray")
		RCAgent("settraystatus", HTML.User.GetName(), argv[2]);
	else if (ID == "AutoRefresh")
		SetSettings("AutoRefresh", Status);
	else if (ID == "AgentProtect")
	{
		SetSettings("AgentProtect", Status);
		if (Status)
		{
			if (!CheckProcess("agentprotect.exe"))
				RCAgent("startprotect", HTML.User.GetName(), "");
		}
		else
			StopProcess("agentprotect.exe");
	}
	else if (ID == "Log")
	{
		if (HTML.User.GetUserType() != UT_ROOT)
			return 0;
		SetSettings("Log", Status);
	}
	else if (ID == "NewFeature")
		SetSettings("NewFeature", Status);
	else if (ID == "StrongPassword")
	{
		SetSettings("StrongPassword", Status);
		if (Status)
		{
			// 配置强制修改密码
			std::ifstream fin(DatabasePath() + "User\\list"), fin2;
			std::ofstream fout;

			int tot;
			fin >> tot;
			std::string _uname, _guid, _md5;
			for (int i = 1; i <= tot; i++)
			{
				fin >> _uname >> _guid;

				fin2.open(UserFile(_guid, "info", "md5"));
				fin2 >> _md5;
				fin2.close();

				if (_md5 == "e10adc3949ba59abbe56e057f20f883e")
					continue;

				fout.open(DatabasePath() + "User\\" + _guid + "\\data\\ChangePassword");
				fout << 1;
				fout.close();
			}
			fin.close();
		}
	}
	else if (ID == "UserAgentCheck")
		SetSettings("UserAgentCheck", Status);

	AJAXOutput("Yes");
	HTML.Log("成功修改了设置，设置项" + ID + ": " + (Status ? "true" : "false"), "manage", LL_INFO);

	return 0;
}

