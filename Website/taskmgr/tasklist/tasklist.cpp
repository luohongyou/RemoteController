// tasklist.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/taskmgr/tasklist.exe"

#include <iostream>
#include <vector>
#include <algorithm>
#include <Windows.h>
#include <tlhelp32.h>
#include <comdef.h>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

std::vector<std::pair<std::string, int>> List;
bool cmp(std::pair<std::string, int> A, std::pair<std::string, int> B)
{
	for (auto &a : A.first)
		a = std::tolower(a);
	for (auto& b : B.first)
		b = std::tolower(b);

	if (A != B)
		return A < B;
	else
		return A.second < B.second;
}
int main(int argc, char* argv[])
{
	SetAppDirectory("\\..\\..\\");

	HTMLFrame HTML;
	HTML.Register(PT_EMPTY, ADDRESS, "tasklist", "tasklist", "", true);

	std::string Code = R"(
<center>
<table class="table">
<tr>
<th>名称</th>
<th>PID</th>
<th>操作</th>
</tr>
)";

	HANDLE hToken;
	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken);
	TOKEN_PRIVILEGES tp;
	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
	PROCESSENTRY32 pd;
	pd.dwSize = sizeof(pd);
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	BOOL choose = ::Process32First(hProcessSnap, &pd);
	while (choose)
	{
		_bstr_t ExeFile(pd.szExeFile);
		List.push_back(std::make_pair((std::string)ExeFile, pd.th32ProcessID));

		choose = ::Process32Next(hProcessSnap, &pd);
	}
	
	sort(List.begin(), List.end(), cmp);

	for (auto ListItem : List)
	{
		Code += "<tr>\n";
		Code += "<td>" + ListItem.first + "</td>\n";
		Code += "<td>" + Num2String(ListItem.second) + "</td>\n";
		if (HTML.User.GetUserType() != UT_GUEST)
			Code += "<td><a href=\"javascript:void(0);\" onclick=\"Kill(" + Num2String(ListItem.second) + ", \'" + ListItem.first + "\')\">结束</a>&nbsp;&nbsp;&nbsp;<a href=\"javascript:void(0);\" onclick=\"KillAll(" + Num2String(ListItem.second) + ", \'" + ListItem.first + "\')\">结束所有</a></td>\n";
		else
			Code += "<td></td>\n";
		Code += "</tr>\n";
	}

	Code += R"(
</table>
</center>
)";

	AJAXOutput(Code);

	return 0;
}

