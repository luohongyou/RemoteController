// index_datapush.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/datapush/"

#include <iostream>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\");

    HTMLFrame HTML;
    HTML.Register(PT_INDEX, ADDRESS, "内容推送", "内容推送", R"(<li><a href="/">主页</a></li><li>内容推送</li>)", true);

    HTML.Custom((std::string)R"(
    <div class="portfolio-info">

      <ul class="nav nav-underline">
        <li class="nav-item">
          <a class="nav-link active" data-bs-toggle="tab" data-bs-target="#media-pane" role="tab">影音推送</a>
        </li>
        <li class="nav-item">
          <a class="nav-link" data-bs-toggle="tab" data-bs-target="#richtext-pane" role="tab">图文推送</a>
        </li>
      </ul>
      <br>

      <div class="tab-content">
        <div class="tab-pane fade show active" id="media-pane" role="tabpanel" tabindex="0">
          In Development. Coming Soon.
        </div>
        <div class="tab-pane fade" id="richtext-pane" role="tabpanel" tabindex="1">
          In Development. Coming Soon.
        </div>
      </div>

    </div>

    <script>

    </script>
)");
    HTML.Output();

    return 0;
}
