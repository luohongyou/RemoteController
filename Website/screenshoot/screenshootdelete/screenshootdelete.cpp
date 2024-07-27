// screenshootdelete.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2024 Coral Studio. All Rights Reserved.

#define ADDRESS "/screenshoot/screenshootdelete.exe"

#include <iostream>
#include <vector>
#include <Windows.h>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

std::ifstream fin;
std::ofstream fout;
int cnt;
std::string FileName;
std::vector<std::string> FileList;

int main(int argc, char* argv[])
{
	SetAppDirectory("\\..\\..\\");

	if (argc != 2)
		return 0;

	std::string _FileName = argv[1];

	HTMLFrame HTML;
	HTML.Register(PT_EMPTY, ADDRESS, "screenshootdelete", "screenshootdelete", "", true);

	fin.open(HTML.User.Data("screenshootlist"));
	if (fin.fail())
	{
		fin.close();
		AJAXOutput("Yes");
		return 0;
	}

	fin >> cnt;

	if (_FileName == "all")
	{
		for (int i = 1; i <= cnt; ++i)
		{
			fin >> FileName;
			remove(("screenshoot\\cache\\" + FileName + ".jpg").c_str());
		}
		fin.close();
		remove(HTML.User.Data("screenshootlist").c_str());
	}
	else
	{
		for (int i = 1; i <= cnt; ++i)
		{
			fin >> FileName;

			if (FileName + ".jpg" == _FileName)
				remove(("screenshoot\\cache\\" + FileName + ".jpg").c_str());
			else
				FileList.push_back(FileName);
		}
		fin.close();

		fout.open(HTML.User.Data("screenshootlist"));
		fout << FileList.size() << std::endl;
		for (auto FileItem : FileList)
			fout << FileItem << std::endl;
	}

	AJAXOutput("Yes");

	return 0;
}
