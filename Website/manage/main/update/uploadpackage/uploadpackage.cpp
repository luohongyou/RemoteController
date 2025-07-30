// uploadpackage.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/manage/main/update/uploadpackage.exe"

#include <iostream>
#include <string>
#include <io.h>
#include <fcntl.h>
#include "../../../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\..\\..\\");

    _setmode(_fileno(stdin), _O_BINARY);

    HTMLFrame HTML;
    HTML.Register(PT_EMPTY, ADDRESS, "uploadpackage", "uploadpackage", "", true);
    HTML.ManageInit();
    if (HTML.User.GetUserType() != UT_ROOT)
        return 0;

    cgicc::const_file_iterator file = HTML.cgi.getFile("file");
    if (file != HTML.cgi.getFiles().end()) 
    {
        RemoveDir("..\\..\\ManageUI\\update\\packages");
        CreateDirectoryA("..\\..\\ManageUI\\update\\packages", NULL);

        std::ofstream fout("..\\..\\ManageUI\\update\\packages\\upload.zip", std::ios::binary);
        file->writeToStream(fout);
        fout.close();

        WinExecAndWait32("..\\..\\ManageUI\\7zip\\7z.exe", "x \"..\\..\\ManageUI\\update\\packages\\upload.zip\" -o..\\..\\ManageUI\\update\\packages", "", INFINITE);
        remove("..\\..\\ManageUI\\update\\packages\\upload.zip");

        CopyFileA("..\\..\\ManageUI\\update\\packages\\Coral Remote Controller\\ManageUI\\update\\rcupdater.exe", "temp\\rcupdater.exe", false);
        CopyFileA("..\\..\\ManageUI\\update\\packages\\Coral Remote Controller\\ManageUI\\agent\\agentupdater.exe", "temp\\agentupdater.exe", false);
        remove("temp\\Updated.ini");
    }

    HTML.Custom("<script> window.location.href = \"/manage/main/update\"; </script>");
    HTML.Output();
    return 0;

}
