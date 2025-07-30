// index_windowmgr.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/windowmgr/"

#include <iostream>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\");

    HTMLFrame HTML;
    HTML.Register(PT_INDEX, ADDRESS, "窗口管理", "窗口管理", R"(<li><a href="/">主页</a></li><li>窗口管理</li>)", true);

    int AutoRefresh = GetSettings("AutoRefresh", true);

    std::string Code = (std::string)R"(
             <div class="portfolio-info">
              )" + (!AutoRefresh ? "" : "<!--") + R"( <div class="row gy-4">
			    <div class="col-8"> )" + (!AutoRefresh ? "" : "-->") + R"(
                  <h3>窗口列表</h3>
				)" + (!AutoRefresh ? "" : "<!--") + R"( </div>
				<div class="col-4" align="right">
                  <button class="btn btn-success btn-sm" type="button" onclick="LoadWindowList() ">刷新</button>
				</div>
			  </div> )" + (!AutoRefresh ? "" : "-->") + R"(
              <div id="windowlist" style="overflow:auto; height:500px"></div>
            </div>

        <script>
            function LoadWindowList()
            {
                AsyncChangeTextByAJAX("windowlist", "/windowmgr/listwindows.exe");
            }
            LoadWindowList();

)" + (!AutoRefresh ? "/*" : "") + R"(
            setInterval(function(){
			    LoadWindowList();
		    }, 1000);
)" + (!AutoRefresh ? "*/" : "") + R"(

            function Close(WindowID, Title)
            {
                swal({
                  title: "关闭窗口",
                  text: "确实要关闭窗口：" + Title + " ？",
                  icon: "warning",
                  buttons: true,
                  dangerMode: true,
                  buttons: ["取消", "确定"],
                })
                .then((IsClose) => {
                  if (IsClose) {
                    GetTextByAJAX("/windowmgr/closewindow.exe?" + WindowID + "+0");
                    LoadWindowList();
                  }
                });
            }
            function ForceClose(WindowID, Title)
            {
                swal({
                  title: "强制关闭窗口",
                  text: "确实要强制关闭窗口：" + Title + " ？",
                  icon: "warning",
                  buttons: true,
                  dangerMode: true,
                  buttons: ["取消", "确定"],
                })
                .then((IsClose) => {
                  if (IsClose) {
                    GetTextByAJAX("/windowmgr/closewindow.exe?" + WindowID + "+1");
                    LoadWindowList();
                  }
                });
            }
        </script>

)";

    HTML.Custom(Code);

    HTML.Output();
    return 0;
}

