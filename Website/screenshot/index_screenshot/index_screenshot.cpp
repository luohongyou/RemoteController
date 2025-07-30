// index_screenshot.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define _CRT_SECURE_NO_WARNINGS

#define ADDRESS "/screenshot/"

#include <iostream>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\");

    HTMLFrame HTML;
    HTML.Register(PT_INDEX, ADDRESS, "实时屏幕", "实时屏幕", R"(<li><a href="/">主页</a></li><li>实时屏幕</li>)", true);


    std::string Code;

    Code = (std::string)R"(
<div class="row gy-4">

    <div class="col-lg-8">
      <div class="portfolio-info">
        <div class="row gy-4">

          <div class="col-8">
            <ul class="nav nav-underline">
              <li class="nav-item">
                <a class="nav-link active" data-bs-toggle="tab" data-bs-target="#static-pane" role="tab">静态屏幕</a>
              </li>
              <li class="nav-item">
                <a class="nav-link" data-bs-toggle="tab" data-bs-target="#dynamic-pane" role="tab">动态屏幕 (Beta)</a>
              </li>
            </ul>
          </div>
          <div class="col-4" align="right">
            <button class="btn btn-success btn-sm" type="button" onclick="RefreshScreen() ">截图</button>
          </div>

        </div>

        <br>
        <div class="tab-content">

          <div class="tab-pane fade show active" id="static-pane" role="tabpanel" tabindex="0">
            <div id="now" style="overflow:auto; height:400px">
              <br><br><br><center><h2>暂无实时屏幕</h2><p>使用右侧“截图”以获取实时屏幕。</p></center>
            </div>
          </div>
          <div class="tab-pane fade" id="dynamic-pane" role="tabpanel" tabindex="1">
            <iframe id="dynamicscreen" src="" width="100%" style="height:400px; border:none; display:none"></iframe>
            <div id="dynamicdisabled" style="overflow:auto; height:400px">
)" + (GetSettings("RealtimeScreen", false) ? R"(
              <br><br><br>
              <center><h2>动态屏幕(Beta)</h2>
              <p>点击下方“开始”，以查看远程计算机实时的动态屏幕。</p>
              <p><b>注意：</b>该功能处于早期开发阶段，可能尚不稳定。</p>
              <button class="btn btn-success" type="button" onclick="StartRealtimeScreen() ">开始</button></center>
)" : R"(
              <br><br><br><center><h2>动态屏幕(Beta) 未启用</h2><p>请联系管理员以获取更多信息。</p></center>
)") + R"(
            </div>
          </div>

        </div>

      </div>
    </div>

    <div class="col-lg-4">
      <div class="portfolio-info">
        <div class="row gy-4">
          <div class="col-8">
            <h3>屏幕截图</h3>
          </div>
          <div class="col-4" align="right">
            <button class="btn btn-success btn-sm" type="button" onclick="DeleteAll() ">清空</button>
          </div>
        </div>
        <div id="screenshotlist" style="overflow:auto; height:400px"></div>
      </div>
    </div>

</div>


  <script>
      function ResetLightbox()
      {
          portfolioLightbox.destroy();
          portfolioLightbox = GLightbox({
            selector: '.portfolio-lightbox'
          });
      }
      function RefreshScreen()
      {
          ChangeTextByAJAX("now", "/screenshot/printscreen.exe?0");
          ChangeTextByAJAX("screenshotlist", "/screenshot/screenshotlist.exe");
          ResetLightbox();
      }
      function Delete(name) 
      {
          swal({
            title: "删除截图",
            text: "确实要删除：" + name + "？",
            icon: "warning",
            buttons: true,
            dangerMode: true,
            buttons: ["取消", "确定"],
          })
          .then((willDelete) => {
            if (willDelete) {
              GetTextByAJAX("/screenshot/screenshotdelete.exe?" + name);
              ChangeTextByAJAX("screenshotlist", "/screenshot/screenshotlist.exe");
              ResetLightbox();
            }
          });
      }
      function DeleteAll() 
      {
          swal({
            title: "删除所有截图",
            text: "确实要删除所有截图吗？该操作不可逆。",
            icon: "warning",
            buttons: true,
            dangerMode: true,
            buttons: ["取消", "确定"],
          })
          .then((willDelete) => {
            if (willDelete) {
              GetTextByAJAX("/screenshot/screenshotdelete.exe?all");
              ChangeTextByAJAX("screenshotlist", "/screenshot/screenshotlist.exe");
              document.getElementById("now").innerHTML = '<br><br><br><center><h2>暂无实时屏幕</h2><p>使用右侧“刷新”以获取实时屏幕。</p></center>';
              ResetLightbox();
            }
          });
      }
      function StartRealtimeScreen()
      {
          document.getElementById('dynamicscreen').src = "http://" + window.location.hostname + ":8089/main.html";
          document.getElementById('dynamicdisabled').style.display = "none";
          document.getElementById('dynamicscreen').style.display = "block";
      }
      ChangeTextByAJAX("screenshotlist", "/screenshot/screenshotlist.exe");
  </script>
)";

    HTML.Custom(Code);

    HTML.Output();
    return 0;
}

