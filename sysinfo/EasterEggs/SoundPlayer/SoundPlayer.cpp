// SoundPlayer.cpp : Coral Remote Controller EasterEggs 音乐播放模块

// 版权声明：
// Copyright (C) 2020-2021 Coral Studio. All Rights Reserved.

#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
using namespace std;
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
    <title>Easter Eggs - Coral Remote Controller</title>
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
              <h1 class="text-center">Easter Eggs</h1>
              <p class="text-center">
)";
    ofstream fout;
    if (!strcmp(argv[1], "begin"))
    {
        fout.open("temp\\action\\PlayClassBegin");
        fout.close();
        Code += "成功播放了音乐：Symphony No. 100 in G Major \'Military\': III. Menuetto: Moderato (0-13s)";
    }
    else if (!strcmp(argv[1], "end"))
    {
        fout.open("temp\\action\\PlayClassEnd");
        fout.close();
        Code += "成功播放了音乐：Les premiers sourires de Vanessa (0-15s)";
    }
    Code += R"(
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
    puts(Code.c_str());
    return 0;
}
