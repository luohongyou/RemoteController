// index_screenshoot.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2024 Coral Studio. All Rights Reserved.

#define ADDRESS "/screenshoot/"

#include <iostream>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\");

    HTMLFrame HTML;
    HTML.Register(PT_INDEX, ADDRESS, "屏幕截图", "屏幕截图", R"(<li><a href="/">主页</a></li><li>屏幕截图</li>)", true);


    std::string Code;

    Code = R"(
<div class="row gy-4">

    <div class="col-lg-8">
      <div class="portfolio-info">
        <div class="row gy-4">
          <div class="col-lg-8">
            <h3>实时屏幕</h3>
          </div>
          <div class="col-lg-4" align="right">
            <button class="btn btn-success btn-sm" type="button" onclick="RefreshScreen() ">刷新</button>
          </div>
        </div>
        <!-- <iframe src="/screenshoot/blank.html" id="now" frameborder="0" style="width:100%;height:400px"></iframe> -->
        <div id="now" style="overflow:auto; height:400px">
          <br><br><br><center><h2>暂无实时屏幕</h2><p>使用右侧“刷新”以获取实时屏幕。</p></center>
        </div>
      </div>
    </div>

    <div class="col-lg-4">
      <div class="portfolio-info">
        <div class="row gy-4">
          <div class="col-lg-8">
            <h3>所有截图</h3>
          </div>
          <div class="col-lg-4" align="right">
            <button class="btn btn-success btn-sm" type="button" onclick="DeleteAll() ">清空</button>
          </div>
        </div>
        <!-- <iframe src="/screenshoot/screenshootlist.exe" id="history" frameborder="0" style="width:100%; height:400px;"></iframe> -->
        <div id="screenshootlist" style="overflow:auto; height:400px"></div>
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
          ChangeTextByAJAX("now", "/screenshoot/printscreen.exe?0");
          ChangeTextByAJAX("screenshootlist", "/screenshoot/screenshootlist.exe");
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
              GetTextByAJAX("/screenshoot/screenshootdelete.exe?" + name);
              ChangeTextByAJAX("screenshootlist", "/screenshoot/screenshootlist.exe");
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
              GetTextByAJAX("/screenshoot/screenshootdelete.exe?all");
              ChangeTextByAJAX("screenshootlist", "/screenshoot/screenshootlist.exe");
              document.getElementById("now").innerHTML = '<br><br><br><center><h2>暂无实时屏幕</h2><p>使用右侧“刷新”以获取实时屏幕。</p></center>';
              ResetLightbox();
            }
          });
      }
      ChangeTextByAJAX("screenshootlist", "/screenshoot/screenshootlist.exe");
  </script>
)";

    HTML.Custom(Code);

    HTML.Output();
    return 0;
}

