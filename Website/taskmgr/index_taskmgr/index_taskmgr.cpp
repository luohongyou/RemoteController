// index_taskmgr.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

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
    <div class="portfolio-info">

        <ul class="nav nav-underline">
          <li class="nav-item">
            <a class="nav-link active" data-bs-toggle="tab" data-bs-target="#task-list" role="tab">进程列表</a>
          </li>
)"
#ifndef V4_RELEASE
+ R"(
          <li class="nav-item">
            <a class="nav-link" data-bs-toggle="tab" data-bs-target="#task-rules" role="tab">进程策略</a>
          </li>
)"
#endif
+ R"(
        </ul>

        <br>
        <div class="tab-content">

          <div class="tab-pane fade show active" id="task-list" role="tabpanel" tabindex="0">)" + (AutoRefresh ? "" : R"(
	    	<div class="col-4" align="right">
              <button class="btn btn-success btn-sm" type="button" onclick="LoadTaskList() ">刷新</button>
			</div>)") + R"(
            <div id="tasklist" style="overflow:auto; height:500px"></div>
          </div>
)" 
#ifndef V4_RELEASE
+ R"(
          <div class="tab-pane fade" id="task-rules" role="tabpanel" tabindex="1">
           In Development. Coming Soon.
            <div class="row">
              <div class="col-lg-4">
                <div class="form-floating mb-3">
                  <select class="form-select" id="Action" )" + (HTML.User.GetUserType() == UT_GUEST ? "disabled" : "") + R"(>
                    <option value="1" selected>阻止进程运行</option>
                    <option value="2">结束进程（增强版）</option>
                  </select>
                  <label for="Action">操作</label>
                </div>
              </div>

              <div class="col-lg-4">
                <div class="form-floating mb-3">
                  <select class="form-select" id="Mode" )" + (HTML.User.GetUserType() == UT_GUEST ? "disabled" : "") + R"(>
                    <option value="1" selected>进程名</option>
                    <option value="0">PID</option>
                  </select>
                  <label for="Mode">进程标识</label>
                </div>
              </div>

              <div class="col-lg-4">
                <div class="form-floating mb-3">
                  <input type="number" class="form-control" id="ID" value="0" )" + (HTML.User.GetUserType() == UT_GUEST ? "disabled" : "") + R"(>
                  <label for="ID">要操作的进程</label>
                </div>
              </div>

              <div class="d-grid gap-2 d-md-flex justify-content-md-end">
                <button class="btn btn-success" type="button" onclick="Execute() " )" + (HTML.User.GetUserType() == UT_GUEST ? "disabled" : "") + R"(>执行</button>
              </div>

            </div>

          </div>
)"
#endif
+ R"(

        </div>
    </div>

        <script>
            function LoadTaskList()
            {
                AsyncChangeTextByAJAX("tasklist", "/taskmgr/tasklist.exe");
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

