// printscreen.cpp : Coral Remote Controller 屏幕截图 删除截图模块

// 版权声明：
// Copyright (C) 2020-2021 Coral Studio. All Rights Reserved.

#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>
using namespace std;
ifstream fin;
ofstream fout;
int cnt;
string FileName, FileList[100001];
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
    <title>screenshootdelete - Coral Remote Controller</title>
    <!-- Bootstrap -->
    <link href="/css/bootstrap-4.4.1.css" rel="stylesheet">
  </head>
  <body>
    <script>
)";
    string Code2 = R"(
window.location.href = "/screenshoot/screenshootlist.exe";
    </script>
    <!-- jQuery (necessary for Bootstrap's JavaScript plugins) --> 
    <script src="/js/jquery-3.4.1.min.js"></script> 
    <!-- Include all compiled plugins (below), or include individual files as needed --> 
    <script src="/js/popper.min.js"></script> 
    <script src="/js/bootstrap-4.4.1.js"></script>
  </body>
</html>
)";

    if (argc != 2)
    {
        Code1 += "alert(\'Web Application 调用格式错误！\')\n";
        Code1 += Code2;
        puts(Code1.c_str());
        return 0;
    }
    fin.open("screenshoot\\cache\\list");
    if (fin.fail())
    {
        fin.close();
        Code1 += Code2;
        puts(Code1.c_str());
        return 0;
    }
    fin >> cnt;
    if (!strcmp(argv[1], "all"))
    {
        for (int i = 1; i <= cnt; ++i)
        {
            fin >> FileName;
            remove(("screenshoot\\cache\\" + FileName).c_str());
            remove(("screenshoot\\cache\\jpg\\" + FileName + ".jpg").c_str());
        }
        fin.close();
        remove("screenshoot\\cache\\list");
    }
    else
    {
        bool flag = 0;
        for (int i = 1; i <= cnt; ++i)
        {
            fin >> FileList[i];
            if (FileList[i] == argv[1])
                flag = 1;
        }
        fin.close();
        fout.open("screenshoot\\cache\\list");
        fout << cnt - flag << endl;
        for (int i = 1; i <= cnt; ++i)
        {
            if (FileList[i] == argv[1])
            {
                remove(("screenshoot\\cache\\" + FileList[i]).c_str());
                remove(("screenshoot\\cache\\jpg\\" + FileList[i] + ".jpg").c_str());
            }
            else
                fout << FileList[i] << endl;
        }
    }

    Code1 += Code2;
    puts(Code1.c_str());

    return 0;
}
