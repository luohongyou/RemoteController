// deleteuser.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/manage/main/user/deleteuser.exe"

#include <iostream>
#include <vector>
#include "../../../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../../../x64/Release/HTMLFrame.lib")

std::vector<std::pair<std::string, std::string>> UserList;
int main(int argc, char* argv[])
{
	SetAppDirectory("\\..\\..\\..\\..\\");

	if (argc != 2)
		return 0;

	std::string User = argv[1], UserGUID;

	HTMLFrame HTML;
	HTML.Register(PT_EMPTY, ADDRESS, "deleteuser", "deleteuser", "", true);
	HTML.ManageInit();

	std::ifstream fin, fin2;
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
			int Type = 0;
			fin2.open(UserFile(_GUID, "info", "type"));
			fin2 >> Type;
			fin2.close();

			if (Type <= HTML.User.GetUserType())
			{
				AJAXOutput("No");
				return 0;
			}

			UserGUID = _GUID;
			UserList.pop_back();
		}
	}
	fin.close();

	std::string FileName;
	fin.open(UserFile(UserGUID, "data", "screenshotlist"));
	fin >> cnt;
	for (int i = 1; i <= cnt; ++i)
	{
		fin >> FileName;
		remove(("storage\\screenshot\\" + FileName + ".jpg").c_str());
	}
	fin.close();

	RemoveDir((DatabasePath() + "User\\" + UserGUID).c_str());

	fout.open(DatabasePath() + "User\\list");
	fout << UserList.size() << std::endl;
	for (auto UserItem : UserList)
		fout << UserItem.first << std::endl << UserItem.second << std::endl;
	fout.close();

	RCAuth("DeleteUser", UserGUID);

	AJAXOutput("Yes");
	HTML.Log("已删除用户：" + User, "manage", LL_INFO);

	return 0;
}
