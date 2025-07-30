// index_errors_404.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/errors/404/"

#include <iostream>
#include "../../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\..\\");

    HTMLFrame HTML;
    HTML.Register(PT_INDEX, ADDRESS, "404 Not Found", "404 Not Found", "", false);


    HTML.Custom(R"(
    <div class="p-5 mb-4 bg-body-tertiary rounded-3">
      <div class="container-fluid py-7">
        <h1 class="display-7 fw-bold">404</h1>
        <p class="fs-6">请求的页面在服务器上不存在。</p>
        <a class="btn btn-success" href="/">返回主页</a>
      </div>
    </div>
)");

    HTML.Output();

    return 0;
}
