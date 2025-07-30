// batchfilehandler.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/command/batchfilehandler.exe"

#include <iostream>
#include <Windows.h>
#include <cstdio>
#include <io.h>
#include <fcntl.h>
#include <filesystem>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
	SetAppDirectory("\\..\\..\\");

	_setmode(_fileno(stdin), _O_BINARY);

	HTMLFrame HTML;
	HTML.Register(PT_EMPTY, ADDRESS, "batchfilehandler", "batchfilehandler", "", true);

	if (HTML.User.GetUserType() == UT_GUEST)
	{
		HTML.Log("用户没有权限执行自定义脚本", "command", LL_ERROR);
		return 0;
	}

	if (argc != 3)
		return 0;

	int Auth = String2Num(argv[1]);
	int Inter = String2Num(argv[2]);

	cgicc::const_file_iterator file = HTML.cgi.getFile("file");

	GUID guid;
	CoCreateGuid(&guid);
	std::string _GUID = GuidToString(guid);

	std::string result;
	if (file != HTML.cgi.getFiles().end())
	{
		CreateDirectoryA(("temp\\data\\" + _GUID + "\\").c_str(), NULL);
		std::ofstream fout("temp\\data\\" + _GUID + "\\UserScript.bat");

		fout << "@echo off\n";
		char _str[MAX_PATH], SystemC;
		GetSystemDirectoryA(_str, MAX_PATH);
		SystemC = _str[0];
		fout << SystemC << ":\n";
		fout << "cd C:\\Windows\\System32\n";
		// fout << "@echo on\n";

		std::stringstream ScriptData;
		file->writeToStream(ScriptData);
		fout << UTF8ToANSI(ScriptData.str());
		fout << std::endl;
		fout.close();

		if (Auth == 2) // Service
		{
			FILE* pipe = _popen(("temp\\data\\" + _GUID + "\\UserScript.bat").c_str(), "r");
			if (!pipe)
			{
				result = "执行命令时出现错误。";
				HTML.Log("请求执行自定义脚本时出现错误<br>模式：系统服务<br>GUID：" + _GUID, "command", LL_ERROR);
			}
			else
			{
				char buffer[500];
				while (fgets(buffer, sizeof(buffer), pipe))
					result += buffer;
				_pclose(pipe);

				fout.open("temp\\data\\" + _GUID + "\\result.log");
				fout << result;
				fout.close();
			}

			HTML.Log("已请求执行自定义脚本<br>模式：系统服务<br>GUID：" + _GUID, "command", LL_INFO);
		}
		else
		{
			RCAgent("runbatchfile", HTML.User.GetName(), Num2String(Inter) + " " + _GUID);
			HTML.Log("已请求执行自定义脚本<br>模式：前台用户 " + (std::string)(Inter ? "允许交互" : "后台运行") + "<br>GUID：" + _GUID, "command", LL_INFO);

			if (!Inter)
			{
				while (!std::filesystem::exists("temp\\data\\" + _GUID + "\\result.log"))
					Sleep(200);

				std::ifstream fin;
				std::string temp;
				fin.open("temp\\data\\" + _GUID + "\\result.log");
				if (!fin.fail())
				{
					while (!fin.eof())
					{
						getline(fin, temp);
						result += temp + "\n";
					}
				}
				fin.close();
			}
			else
				result = "该模式不支持查看控制台输出结果";
		}
	}

	AJAXOutput(result);

	return 0;
}
