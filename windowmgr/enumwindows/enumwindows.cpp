// enumwindows.cpp : Coral Remote Controller 窗口管理 列出窗口模块

// 版权声明：
// Copyright (C) 2020-2021 Coral Studio. All Rights Reserved.

#include <iostream>
#include <Windows.h>
#include <tlhelp32.h>
#include <comdef.h>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;
int cnt = 0;
ifstream fin;
ofstream fout;
string Title, ClassName, WindowID;
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
    <title>enumwindows - Coral Remote Controller</title>
    <!-- Bootstrap -->
    <link href="/css/bootstrap-4.4.1.css" rel="stylesheet">
  </head>
  <body>
    <script>
		function Close(id, name) {
			var ans = confirm("确实要关闭窗口“" + name + "”吗？");
			if (ans == true) window.location.href = "/windowmgr/windowkiller.exe?" + id;
		}
		function ForceClose(id, name) {
			var ans = confirm("确实要强行关闭窗口“" + name + "”吗？");
			if (ans == true) window.location.href = "/windowmgr/windowkiller.exe?" + id + "+force";
		}
    </script>
<center>
<table class="table table-hover">
<tr>
<th>标题</th>
<th>类名</th>
<th>操作</th>
</tr>
)";
	fout.open("temp\\action\\enumwindows");
	fout.close();

	Sleep(500);

	fin.open("temp\\action\\enumwindows.success");
	if (fin.good())
	{
		fin >> cnt;
		fin.get();
		for (register int i = 1; i <= cnt; ++i)
		{
			getline(fin, Title);
			getline(fin, ClassName);
			getline(fin, WindowID);
			Code += "<tr>\n";
			Code += "<td>" + Title + "</td>\n";
			Code += "<td>" + ClassName + "</td>\n";
			Code += "<td><a href=\"javascript:void(0);\" onclick=\"Close(" + WindowID + ", \'" + Title + "\')\">关闭</a>&nbsp;&nbsp;&nbsp;<a href=\"javascript:void(0);\" onclick=\"ForceClose(" + WindowID + ", \'" + Title + "\')\">强行关闭</a></td>\n";
			Code += "</tr>\n";
		}
		fin.close();
		remove("temp\\action\\enumwindows.success");
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