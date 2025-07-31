﻿// quicklook.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/explorer/quicklook.exe"

#include <iostream>
#include <Windows.h>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

#define T_IMAGE 1
#define T_VIDEO 2
#define T_AUDIO 3
#define T_PDF   5
#define T_DOCX  6

int main(int argc, char* argv[])
{
	SetAppDirectory("\\..\\..\\");

	if (argc != 2)
		return 0;

	std::string Address = UTF8ToANSI(decodeURI(HexDecode(argv[1])));
	int IsCloudStorage = (Address.substr(0, 8) == "共享云盘");

	HTMLFrame HTML;
	HTML.Register(PT_EMPTY, ADDRESS, "filedownload", "filedownload", "", true);

	if (!IsCloudStorage && HTML.User.IsCloudStorageUser())
	{
		HTML.Log("用户没有权限访问远程计算机本地文件", "explorer", LL_ERROR);
		printf("No");
		return 0;
	}
	if (IsCloudStorage)
		Address = "storage\\sharedzone" + Address.substr(Address.find_first_of('\\'));

	size_t __ext = Address.find_last_of(".");
	std::string ext;
	if (__ext != std::string::npos)
	{
		ext = Address.substr(__ext + 1);
		for (auto& c : ext)
			c = tolower(c);
	}
	else
		ext = "";
	int extType = 0;

	if (ext == "bmp" || ext == "jpg" || ext == "jpeg" || ext == "jfif" ||
		ext == "gif" || ext == "png" || ext == "webp" || ext == "svg")
		extType = T_IMAGE;    // 图片
	else if (ext == "avi" || ext == "wmv" || ext == "mpg" || ext == "mpeg" ||
		ext == "ts" || ext == "mp4" || ext == "mkv" || ext == "flv" || ext == "mov")
		extType = T_VIDEO;    // 视频
	else if (ext == "wav" || ext == "mp3" || ext == "m4a" || ext == "flac")
		extType = T_AUDIO;    // 音频
	// else if (ext == ...) // 文本和源代码: 5KB
	else if (ext == "pdf")
		extType = T_PDF;      // PDF文档
	else if (ext == "docx")
		extType = T_DOCX;     // Word文档


	if (extType == T_IMAGE || extType == T_VIDEO || extType == T_PDF || extType == T_DOCX)
	{
		std::string LocalFileName, TempFolderName;

		bool ans = 0;
		if (IsCloudStorage)
		{
			ans = 1;
			LocalFileName = Address;
		}
		else
		{
			GUID guid;
			CoCreateGuid(&guid);
			std::string GUID = GuidToString(guid);
			LocalFileName = "temp\\download\\" + GUID + "\\tmp." + ext;
			TempFolderName = "temp\\download\\" + GUID + "\\";

			CreateDirectoryA(TempFolderName.c_str(), NULL);
			ans = CopyFileA(Address.c_str(), LocalFileName.c_str(), false);
		}

		if (!ans)
		{
			AJAXOutput("加载文件预览失败");
			HTML.Log("尝试远程预览文件失败<br>请求的文件：" + Address, "explorer", LL_ERROR);
			return 0;
		}

		if (extType == T_VIDEO)
			AJAXOutput((std::string)"<video src=\"/" + LocalFileName + "\" width=\"100%\" controls></video>");
		else if (extType == T_IMAGE)
			AJAXOutput((std::string)"<img src=\"/" + LocalFileName + "\" width=\"100%\"></img>");
		else if (extType == T_PDF)
			AJAXOutput((std::string)"<!-- UseIframe -->/assets/vendor/previewhandler/pdfjs/viewer.html?file=/" + LocalFileName);
		else if (extType == T_DOCX)
			AJAXOutput((std::string)"<!-- UseIframe -->/assets/vendor/previewhandler/docxjs/viewer_v2.html?file=/" + LocalFileName);

	}
	// else if (...)
	else
	{
		AJAXOutput("暂不支持预览该文件");
	}

	return 0;
}
