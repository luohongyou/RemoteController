// datatracker.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define _CRT_SECURE_NO_DEPRECATE

#define ADDRESS "/login/datatracker.exe"

#include <iostream>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\");

    if (argc != 3) // (Hash)Fingerprint, (Hash)Info
        return 0;

    HTMLFrame HTML;
    HTML.Register(PT_EMPTY, ADDRESS, "datatracker", "datatracker", "", false);

    std::string FileName;
    std::string IPAddress = getenv("REMOTE_ADDR");
    if (IPAddress == "::1") IPAddress = "localhost";

    if (HTML.User.IsCloudStorageUser()) // 链接到云盘
        FileName = "Cloud_" + IPAddress + '-' + StandardTime(true);
    else if (HTML.User.GetGUID() != "") // 链接到用户
        FileName = HTML.User.GetName() + '-' + StandardTime(true);
    else                                // 无用户链接
        FileName = "Unknown_" + IPAddress + '-' + StandardTime(true);

    std::string Fingerprint = HexDecode(argv[1]);
    std::string BrowserInfo = HexDecode(argv[2]);

    CreateDirectoryA((DatabasePath() + "Log\\request").c_str(), NULL);
    CreateDirectoryA((DatabasePath() + "Log\\request\\latest").c_str(), NULL);

    std::ofstream fout(DatabasePath() + "Log\\request\\" + FileName + ".log");
    fout << Fingerprint << std::endl;
    fout << BrowserInfo;
    fout.close();

    if (HTML.User.GetGUID() != "" && !HTML.User.IsCloudStorageUser())
        CopyFileA((DatabasePath() + "Log\\request\\" + FileName + ".log").c_str(), 
            (DatabasePath() + "Log\\request\\latest\\" + HTML.User.GetGUID() + ".log").c_str(), false);

    puts("Content-type: text/html\n");
    printf("true");

    return 0;
}
