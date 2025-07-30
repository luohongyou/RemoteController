// printscreen.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/screenshot/printscreen.exe"

#include <iostream>
#include <vector>
#include <Windows.h>
#include "bmp2jpg.h"
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

int cnt;
std::vector<std::string> FileList;

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\");

    if (argc != 2)
        return 0;

    HTMLFrame HTML;
    HTML.Register(PT_EMPTY, ADDRESS, "printscreen", "printscreen", "", true);

    std::string FileName, ShortFileName;
    std::ifstream fin;
    std::ofstream fout;
    
    // ShootScreen(FileName.c_str(), NULL);

    ShortFileName = RCAgent("printscreen", HTML.User.GetName(), argv[1]);

    if (ShortFileName == "CORAL_RC_AGENT_FAILED")
    {
        AJAXOutput("Failed!");
        return 0;
    }

    FileName = "storage\\screenshot\\" + ShortFileName;

    CBmp2Jpeg bmp;
    bmp.Bmp2Jpeg(FileName.c_str(), (FileName + ".jpg").c_str());

    remove(FileName.c_str());

    FileList.push_back(ShortFileName);
    std::string tempFileName;
    fin.open(HTML.User.Data("screenshotlist"));
    if (fin.fail())
        cnt = 0;
    else
    {
        fin >> cnt;
        for (int i = 1; i <= cnt; i++)
        {
            fin >> tempFileName;
            if (tempFileName != FileList.back())
                FileList.push_back(tempFileName);
        }
    }
    fin.close();

    fout.open(HTML.User.Data("screenshotlist"));
    fout << FileList.size() << std::endl;
    for (auto ListItem : FileList)
        fout << ListItem << std::endl;
    fout.close();

    AJAXOutput(R"(
<center>
<img src="\storage\screenshot\)"
+ ShortFileName +
R"(.jpg" width="100%"/>
</center>
)");
    HTML.Log("成功截取了屏幕图像到 " + ShortFileName + ".jpg", "screenshot", LL_INFO);

    return 0;
}