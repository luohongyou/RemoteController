// index_explorer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2024 Coral Studio. All Rights Reserved.

#define ADDRESS "/explorer/"

#include <iostream>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\");

    HTMLFrame HTML;
    HTML.Register(PT_INDEX, ADDRESS, "文件浏览", "文件浏览", R"(<li><a href="/">主页</a></li><li>文件浏览</li>)", true);

    std::string Code = R"(
<div class="portfolio-info">
    <form class="toolbar">
    <div class="btn-group btn-group-sm" role="group" style="width:100%;">
      <button type="button" class="btn btn-primary" onclick="SwitchQuickStartBar() " id="qbarbutton"><i class="bi bi-list-ul"></i></button>
      <button type="button" class="btn btn-explorer" id="prebutton" onclick="PreLoadAddress(); RefreshFileList();"><i class="bi bi-arrow-left"></i></button>
      <button type="button" class="btn btn-explorer" id="nextbutton" onclick="NextLoadAddress(); RefreshFileList();"><i class="bi bi-arrow-right"></i></button>
      <button type="button" class="btn btn-explorer" id="upbutton" onclick="UpFolder() "><i class="bi bi-arrow-up"></i></button>
      <button type="submit" class="btn btn-explorer" id="refreshbutton"><i class="bi bi-arrow-clockwise"></i></button>
      <input class="form-control form-control-sm" type="text" placeholder="输入要访问的文件夹" id="address">

      <button type="button" class="btn btn-explorer dropdown-toggle" data-bs-toggle="dropdown" data-bs-auto-close="outside"><i class="bi bi-download"></i></button>
      <div class="dropdown-menu dropdown-menu-end">
        <div class="px-2 py-2">
          <h6 align="center"><b>下载列表</b></h6>
          <div id="downloadnone" align="center" style="font-size:14px;">暂无正在进行的文件下载</div>
          <table id="downloadlist" class="table table-borderless" style="font-size:14px;"></table>
        </div>
      </div>


      <button type="button" class="btn btn-explorer dropdown-toggle" data-bs-toggle="dropdown" data-bs-auto-close="outside"><i class="bi bi-gear-fill"></i></button>
      <div class="dropdown-menu dropdown-menu-end">
        <div class="px-4 py-3">
          <div class="form-check form-switch">
            <input class="form-check-input" type="checkbox" role="switch" id="ShowInvisibleItems">
            <label class="form-check-label" for="ShowInvisibleItems">显示隐藏的项目</label>
          </div>
          <div class="dropdown-divider"></div>
          <b>文件排序</b>
          <div id="SortBy">
            <div class="form-check form-check-inline">
              <input class="form-check-input" type="radio" name="SortBy" value="0" id="r11" checked>
              <label class="form-check-label" for="r11">名称</label>
            </div>
            <div class="form-check form-check-inline">
              <input class="form-check-input" type="radio" name="SortBy" value="1" id="r12">
              <label class="form-check-label" for="r12">时间</label>
            </div>
          </div>
          <div id="SortDirection">
            <div class="form-check form-check-inline">
              <input class="form-check-input" type="radio" name="SortDirection" value="0" id="r21" checked>
              <label class="form-check-label" for="r21">升序</label>
            </div>
            <div class="form-check form-check-inline">
              <input class="form-check-input" type="radio" name="SortDirection" value="1" id="r22">
              <label class="form-check-label" for="r22">降序</label>
            </div>
          </div>
          <div class="dropdown-divider"></div>
          <div class="form-check form-switch">
            <input class="form-check-input" type="checkbox" role="switch" id="IsCompactMode">
            <label class="form-check-label" for="IsCompactMode">紧凑模式</label>
          </div>
        </div>
      </div>

    </div>
    </form>

    <div class="row gy-2">
      <div id="quickstartframe" class="col-lg-2">
        <div id="quickstart" style="overflow:auto;"></div>
      </div>
      <div id="filelistframe" class="col-lg-10">
        <div id="filelist" style="overflow:auto; height:500px"></div>
      </div>
    </div>
  </div>
</div>

<script src="/assets/js/filelist.js?version=)" + (std::string)PROGRAM_BUILD_S + R"("></script>
)";

    HTML.Custom(Code);
    
    HTML.Output();
    return 0;
}
