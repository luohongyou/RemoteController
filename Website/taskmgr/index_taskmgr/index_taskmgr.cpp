// index_taskmgr.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2024 Coral Studio. All Rights Reserved.

#define ADDRESS "/taskmgr/"

#include <iostream>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\");

    HTMLFrame HTML;
    HTML.Register(PT_INDEX, ADDRESS, "进程管理", "进程管理", R"(<li><a href="/">主页</a></li><li>进程管理</li>)", true);

    int AutoRefresh = GetSettings("AutoRefresh", true);

    std::string Code = (std::string)R"(
        <div class="row gy-4">

            <div class="portfolio-info">
              )" + (!AutoRefresh ? "" : "<!--") + R"( <div class="row gy-4">
			    <div class="col-lg-8"> )" + (!AutoRefresh ? "" : "-->") + R"(
                  <h3>进程列表</h3>
				)" + (!AutoRefresh ? "" : "<!--") + R"( </div>
				<div class="col-lg-4" align="right">
                  <button class="btn btn-success btn-sm" type="button" onclick="LoadTaskList() ">刷新</button>
				</div>
			  </div> )" + (!AutoRefresh ? "" : "-->") + R"(
              <div id="tasklist" style="overflow:auto; height:500px"></div>
            </div>

        </div>
        <script>
            function LoadTaskList()
            {
                ChangeTextByAJAX("tasklist", "/taskmgr/tasklist.exe");
            }
            LoadTaskList();

)" + (!AutoRefresh ? "/*" : "") + R"(
            setInterval(function(){
			    LoadTaskList();
		    }, 1000);
)" + (!AutoRefresh ? "*/" : "") + R"(

            function Kill(PID, ProcessName)
            {
                swal({
                  title: "结束进程",
                  text: "确实要结束PID为" + PID + "的" + ProcessName + " ？",
                  icon: "warning",
                  buttons: true,
                  dangerMode: true,
                  buttons: ["取消", "确定"],
                })
                .then((IsClose) => {
                  if (IsClose) {
                    GetTextByAJAX("/taskmgr/taskkill.exe?" + PID + "+0")
                    LoadTaskList();
                  }
                });
            }
            function KillAll(PID, ProcessName)
            {
                swal({
                  title: "结束进程",
                  text: "确实要结束所有" + ProcessName + " ？",
                  icon: "warning",
                  buttons: true,
                  dangerMode: true,
                  buttons: ["取消", "确定"],
                })
                .then((IsClose) => {
                  if (IsClose) {
                    GetTextByAJAX("/taskmgr/taskkill.exe?" + PID + "+1")
                    LoadTaskList();
                  }
                });
            }
        </script>

)";
    HTML.Custom(Code);

    HTML.Output();
    return 0;
}

