// index_manage_main_log.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/manage/main/log/"

#include <iostream>
#include "../../../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\..\\..\\");

    HTMLFrame HTML;
    HTML.Register(PT_INDEX, ADDRESS, "日志查看", "日志查看", R"(<li><a href="/">主页</a></li><li><a href="/manage/main">后台管理</a></li><li>日志查看</li>)", true);
    HTML.ManageInit();
    bool IsRoot = HTML.User.GetUserType() == UT_ROOT;

    int Log = GetSettings("Log", true);

    std::string Code = (std::string)R"(
    <div class="portfolio-info">

)" + (Log ? "<!--" : "") + R"(
        <div class="alert alert-danger" role="alert">
          日志功能已被设置为<b>关闭</b>，您可以在此处查看以前的日志，或者联系超级管理员打开日志记录。
        </div>
)" + (Log ? "-->" : "") + R"(

        <ul class="nav nav-underline">
          <li class="nav-item">
            <a class="nav-link active" data-bs-toggle="tab" data-bs-target="#basic-pane" role="tab">基础</a>
          </li>
          <li class="nav-item">
            <a class="nav-link" data-bs-toggle="tab" data-bs-target="#advanced-pane" role="tab">高级</a>
          </li>
        </ul>

        <br>
        <div class="tab-content">

          <div class="tab-pane fade show active" id="basic-pane" role="tabpanel" tabindex="0">
              )" + (IsRoot ? "" : "<!--") + R"( <div class="row gy-4">
			    <div class="col-9"> )" + (IsRoot ? "" : "-->") + R"(
                  <h3>Remote Controller 基本日志</h3>
				)" + (IsRoot ? "" : "<!--") + R"( </div>
				<div class="col-3" align="right">
                  <button class="btn btn-success btn-sm" type="button" onclick="Clear(1) ">清空</button>
				</div>
			  </div> )" + (IsRoot ? "" : "-->") + R"(

            <div id="basiclog" style="overflow:auto; height:350px"></div>
          </div>

          <div class="tab-pane fade" id="advanced-pane" role="tabpanel" tabindex="1">
              )" + (IsRoot ? "" : "<!--") + R"( <div class="row gy-4">
			    <div class="col-9"> )" + (IsRoot ? "" : "-->") + R"(
                  <h3>Remote Controller Website 日志</h3>
				)" + (IsRoot ? "" : "<!--") + R"( </div>
				<div class="col-3" align="right">
                  <button class="btn btn-success btn-sm" type="button" onclick="Clear(2) ">清空</button>
				</div>
			  </div> )" + (IsRoot ? "" : "-->") + R"(

            <div id="websitelog" style="overflow:auto; height:350px"></div>

            <br><br>

              )" + (IsRoot ? "" : "<!--") + R"( <div class="row gy-4">
			    <div class="col-9"> )" + (IsRoot ? "" : "-->") + R"(
                  <h3>Remote Agent 日志</h3>
				)" + (IsRoot ? "" : "<!--") + R"( </div>
				<div class="col-3" align="right">
                  <button class="btn btn-success btn-sm" type="button" onclick="Clear(3) ">清空</button>
				</div>
			  </div> )" + (IsRoot ? "" : "-->") + R"(

            <div id="agentlog" style="overflow:auto; height:350px"></div>
          </div>

        </div>
    </div>
    <script>
        AsyncChangeTextByAJAX("basiclog", "/manage/main/log/logdisplayer.exe?1");
        AsyncChangeTextByAJAX("websitelog", "/manage/main/log/logdisplayer.exe?2");
        AsyncChangeTextByAJAX("agentlog", "/manage/main/log/logdisplayer.exe?3");

        function Clear(ID)
        {
            swal({
              title: "清空日志",
              text: "确实要清空指定的日志数据吗？",
              icon: "warning",
              buttons: true,
              dangerMode: true,
              buttons: ["取消", "确定"],
            })
            .then((willExec) => {
              if (willExec) {
                GetTextByAJAX("/manage/main/log/clearlog.exe?" + ID);
                AsyncChangeTextByAJAX("basiclog", "/manage/main/log/logdisplayer.exe?1");
                AsyncChangeTextByAJAX("websitelog", "/manage/main/log/logdisplayer.exe?2");
                AsyncChangeTextByAJAX("agentlog", "/manage/main/log/logdisplayer.exe?3");
              }
            });
        }
    </script>
)";

    HTML.Custom(Code);

    HTML.Output();
    return 0;

}