// index.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2024 Coral Studio. All Rights Reserved.

#define ADDRESS "/"

#include <iostream>
#include "../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\");

    HTMLFrame HTML;
    HTML.Register(PT_INDEX, ADDRESS, "主页", "主页", "", true);

    // 检查用户的OOBE状态
    if (HTML.User.IsOOBEed() < OOBEVERSION)
    {
        AJAXOutput(R"(<script> window.location.href = "/OOBE/user"; </script>)");
        return 0;
    }

    std::string Code = R"(
    <div class="p-5 mb-4 bg-body-tertiary rounded-3">
      <div class="container-fluid py-7">
        <h1 class="display-7 fw-bold">欢迎使用 <span class="text-primary">Coral Remote Controller</span></h1>
        <p class="fs-6">借助Coral Remote Controller，您可以轻松地在浏览器上远程控制和管理远端计算机。</p>
      </div>
    </div>
    
    <br>
    <div class="section-title">
      <h2>全部工具</h2>
    </div>
)";

    HTMLToolCards ToolCards;

    ToolCards.Add("基本控制", "控制远程计算机的电源、音量等，还可远程更改桌面壁纸。", "/power");
    ToolCards.Add("进程管理", "对远程计算机上正运行的进程进行控制，包括轻松结束他们。", "/taskmgr");
    ToolCards.Add("窗口管理", "列出远程计算机上正打开的窗口的信息，并可轻松对他们进行操作。", "/windowmgr");
    ToolCards.Add("文件浏览", "在只读模式下浏览远程计算机上的文件，并可轻松下载查看它们。", "/explorer");
    ToolCards.Add("屏幕截图", "截获远程计算机实时的屏幕图像，并可以轻松地查看和管理之前的截图。", "/screenshoot");
    ToolCards.Add("剪贴板", "获取和更改远程计算机实时的剪贴板数据，并可轻松查看剪贴板历史记录。", "/clipboard");
    ToolCards.Add("发送消息", "向远程计算机前端界面发送自定义信息，支持多种显示模式。", "/message");
    ToolCards.Add("网络设置", "更改远程计算机上的hosts信息，可实现域名黑名单和手动链接域名至指定服务器。", "/network");
    ToolCards.Add("系统信息", "查看远程计算机硬件以及系统软件信息，还可查看Remote Controller版本信息。", "/sysinfo");

    Code += ToolCards.Output();

    HTML.Custom(Code);
    HTML.Output();

    return 0;
}

