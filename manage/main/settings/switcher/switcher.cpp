// printscreen.cpp : Coral Remote Controller 后台设置 系统开关模块

// 版权声明：
// Copyright (C) 2020-2021 Coral Studio. All Rights Reserved.

#include <iostream>
#include <fstream>
#include <Windows.h>
#include <string>
using namespace std;
ifstream fin;
ofstream fout;
int main(int argc, char* argv[])
{
    puts("Content-type: text/html\n");
    string Code = R"(
<!DOCTYPE html>
<html lang="zh">
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=gb2312">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>系统开关 - Coral Remote Controller</title>
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
              <h1 class="text-center">系统开关</h1>
              <p class="text-center">
)";
    if (!strcmp(argv[1], "on"))
    {
        fin.open("disabled");
        if (fin.fail())
        {
            Code += "系统已经开启，无需进行操作";
            fin.close();
        }
        else
        {
            fin.close();

            rename("login.disabled", "login");
            rename("logout.disabled", "logout");
            rename("network.disabled", "network");
            rename("power.disabled", "power");
            rename("runapp.disabled", "runapp");
            rename("screenshoot.disabled", "screenshoot");
            rename("sysinfo.disabled", "sysinfo");
            rename("taskmgr.disabled", "taskmgr");
            rename("windowmgr.disabled", "windowmgr");

            rename("index.html.disabled", "index.html");

            remove("disabled");

            Code += "成功启用了Coral Remote Controller";
        }
    }
    else if (!strcmp(argv[1], "off"))
    {
        fin.open("disabled");
        if (fin.good())
        {
            Code += "系统已经关闭，无需进行操作";
            fin.close();
        }
        else
        {
            fin.close();

            rename("login", "login.disabled");
            rename("logout", "logout.disabled");
            rename("network", "network.disabled");
            rename("power", "power.disabled");
            rename("runapp", "runapp.disabled");
            rename("screenshoot", "screenshoot.disabled");
            rename("sysinfo", "sysinfo.disabled");
            rename("taskmgr", "taskmgr.disabled");
            rename("windowmgr", "windowmgr.disabled");

            rename("index.html", "index.html.disabled");

            fout.open("disabled");
            fout.close();

            Code += "成功关闭了Coral Remote Controller";
        }
    }
    Code += R"(
</p>
              <center>
				  <a class="btn btn-outline-success my-2 my-sm-0" href="/manage/main/settings" title="" one-link-mark="yes">返回</a>
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
    puts(Code.c_str());
    return 0;
}