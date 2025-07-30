// listwindows.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/windowmgr/listwindows.exe"

#include <iostream>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
	SetAppDirectory("\\..\\..\\");

	HTMLFrame HTML;
	HTML.Register(PT_EMPTY, ADDRESS, "listwindows", "listwindows", "", true);

	std::string Code = R"(
<center>
<table class="table">
<tr>
<th>标题</th>
<th>进程ID</th>
<th>操作</th>
</tr>
)";

	int cnt;
	std::string ans, Title, WindowID, PID;

	ans = RCAgent("listwindows", HTML.User.GetName(), "");

	if (ans == "CORAL_RC_AGENT_FAILED")
	{
		AJAXOutput("Failed!");
		return 0;
	}

	std::stringstream tempIO;
	tempIO << ans;

	tempIO >> cnt;
	tempIO.get();
	for (register int i = 1; i <= cnt; ++i)
	{
		getline(tempIO, Title);
		getline(tempIO, WindowID);
		getline(tempIO, PID);
		Code += "<tr>\n";
		Code += "<td>" + Title + "</td>\n";
		Code += "<td>" + PID + "</td>\n";
		if (HTML.User.GetUserType() != UT_GUEST)
			Code += "<td><a href=\"javascript:void(0);\" onclick=\"Close(" + WindowID + ", \'" + Title + "\')\">关闭</a>&nbsp;&nbsp;&nbsp;<a href=\"javascript:void(0);\" onclick=\"ForceClose(" + WindowID + ", \'" + Title + "\')\">强行关闭</a></td>\n";
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

