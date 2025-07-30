// expuploadhandler.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/explorer/expuploadhandler.exe"

#include <iostream>
#include <Windows.h>
#include <filesystem>
#include <io.h>
#include <fcntl.h>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
	SetAppDirectory("\\..\\..\\");

	_setmode(_fileno(stdin), _O_BINARY);

	HTMLFrame HTML;
	HTML.Register(PT_EMPTY, ADDRESS, "expuploadhandler", "expuploadhandler", "", true);
	if (HTML.User.GetUserType() == UT_GUEST)
	{
		HTML.Log("用户没有权限操作远程计算机上的文件", "explorer", LL_ERROR);
		return 0;
	}

	if (argc != 2)
		return 0;

	std::string Address = UTF8ToANSI(decodeURI(HexDecode(argv[1])));
	if (!ExpCheckAddress(Address))
	{
		HTML.Log("用户尝试操作非法路径", "explorer", LL_ERROR);
		return 0;
	}

	int IsCloudStorage = (Address.substr(0, 8) == "共享云盘");
	if (!IsCloudStorage && HTML.User.IsCloudStorageUser())
	{
		HTML.Log("用户尝试访问不存在的路径", "explorer", LL_ERROR);
		puts("Content-type: text/html\n");
		printf("No");
		return 0;
	}
	
	for (int i = 0; i < Address.size(); i++)
		if (Address[i] == '\n')
		{
			Address.erase(i);
			i--;
		}

	if (Address[Address.size() - 1] != '\\' && Address[Address.size() - 1] != '/')
        Address += "\\";

	if (IsCloudStorage)
		Address = "storage\\sharedzone" + Address.substr(Address.find_first_of('\\'));

	if (!fs::exists(Address))
	{
		HTML.Log("用户尝试访问不存在的路径", "explorer", LL_ERROR);
		printf("No");
		return 0;
	}

	puts("Content-type: text/html\n");

	cgicc::const_file_iterator file = HTML.cgi.getFile("file");
	std::string FileName = UTF8ToANSI(file->getFilename().c_str());

	int cnt = 0;
	std::string Ext, DisplayName;
	bool flag = 0;
	for (int i = FileName.size() - 1; i >= 0; i--)
		if (FileName[i] == '.')
		{
			Ext = "." + FileName.substr(i + 1);
			DisplayName = FileName.substr(0, i);
			flag = 1;
			break;
		}
	if (!flag)
	{
		Ext = "";
		DisplayName = FileName;
	}

	FileName = Address + FileName;
	while (std::filesystem::exists(FileName))
		FileName = Address + DisplayName + " (" + Num2String(++cnt) + ")" + Ext;

	bool Result = false;
    if (file != HTML.cgi.getFiles().end())
    {
        std::ofstream fout(FileName, std::ios::binary);
        file->writeToStream(fout);
        fout.close();

		std::ifstream fin(FileName, std::ios::binary);
		Result = (!fin.fail());
		fin.close();
    }

	if (Result)
	{
		HTML.Log("已请求远程上传文件<br>保存的文件：" + FileName, "explorer", LL_INFO);
		printf("Yes");
	}
	else
	{
		HTML.Log("远程上传文件失败<br>请求的文件：" + Address + file->getFilename(), "explorer", LL_ERROR);
		printf("No");
	}

	return 0;
}
