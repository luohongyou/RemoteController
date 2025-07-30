// filedownload.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/explorer/filedownload.exe"

#include <iostream>
#include <Windows.h>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
	SetAppDirectory("\\..\\..\\");

	if (argc != 3)
		return 0;

	std::string Address = UTF8ToANSI(decodeURI(HexDecode(argv[1])));
	bool IsFolder = (!strcmp(argv[2], "1"));
	int IsCloudStorage = (Address.substr(0, 8) == "共享云盘");

	HTMLFrame HTML;
	HTML.Register(PT_EMPTY, ADDRESS, "filedownload", "filedownload", "", true);

	if (!IsCloudStorage && HTML.User.IsCloudStorageUser())
	{
		HTML.Log("用户没有权限下载远程计算机的本地文件", "explorer", LL_ERROR);
		printf("No");
		return 0;
	}
	if (IsCloudStorage)
		Address = "storage\\sharedzone" + Address.substr(Address.find_first_of('\\'));

	std::string LocalFileName, TempFolderName;

	GUID guid;
	CoCreateGuid(&guid);
	std::string GUID = GuidToString(guid);
	LocalFileName = "temp\\download\\" + GUID + "\\FileDownloading.down";
	TempFolderName = "temp\\download\\" + GUID + "\\";
	CreateDirectoryA(TempFolderName.c_str(), NULL);

	bool ans = 0;
	if (!IsFolder)
		ans = CopyFileA(Address.c_str(), LocalFileName.c_str(), false);
	else
	{
		WinExecAndWait32("..\\..\\ManageUI\\7zip\\7z.exe", ("a \"" + LocalFileName + ".zip\" \"" + Address + "\"").c_str(), "", INFINITE);
		ans = !rename((LocalFileName + ".zip").c_str(), LocalFileName.c_str());
	}

	if (!ans)
	{
		AJAXOutput(HexEncode(encodeURI("Failed")));
		HTML.Log("尝试远程下载文件失败<br>请求的文件：" + Address, "explorer", LL_ERROR);
		return 0;
	}

	// HTMLFileName = "/temp/download/" + GUID + "/FileDownloading";

	AJAXOutput(HexEncode(encodeURI(GUID.c_str())));
	HTML.Log("已请求远程下载文件<br>请求的文件：" + Address + "<br>GUID：" + GUID, "explorer", LL_INFO);

	return 0;
}
