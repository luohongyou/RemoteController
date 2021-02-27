// windowkiller.cpp : Coral Remote Controller 窗口管理 关闭窗口模块

// 版权声明：
// Copyright (C) 2020-2021 Coral Studio. All Rights Reserved.

// 调用方式：windowkiller.exe HWND_ID [force]

#include <iostream>
#include <Windows.h>
#include <tlhelp32.h>
#include <comdef.h>
#include <sstream>
#include <fstream>
using namespace std;

bool IsForce;
ifstream fin;
ofstream fout;
int main(int argc, char* argv[])
{
    // 输出HTML
    puts("Content-type: text/html\n");
    string Code1 = R"(
<!DOCTYPE html>
<html lang="zh">
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=gb2312">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>windowkiller - Coral Remote Controller</title>
    <!-- Bootstrap -->
    <link href="/css/bootstrap-4.4.1.css" rel="stylesheet">
  </head>
  <body>
    <script>
)";
    string Code2 = R"(
window.location.href = "/windowmgr/enumwindows.exe";
    </script>
    <!-- jQuery (necessary for Bootstrap's JavaScript plugins) --> 
    <script src="/js/jquery-3.4.1.min.js"></script> 
    <!-- Include all compiled plugins (below), or include individual files as needed --> 
    <script src="/js/popper.min.js"></script> 
    <script src="/js/bootstrap-4.4.1.js"></script>
  </body>
</html>
)";

    // 检查参数
    if (argc == 3)
    {
        if (strcmp(argv[2], "force"))
        {
            Code1 += "alert(\'Web Application 调用格式错误！\')\n";
            Code1 += Code2;
            puts(Code1.c_str());
            return 0;
        }
        IsForce = 1;
    }
    else if (argc != 2)
    {
        Code1 += "alert(\'Web Application 调用格式错误！\')\n";
        Code1 += Code2;
        puts(Code1.c_str());
        return 0;
    }

    fout.open("temp\\action\\closewindow");
    fout << argv[1] << endl << IsForce;
    fout.close();

    Sleep(500);

    fin.open("temp\\action\\closewindow.success");
    if (fin.fail())
    {
        Code1 += "alert(\'关闭窗口失败！\')\n";
        Code1 += Code2;
        puts(Code1.c_str());
        return 0;
    }
    fin.close();
    remove("temp\\action\\closewindow.success");
    Code1 += Code2;
    puts(Code1.c_str());
    return 0;
}

