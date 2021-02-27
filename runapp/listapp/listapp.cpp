// listapp.cpp : Coral Remote Controller 执行文件 列出文件模块

// 版权声明：
// Copyright (C) 2020-2021 Coral Studio. All Rights Reserved.

#include <iostream>
#include <string>
#include <windows.h>
using namespace std;

int main()
{
    puts("Content-type: text/html");
    string Code = R"(
<!DOCTYPE html>
<html lang="zh">
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=gb2312">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>listapp - Coral Remote Controller</title>
    <!-- Bootstrap -->
    <link href="/css/bootstrap-4.4.1.css" rel="stylesheet">
  </head>
  <body>
    <script>
		function Run(name) {
			var ans = confirm("确实要在远程计算机前台打开" + name + "吗？");
			if (ans == true)window.location.href = "/runapp/run.exe?" + name;
		}
		function Delete(name) {
			var ans = confirm("确实要删除" + name + "吗？");
			if (ans == true) window.location.href = "/runapp/deleteapp.exe?" + name;
		}
    </script>
<center>
<table class="table table-hover">
<tr>
<th>文件名</th>
<th>操作</th>
</tr>
)";

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
                Code += "<tr>\n";
                Code += "<td>" + (string)FindFileData.cFileName + "</td>\n";
                Code += "<td><a href=\"javascript:void(0);\" onclick=\"Run('" + (string)FindFileData.cFileName + "')\">运行</a>&nbsp;&nbsp;&nbsp;<a href=\"javascript:void(0);\" onclick=\"Delete('" + (string)FindFileData.cFileName + "')\">删除</a></td>\n";
                Code += "</tr>\n";
            }
            if (!FindNextFileA(hFind, &FindFileData))
                break;
        }
        FindClose(hFind);
    }
    Code += R"(
</table>
</center>
    <!-- jQuery (necessary for Bootstrap's JavaScript plugins) --> 
    <script src="/js/jquery-3.4.1.min.js"></script> 
    <!-- Include all compiled plugins (below), or include individual files as needed --> 
    <script src="/js/popper.min.js"></script> 
    <script src="/js/bootstrap-4.4.1.js"></script>
  </body>
</html>
)";
    puts(Code.c_str());
    return 0;
}
