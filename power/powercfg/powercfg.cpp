// powercfg.cpp : Coral Remote Controller 电源控制 操作执行模块

// 版权声明：
// Copyright (C) 2020-2021 Coral Studio. All Rights Reserved.

// 调用方式：powercfg.exe action

#include <iostream>
#include <Windows.h>
#include <tlhelp32.h>
#include <comdef.h>
#include <sstream>
#include "NTVersion.h"
using namespace std;

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
    <title>电源控制 - Coral Remote Controller</title>
    <!-- Bootstrap -->
    <link href="/css/bootstrap-4.4.1.css" rel="stylesheet">
  </head>
  <body>
    <nav class="navbar navbar-expand-lg navbar-light bg-light">
      <a class="navbar-brand" href="/">Coral Remote Controller</a>
      <button class="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarSupportedContent" aria-controls="navbarSupportedContent" aria-expanded="false" aria-label="Toggle navigation"> <span class="navbar-toggler-icon"></span> </button>
      </div>
    </nav>
    <header>
      <div class="jumbotron">
        <div class="container">
          <div class="row">
            <div class="col-12">
              <h1 class="text-center">电源控制</h1>
              <p class="text-center">
)";
    string Code2 = R"(
</p>
              <center>
				  <a class="btn btn-outline-success my-2 my-sm-0" href="/" title="" one-link-mark="yes">返回主页</a>
			  </center>
            </div>
          </div>
        </div>
      </div>
    </header>
    <footer class="text-center">
      <div class="container">
        <div class="row">
          <div class="col-12">
            <p>Copyright (c) 2020-2021 Coral Studio. All rights reserved.</p>
          </div>
        </div>
      </div>
    </footer>
    <!-- jQuery (necessary for Bootstrap's JavaScript plugins) --> 
    <script src="/js/jquery-3.4.1.min.js"></script> 
    <!-- Include all compiled plugins (below), or include individual files as needed --> 
    <script src="/js/popper.min.js"></script> 
    <script src="/js/bootstrap-4.4.1.js"></script>
  </body>
</html>
)";
    // 检查参数
    if (argc != 2)
    {
        Code1 += "Web Application Error!";
        Code1 += Code2;
        puts(Code1.c_str());
        return 0;
    }

    if (!strcmp(argv[1], "shutdown"))
    {
        NTVERSIONINFO Info;
        GetWindowsNTVersionEx(Info, false);
        if (Info.BuildNumber >= 9200)
            system("shutdown -s -hybrid -t 0");
        else
            system("shutdown -s -t 0");
        Code1 += "成功发送了关机消息，远程计算机将关闭。";
    }
    else if (!strcmp(argv[1], "restart"))
    {
        system("shutdown -r -t 0");
        Code1 += "成功发送了重新启动消息，远程计算机将重新启动。";
    }
    else
    {
        Code1 += "Web Application Error!";
        Code1 += Code2;
        puts(Code1.c_str());
        return 0;
    }

    Code1 += Code2;
    puts(Code1.c_str());
    return 0;
}
