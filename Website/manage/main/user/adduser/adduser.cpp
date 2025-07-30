// adduser.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/manage/main/user/adduser.exe"

#include <iostream>
#include <vector>
#include "../../../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../../../x64/Release/HTMLFrame.lib")

std::vector<std::pair<std::string, std::string>> UserList;
int main(int argc, char* argv[])
{
	SetAppDirectory("\\..\\..\\..\\..\\");

	if (argc != 3)
		return 0;

	std::string User = argv[1];
	int Type = String2Num(argv[2]);

	HTMLFrame HTML;
	HTML.Register(PT_EMPTY, ADDRESS, "adduser", "adduser", "", true);
	HTML.ManageInit();
	if (Type <= HTML.User.GetUserType())
	{
		AJAXOutput("No");
		return 0;
	}

	std::ifstream fin;
	std::ofstream fout;
	int cnt = 0;
	std::string _Name, _GUID;

	fin.open(DatabasePath() + "User\\list");
	fin >> cnt;

	for (int i = 1; i <= cnt; ++i)
	{
		fin >> _Name >> _GUID;
		UserList.push_back(std::make_pair(_Name, _GUID));

		if (User == _Name)
		{
			AJAXOutput("No");
			return 0;
		}
	}
	fin.close();

	GUID guid;
	CoCreateGuid(&guid);
	std::string GUID = GuidToString(guid);
	UserList.push_back(std::make_pair(User, GUID));

	CreateDirectoryA((DatabasePath() + "User\\" + GUID).c_str(), NULL);
	CreateDirectoryA((DatabasePath() + "User\\" + GUID + "\\info").c_str(), NULL);
	CreateDirectoryA((DatabasePath() + "User\\" + GUID + "\\data").c_str(), NULL);

	fout.open(UserFile(GUID, "info", "md5"));
	fout << "e10adc3949ba59abbe56e057f20f883e"; // "123456"
	fout.close();

	fout.open(UserFile(GUID, "info", "type"));
	fout << Type;
	fout.close();

	fout.open(DatabasePath() + "User\\list");
	fout << UserList.size() << std::endl;
	for (auto UserItem : UserList)
		fout << UserItem.first << std::endl << UserItem.second << std::endl;
	fout.close();

	RCAuth("AddUser", GUID);

	AJAXOutput("Yes");
	HTML.Log("已添加用户：" + User, "manage", LL_INFO);

	return 0;
}
