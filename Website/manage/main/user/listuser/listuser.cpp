// listuser.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2024 Coral Studio. All Rights Reserved.

#define ADDRESS "/manage/main/user/listuser.exe"

#include <iostream>
#include "../../../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
	SetAppDirectory("\\..\\..\\..\\..\\");

	HTMLFrame HTML;
	HTML.Register(PT_EMPTY, ADDRESS, "listuser", "listuser", "", true);
	HTML.ManageInit();

	std::string Code = R"(
<table class="table table-hover">
<tr>
<th>用户</th>
<th>权限</th>
<th>最近使用</th>
<th>操作</th>
</tr>
)";
	
	std::ifstream fin, fin2;
	int cnt = 0, _UserType;
	std::string _Name, _GUID, _LastRequest, _sUserType;
	fin.open(DatabasePath() + "User\\list");
	fin >> cnt;

	for (int i = 1; i <= cnt; ++i)
	{
		fin >> _Name >> _GUID;

		fin2.open(UserFile(_GUID, "info", "type"));
		fin2 >> _UserType;
		fin2.close();
		if (_UserType == UT_ROOT) _sUserType = "超级管理员";
		else if (_UserType == UT_ADMIN) _sUserType = "管理员";
		else if (_UserType == UT_USER) _sUserType = "标准用户";
		else if (_UserType == UT_GUEST) _sUserType = "访客用户";
		else _sUserType = "";

		fin2.open(UserFile(_GUID, "data", "lastrequest"));
		if (fin2.fail())
			_LastRequest = "-";
		else
			getline(fin2, _LastRequest);
		fin2.close();

		Code += "<tr>\n";
		Code += "<td>" + _Name + "</td>\n";
		Code += "<td>" + _sUserType + "</td>\n";
		Code += "<td>" + _LastRequest + "</td>\n";
		if (_UserType > HTML.User.GetUserType())
			Code += "<td><a href=\"javascript:void(0);\" onclick=\"DeleteUser(\'" + _Name + "\')\">删除</a></td>\n";
		else
			Code += "<td></td>\n";
		Code += "</tr>\n";
	}
	fin.close();

	Code += R"(
</table>
)";

	AJAXOutput(Code);

	return 0;
}

