// SystemInfo.cpp : Coral Remote Controller 系统信息 软件信息模块

// 版权声明：
// Copyright (C) 2020-2021 Coral Studio. All Rights Reserved.

#include <iostream>
#include "SoftwareInfo.h"
#include <Windows.h>
#include <string>
#include "NTVersion.h"
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
    <title>SoftwareInfo - Coral Remote Controller</title>
    <!-- Bootstrap -->
    <link href="/css/bootstrap-4.4.1.css" rel="stylesheet">
  </head>
  <body>
    <br>
    <center>
)";

    Code += "<h4> Windows 信息 </h4>\n";
    NTVERSIONINFO Info;
    GetWindowsNTVersionEx(Info, true);
    Code += (string)"<div> " + Info.OSName + "</div>\n";
    char VersionInfo[100];
    sprintf_s(VersionInfo, "<div> 版本：%d.%d.%d </div>\n" , Info.MajorVersion, Info.MinorVersion, Info.BuildNumber);
    Code += VersionInfo;
    Code += "<div> 启动时间：" + GetBootTime() + " </div>\n";
    Code += "<div> 计算机名：" + GetHostName() + " </div>\n";
    Code += "<br>";

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
