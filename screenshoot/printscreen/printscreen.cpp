// printscreen.cpp : Coral Remote Controller 屏幕截图 截获屏幕模块

// 版权声明：
// Copyright (C) 2020-2021 Coral Studio. All Rights Reserved.

#include <iostream>
#include <Windows.h>
#include <fstream>
#include <sstream>
#include <string>
#include "bmp2jpg.h"
using namespace std;

ifstream fin;
ofstream fout;
int cnt;
string FileList[100001];

int main()
{
    puts("Content-type: text/html\n");

    fout.open("temp\\action\\printscreen");
    fout.close();

    Sleep(500);

    string FileName;
    fin.open("temp\\action\\printscreen.success");
    fin >> FileName;
    fin.close();
    remove("temp\\action\\printscreen.success");

    if (FileName == "")
        return 0;

    CBmp2Jpeg bmp;
    bmp.Bmp2Jpeg(("screenshoot\\cache\\" + FileName).c_str(), ("screenshoot\\cache\\jpg\\" + FileName + ".jpg").c_str());

    fin.open("screenshoot\\cache\\list");
    if (fin.fail())
        cnt = 0;
    else
    {
        fin >> cnt;
        for (int i = 1; i <= cnt; i++)
        {
            fin >> FileList[i];
            if (FileList[i] == FileList[i - 1])
                i--, cnt--;
        }
    }
    fin.close();

    fout.open("screenshoot\\cache\\list");
    if (FileList[1] == FileName)
        fout << cnt << endl;
    else
    {
        fout << cnt + 1 << endl;
        fout << FileName << endl;
    }
    for (int i = 1; i <= cnt; i++)
        fout << FileList[i] << endl;
    fout.close();

    string Code = R"(
<!DOCTYPE html>
<html lang="zh">
  <head>
    <meta charset="utf-8">
    <title>printscreen - Coral Remote Controller</title>
  </head>
  <body>
    <center>
      <img src="\screenshoot\cache\)" 
+ FileName +
R"(" height="100%" width="100%"/>
    </center>
  </body>
</html>
)";
    puts(Code.c_str());
    return 0;
}

