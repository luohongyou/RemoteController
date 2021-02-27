// RemoteControllerInfo.cpp : Coral Remote Controller 系统信息 远程控制器信息模块

// 版权声明：
// Copyright (C) 2020-2021 Coral Studio. All Rights Reserved.

#include <iostream>
#include <Windows.h>
#include <string>
#include <fstream>
using namespace std;
int main()
{
    puts("Content-type: text/html\n");
    string Code = R"(
<!DOCTYPE html>
<html lang="zh">
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=gb2312">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>RCInfo - Coral Remote Controller</title>
    <!-- Bootstrap -->
    <link href="/css/bootstrap-4.4.1.css" rel="stylesheet">
  </head>
  <body>
    <br>
    <center>
)";

    Code += "<h4> 版本信息 </h4>\n";
    ifstream fin;
    string CVersion, AGVersion, AIDAVersion;
    fin.open("version");
    getline(fin, CVersion);
    fin.close();
    fin.open("agent\\version");
    getline(fin, AGVersion);
    fin.close();
    fin.open("sysinfo\\AIDA64\\version");
    getline(fin, AIDAVersion);
    fin.close();
    Code += "<div> Coral Remote Controller 版本：" + CVersion + " </div>\n";
    Code += "<div> Coral Remote Agent 版本：" + AGVersion + "</div>\n";
    Code += "<div> 内置 AIDA64 版本：" + AIDAVersion + "</div>\n";
    Code += "<br>";

    Code += "<h4> 关于 </h4>\n";
    Code += "<div> Coral Remote Controller 是基于 Visual C++ 和 IIS CGI-exe 开发的 Windows 远程管理实用程序。 </div>\n";
    Code += "<div> 开发：Coral Studio </div>\n";
    Code += "<div> Copyright (c) 2020 - 2021 Coral Studio. All rights reserved. </div>\n";



    Code += R"(
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
