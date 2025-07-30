// index_errors_500.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/errors/500/"

#include <iostream>
#include "../../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\..\\");

    HTMLFrame HTML;
    HTML.Register(PT_INDEX, ADDRESS, "500 Internal Server Error", "500 Internal Server Error", "", false);


    HTML.Custom(R"(
    <div class="p-5 mb-4 bg-body-tertiary rounded-3">
      <div class="container-fluid py-7">
        <h1 class="display-7 fw-bold">500</h1>
        <p class="fs-6">服务器内部错误。请联系管理员获取更多信息。</p>
        <a class="btn btn-success" href="/">返回主页</a>
      </div>
    </div>
)");

    HTML.Output();

    return 0;
}
