// quicklook.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/explorer/quicklook.exe"

#include <iostream>
#include <Windows.h>
#include <locale>
#include <boost\locale.hpp>
#include <boost/locale/encoding.hpp>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

#define T_IMAGE 1
#define T_VIDEO 2
#define T_AUDIO 3
#define T_PDF   5
#define T_DOCX  6
#define T_TEXT  7

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
	else if (ext == "txt" || ext == "log")
		extType = T_TEXT;     // 文本
	else if (ext == "pdf")
		extType = T_PDF;      // PDF文档
	else if (ext == "docx")
		extType = T_DOCX;     // Word文档
	// 文本和源代码: 5KB

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
	else if (extType == T_TEXT)
	{
		std::ifstream fin(Address, std::ios::binary);
		if (!fin.is_open())
		{
			AJAXOutput("加载文件预览失败");
			HTML.Log("尝试远程预览文件失败<br>请求的文件：" + Address, "explorer", LL_ERROR);
			return 0;
		}

		bool isZipped;

		const size_t maxSize = 1024 * 1024; // 1MB
		std::vector<char> buffer(maxSize);
		fin.read(buffer.data(), maxSize);
		size_t readSize = fin.gcount();
		isZipped = !fin.eof();
		fin.close();
		buffer.resize(readSize);

		std::string Code;
		std::string Content(buffer.begin(), buffer.end());

		if (!IsUTF8(Content.c_str(), Content.size()))
		{
			try {
				std::string utf8_content = boost::locale::conv::between(Content, "UTF_8", "GB2312");
				Content = utf8_content;
			}
			catch (const boost::locale::conv::conversion_error& e) {
				AJAXOutput("加载文件预览失败");
				HTML.Log("尝试远程预览文件失败<br>请求的文件：" + Address, "explorer", LL_ERROR);
				return 0;
			}
		}

		size_t pos = 0;
		while ((pos = Content.find("\r\n", pos)) != std::string::npos) {
			Content.replace(pos, 2, "\n");
			pos += 1;
		}

		if (isZipped)
			Code = R"(<span class="badge text-bg-danger">文件过大，此处仅展示前1MB内容</span>)";
		Code += R"(<textarea class="form-control" style="height:300px;" readonly>)" + Content + R"(</textarea>)";

		puts("Content-type: text/html; charset=UTF-8\n");
		puts(Code.c_str());
	}
	// else if (...)
	else // TODO
	{
		/*
		AJAXOutput((std::string)R"(
<div align="center">
	<h2>测试文件.file</h2>
	<p><b>文件类型：</b>FILE文件</p>
	<p><b>文件大小：</b>4 KB</p>
	<p><b>修改时间：</b>2024-01-01 12:00:00</p>
	<p><b>文件路径：</b>)" + Address + R"(</p>
</div>
)");*/

		AJAXOutput("暂不支持预览该文件");
	}

	return 0;
}