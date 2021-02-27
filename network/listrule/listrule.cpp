// listrule.cpp : Coral Remote Controller 网络设置 列出规则模块

// 版权声明：
// Copyright (C) 2020-2021 Coral Studio. All Rights Reserved.

#include <iostream>
#include <Windows.h>
#include <string>
#include <fstream>
#include <sstream>
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
    <title>listrule - Coral Remote Controller</title>
    <!-- Bootstrap -->
    <link href="/css/bootstrap-4.4.1.css" rel="stylesheet">
  </head>
  <body>
    <script>
		function Delete(name) {
			var ans = confirm("确实要删除域名" + name + "的规则吗？");
			if (ans == true) window.location.href = "/network/deleterule.exe?" + name;
		}
    </script>
<center>
<table class="table table-hover">
<tr>
<th>域名</th>
<th>链接的IP地址</th>
<th>操作</th>
</tr>
)";
    char szPath[MAX_PATH];
    GetSystemDirectoryA(szPath, MAX_PATH);
    string Path = szPath, line;
    Path += "\\drivers\\etc\\hosts";
    ifstream fin(Path);
    while (!fin.eof())
    {
        getline(fin, line);
        bool flag = false;
        for (int i = 0; i < line.size(); i++)
            if (line[i] != ' ' && line[i] != '\n' && line[i] != '\0' && line[i] != '\t')
            {
                if (line[i] != '#')
                    flag = true;
                break;
            } 
        if (flag)
        {
            string Domain, IP;
            stringstream tempIO;
            tempIO << line;
            tempIO >> IP >> Domain;
            Code += "<tr>\n";
            Code += "<td>" + Domain + "</td>\n";
            Code += "<td>" + IP + "</td>\n";
            Code += "<td><a href=\"javascript:void(0);\" onclick=\"Delete('" + Domain + "')\">删除</a></td>\n";
            Code += "</tr>\n";
        }
    }
    fin.close();
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
