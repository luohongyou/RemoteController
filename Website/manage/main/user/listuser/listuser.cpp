// listuser.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/manage/main/user/listuser.exe"

#include <iostream>
#include "../../../../../HTMLFrame/HTMLFrame.h"
#include "../../../../../HTMLFrame/json.hpp"
#pragma comment(lib, "../../../../../x64/Release/HTMLFrame.lib")

using json = nlohmann::json;

int main(int argc, char* argv[])
{
	SetAppDirectory("\\..\\..\\..\\..\\");

	HTMLFrame HTML;
	HTML.Register(PT_EMPTY, ADDRESS, "listuser", "listuser", "", true);
	HTML.ManageInit();

	int DataTracker = GetSettings("DataTracker", true);

	std::string Code = (std::string)R"(
<table class="table table-hover">
<tr>
<th>用户</th>
<th>最近使用</th>
)" + (DataTracker ? R"(<th>使用的设备</th>)" : "") + R"(
<th>最近的IP</th>
<th>操作</th>
</tr>
)";
	
	std::ifstream fin, fin2;
	int cnt = 0, _UserType;
	std::string _Name, _GUID, _LastRequest, _sUserType, _IPAddress, _BrowserFingerprint, _JSONData;
	fin.open(DatabasePath() + "User\\list");
	fin >> cnt;

	for (int i = 1; i <= cnt; ++i)
	{
		fin >> _Name >> _GUID;

		fin2.open(UserFile(_GUID, "info", "type"));
		fin2 >> _UserType;
		fin2.close();
		if (_UserType == UT_ROOT) _sUserType = "<span class=\"badge text-bg-danger\">超级管理员</span>";
		else if (_UserType == UT_ADMIN) _sUserType = "<span class=\"badge text-bg-warning\">管理员</span>";
		else if (_UserType == UT_USER) _sUserType = "<span class=\"badge text-bg-primary\">标准用户</span>";
		else if (_UserType == UT_GUEST) _sUserType = "<span class=\"badge text-bg-success\">访客用户</span>";
		else _sUserType = "";

		fin2.open(UserFile(_GUID, "data", "lastrequest"));
		if (fin2.fail())
			_LastRequest = "-";
		else
			getline(fin2, _LastRequest);
		fin2.close();

		fin2.open(UserFile(_GUID, "data", "IPAddress"));
		if (fin2.fail())
			_IPAddress = "-";
		else
			getline(fin2, _IPAddress);
		if (_IPAddress == "::1") _IPAddress = "localhost";
		fin2.close();

		Code += "<tr>\n";
		Code += "<td>" + _Name + " " + _sUserType + "</td>\n";
		Code += "<td>" + _LastRequest + "</td>\n";

		if (DataTracker)
		{
			// 使用的设备
			fin2.open(DatabasePath() + "Log\\request\\latest\\" + _GUID + ".log");
			if (!fin2.fail())
			{
				getline(fin2, _BrowserFingerprint);
				fin2.get();
				getline(fin2, _JSONData);
				json DeviceInfo = json::parse(_JSONData);
				Code += "<td>" + (std::string)DeviceInfo["browser"] + " on " + (std::string)DeviceInfo["system"] + " " + (std::string)DeviceInfo["systemVersion"] + "</td>\n";
			}
			else
				Code += "<td>-</td>\n";
			fin2.close();
		}

		Code += "<td>" + _IPAddress + "</td>\n";
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

