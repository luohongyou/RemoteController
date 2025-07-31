// logdisplayer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/manage/main/log/logdisplayer.exe"

#include <iostream>
#include <vector>
#include "../../../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../../../x64/Release/HTMLFrame.lib")

struct _Log
{
	std::string Time, User, Content, Module;
	int Level;
};
std::vector<_Log> LogList;
int main(int argc, char* argv[])
{
	SetAppDirectory("\\..\\..\\..\\..\\");

	if (argc != 2)
		return 0;

	int LogType = String2Num(argv[1]);

	HTMLFrame HTML;
	HTML.Register(PT_EMPTY, ADDRESS, "logdisplayer", "logdisplayer", "", true);
	HTML.ManageInit();

	std::ifstream fin;
	std::string Code, temp;
	std::stringstream tempIO;
	_Log log;

	if (LogType == 1)        // 基本日志
	{
		fin.open(DatabasePath() + "Log\\main.log");
		if (!fin.fail())
			while (!fin.eof())
			{
				try
				{
					fin >> log.Time >> temp >> log.User >> log.Module >> log.Level;
					fin.get();
					if (fin.eof()) break;
					if (fin.bad()) break;
					getline(fin, log.Content);
					log.Time += " " + temp;
					if (fin.bad()) break;
				}
				catch (std::exception& e)
				{
					// TODO: Error
					break;
				}
				LogList.push_back(log);
			}
		fin.close();

		Code = R"(
<table class="table table-hover table-sm">
<tr>
<th>类型</th>
<th>时间</th>
<th>用户</th>
<th>模块</th>
<th>内容</th>
</tr>
)";
		for (int i = LogList.size() - 1; i >= 0; --i)
		{
			Code += "<tr>\n";

			Code += "<td>";
			switch (LogList[i].Level)
			{
			case LL_INFO:
				Code += R"(<span class="badge text-bg-primary">信息</span>)";
				break;
			case LL_WARNING:
				Code += R"(<span class="badge text-bg-warning">警告</span>)";
				break;
			case LL_ERROR:
				Code += R"(<span class="badge text-bg-danger">错误</span>)";
				break;
			case LL_DEBUG:
				Code += R"(<span class="badge text-bg-success">调试</span>)";
				break;
			}
			Code += "</td>\n";

			Code += "<td>" + LogList[i].Time + "</td>\n";
			Code += "<td>" + LogList[i].User + "</td>\n";
			Code += "<td>" + LogList[i].Module + "</td>\n";
			Code += "<td>" + LogList[i].Content + "</td>\n";
			Code += "</tr>\n";
		}
		Code += "</table>";

	}
	else if (LogType == 2)   // Website 日志
	{
		fin.open(DatabasePath() + "Log\\full.log");
		if (!fin.fail())
			while (!fin.eof())
			{
				try
				{
					getline(fin, temp);
					tempIO << temp;
					tempIO >> log.Time >> temp >> log.User >> log.Content;
					tempIO.clear();
					log.Time += " " + temp;
				}
				catch (std::exception& e)
				{
					// TODO: Error
					break;
				}
				LogList.push_back(log);
			}
		fin.close();

		Code = R"(
<table class="table table-hover table-sm">
<tr>
<th>时间</th>
<th>用户</th>
<th>访问的页面</th>
</tr>
)";
		for (int i = LogList.size() - 1; i >= 0; --i)
		{
			Code += "<tr>\n";
			Code += "<td>" + LogList[i].Time + "</td>\n";
			Code += "<td>" + LogList[i].User + "</td>\n";
			Code += "<td>" + LogList[i].Content + "</td>\n";
			Code += "</tr>\n";
		}
		Code += "</table>";
	}
	else if (LogType == 3)   // Agent 日志
	{
		std::string LogFile = RCAgent("getlogfile", HTML.User.GetName(), "");
		fin.open(LogFile);
		if (!fin.fail())
			while (!fin.eof())
			{
				getline(fin, temp);
				if (temp.size() < 5 || temp[0] == '=')
					continue;
				if (temp.substr(0, 5) == "Coral")
				{
					log.Content = temp.substr(33, temp.find('(') - 34);
					getline(fin, temp);
					log.Content = "Remote Agent 开始运行，版本 " + log.Content + " (" + temp.substr(28) + ")";
					getline(fin, temp);
					log.Time = temp.substr(21);
					log.User = "SYSTEM";
					LogList.push_back(log);
					continue;
				}

				log.Time = temp.substr(0, 20);
				log.Content = temp.substr(27, temp.find(';') - 27);
				log.User = temp.substr(temp.find(';') + 7);
				LogList.push_back(log);
			}
		fin.close();

		Code = R"(
<table class="table table-hover table-sm">
<tr>
<th>时间</th>
<th>用户</th>
<th>操作</th>
</tr>
)";
		for (int i = LogList.size() - 1; i >= 0; --i)
		{
			Code += "<tr>\n";
			Code += "<td>" + LogList[i].Time + "</td>\n";
			Code += "<td>" + LogList[i].User + "</td>\n";
			Code += "<td>" + LogList[i].Content + "</td>\n";
			Code += "</tr>\n";
		}
		Code += "</table>";
	}

	AJAXOutput(Code);

	return 0;
}

