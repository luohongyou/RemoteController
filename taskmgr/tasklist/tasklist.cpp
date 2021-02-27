// tasklist.cpp : Coral Remote Controller 任务管理 任务列表模块

// 版权声明：
// Copyright (C) 2020-2021 Coral Studio. All Rights Reserved.

#include <iostream>
#include <Windows.h>
#include <tlhelp32.h>
#include <comdef.h>
#include <algorithm>
#include <sstream>
using namespace std;
struct Process
{
	string Name;
	string fakeName;
	int PID;
} List[10001];
bool cmp(Process A, Process B)
{
	if (A.fakeName == B.fakeName)
		return A.PID < B.PID;
	return A.fakeName < B.fakeName;
}
int cnt = 0;
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
    <title>tasklist - Coral Remote Controller</title>
    <!-- Bootstrap -->
    <link href="/css/bootstrap-4.4.1.css" rel="stylesheet">
  </head>
  <body>
    <script>
		function Kill(id, name) {
			var ans = confirm("确实要结束PID为" + id + "的进程" + name + "吗？");
			if (ans == true) window.location.href = "/taskmgr/taskkill.exe?" + id;
		}
		function KillAll(id, name) {
			var ans = confirm("确实要结束所有名为" + name + "的进程吗？");
			if (ans == true) window.location.href = "/taskmgr/taskkill.exe?" + id + "+all";
		}
    </script>
<center>
<table class="table table-hover">
<tr>
<th>名称</th>
<th>PID</th>
<th>操作</th>
</tr>
)";
	HANDLE hToken;
	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken);
	TOKEN_PRIVILEGES tp;
	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
	PROCESSENTRY32 pd;
	pd.dwSize = sizeof(pd);
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	BOOL choose = ::Process32First(hProcessSnap, &pd);
	while (choose)
	{
		_bstr_t ExeFile(pd.szExeFile);
		List[++cnt].Name = ExeFile, List[cnt].PID = pd.th32ProcessID;
		List[cnt].fakeName = List[cnt].Name;
		for (register int i = 0; i < List[cnt].fakeName.size(); ++i)
			if ('a' <= List[cnt].fakeName[i] && List[cnt].fakeName[i] <= 'z')
				List[cnt].fakeName[i] -= 'a' - 'A';
		choose = ::Process32Next(hProcessSnap, &pd);
	}
	sort(List + 1, List + 1 + cnt, cmp);
	for (register int i = 1; i <= cnt; ++i)
	{
		stringstream tempIO;
		string sPID;
		tempIO << List[i].PID;
		tempIO >> sPID;
		Code += "<tr>\n";
		Code += "<td>" + List[i].Name + "</td>\n";
		Code += "<td>" + sPID + "</td>\n";
		Code += "<td><a href=\"javascript:void(0);\" onclick=\"Kill(" + sPID + ", \'" + List[i].Name + "\')\">结束</a>&nbsp;&nbsp;&nbsp;<a href=\"javascript:void(0);\" onclick=\"KillAll(" + sPID + ", \'" + List[i].Name + "\')\">结束所有</a></td>\n";
		Code += "</tr>\n";
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