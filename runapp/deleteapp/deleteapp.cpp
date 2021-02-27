// deleteapp.cpp : Coral Remote Controller 执行文件 删除文件模块

// 版权声明：
// Copyright (C) 2020-2021 Coral Studio. All Rights Reserved.

#include <iostream>
#include <fstream>
#include <Windows.h>
using namespace std;
int main(int argc, char* argv[])
{
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
window.location.href = "/runapp/listapp.exe";
    </script>
    <!-- jQuery (necessary for Bootstrap's JavaScript plugins) --> 
    <script src="/js/jquery-3.4.1.min.js"></script> 
    <!-- Include all compiled plugins (below), or include individual files as needed --> 
    <script src="/js/popper.min.js"></script> 
    <script src="/js/bootstrap-4.4.1.js"></script>
  </body>
</html>
)";
    if (argc < 2)
    {
        Code1 += "alert(\'Web Application 调用格式错误！\')\n";
        Code1 += Code2;
        puts(Code1.c_str());
        return 0;
    }

    string Command = "";
    if (argc == 2 && !strcmp(argv[1], "all"))
    {
        char strModule[256];
        GetModuleFileNameA(NULL, strModule, 256);
        std::string dir;
        dir.assign(strModule);
        dir.append("\\..\\upload\\");
        dir = dir.substr(4);

        char szFind[100];
        char szFile[100];
        WIN32_FIND_DATAA FindFileData;
        strcpy_s(szFind, dir.c_str());
        strcat_s(szFind, "*.*");
        HANDLE hFind = ::FindFirstFileA(szFind, &FindFileData);
        if (INVALID_HANDLE_VALUE != hFind)
        {
            while (true)
            {
                if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                {
                    string Name = "runapp\\upload\\" + (string)FindFileData.cFileName;
                    remove(Name.c_str());
                }
                if (!FindNextFileA(hFind, &FindFileData))
                    break;
            }
            FindClose(hFind);
        }
    }
    else
    {
        for (int i = 1; i < argc; i++)
            Command += argv[i], Command += " ";
        remove(("runapp\\upload\\" + Command).c_str());
    }

    Code1 += Code2;
    puts(Code1.c_str());
    return 0;
}
