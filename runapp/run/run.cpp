// run.cpp : Coral Remote Controller 执行文件 执行模块

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
    <title>run - Coral Remote Controller</title>
    <!-- Bootstrap -->
    <link href="/css/bootstrap-4.4.1.css" rel="stylesheet">
  </head>
  <body>
    <br>
    <br>
    <br>
    <center>
)";
    string Code2 = R"(
    </center>
    <center>
      <input class="btn btn-outline-success my-2 my-sm-1" type="button" name="Return" value="返回列表" onclick="window.location.href = '/runapp/listapp.exe'">
    </center>
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
        Code1 += "<h4>Web Application 调用格式错误！</h4>\n";
        Code1 += Code2;
        puts(Code1.c_str());
        return 0;
    }
    string Command = "";
    for (int i = 1; i < argc; i++)
        Command += argv[i], Command += " ";
    ofstream fout("temp\\action\\runapp");
    fout << Command;
    fout.close();

    Sleep(500);
    ifstream fin;
    fin.open("temp\\action\\runapp.success");
    if (fin.fail())
    {
        Code1 += "<h4>执行文件失败！</h4>\n";
        Code1 += Code2;
        puts(Code1.c_str());
        return 0;
    }
    fin.close();
    remove("temp\\action\\runapp.success");
    Code1 += "<h4>成功执行了" + Command + "！</h4>\n";
    Code1 += Code2;
    puts(Code1.c_str());
    return 0;
}
