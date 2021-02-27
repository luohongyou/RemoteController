// screenshootlist.cpp : Coral Remote Controller 屏幕截图 列出截屏模块

// 版权声明：
// Copyright (C) 2020-2021 Coral Studio. All Rights Reserved.

#include <iostream>
#include <string>
#include <fstream>
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
    <title>screenshootlist - Coral Remote Controller</title>
    <!-- Bootstrap -->
    <link href="/css/bootstrap-4.4.1.css" rel="stylesheet">
  </head>
  <body>
    <script>
		function Delete(name) {
			var ans = confirm("确实要删除截图" + name + "吗？");
			if (ans == true) window.location.href = "/screenshoot/screenshootdelete.exe?" + name;
		}
    </script>
<center>
<table class="table table-hover">
<tr>
<th>缩略图</th>
<th>文件名</th>
<th>操作</th>
</tr>
)";
	int cnt;
	ifstream fin;
	string FileName;
	fin.open("screenshoot\\cache\\list");
	if (fin.fail())
		fin.close();
	else
	{
		fin >> cnt;
		for (register int i = 1; i <= cnt; ++i)
		{
			fin >> FileName;
			Code += "<tr>\n";
			Code += "<td> <img src=\"\\screenshoot\\cache\\jpg\\" + FileName + ".jpg\" width=\"100 % \"/> </td>\n";
			Code += "<td>" + FileName + "</td>\n";
			Code += "<td><a href=\"/screenshoot/cache/" + FileName + "\" target=\"_blank\">查看</a>&nbsp;&nbsp;&nbsp;<a href=\"\\screenshoot\\cache\\" + FileName + "\" download=\"" + FileName + "\">下载</a>&nbsp;&nbsp;&nbsp;<a href=\"javascript:void(0);\" onclick=\"Delete('" + FileName + "')\">删除</a></td>\n";
			Code += "</tr>\n";
		}
		fin.close();
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