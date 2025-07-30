// changewallpaper.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/power/changewallpaper.exe"

#include <iostream>
#include <string>
#include <io.h>
#include <fcntl.h>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\");

    _setmode(_fileno(stdin), _O_BINARY);

    HTMLFrame HTML;
    HTML.Register(PT_EMPTY, ADDRESS, "changewallpaper", "changewallpaper", "", true);

    if (HTML.User.GetUserType() == UT_GUEST)
        return 0;

    puts("Content-type: text/html\n");

    cgicc::const_file_iterator file = HTML.cgi.getFile("file");
    if (file != HTML.cgi.getFiles().end())
    {
        std::string FileName, Ext = "";
        FileName = UTF8ToANSI(file->getFilename());

        for (int i = FileName.size() - 1; i >= 0; i--)
            if (FileName[i] == '.')
            {
                Ext = "." + FileName.substr(i + 1);
                break;
            }

        FileName = "upload" + Ext;

        std::ofstream fout("temp\\wallpaper\\" + FileName, std::ios::binary);
        file->writeToStream(fout);
        fout.close();

        RCAgent("setwallpaper", HTML.User.GetName(), FileName);

        printf("Yes");
        HTML.Log("成功修改了远程计算机的壁纸", "power", LL_INFO);
    }
    else
        printf("No");

    return 0;

}
