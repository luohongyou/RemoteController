// index.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/"

#include <iostream>
#include "../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\");

    // 检查系统是否OOBE / 需要迁移配置
    std::ifstream fin;
    int _Version = 1;
    fin.open(DatabasePath() + "OOBEFlag");
    if (fin.fail())
    {
        fin.close();
        AJAXOutput(R"(<script> window.location.href = "/OOBE"; </script>)");
        return 0;
    }
    fin >> _Version;
    // 3.0 - 4.0 : 新文件格式
    if (_Version < 4)
    {
        fin.close();
        AJAXOutput(R"(<script> window.location.href = "/OOBE/update"; </script>)");
        return 0;
    }
    fin.close();

    HTMLFrame HTML;
    HTML.Register(PT_INDEX, ADDRESS, "主页", "主页", "", true);

    // 检查用户的OOBE状态
    if (HTML.User.IsOOBEed() < OOBEVERSION && !HTML.User.IsForegroundUser())
    {
        AJAXOutput(R"(<script> window.location.href = "/OOBE/user"; </script>)");
        return 0;
    }

    // 主页代码
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

    HTMLFrame::HTMLToolCards ToolCards;

    ToolCards.Add("基本控制", "控制远程计算机的电源、音量，以及更改桌面壁纸和锁定屏幕。", "/power", "bi bi-sliders");
    ToolCards.Add("进程管理", "对远程计算机上正运行的进程进行控制，包括轻松结束他们。", "/taskmgr", "bi bi-card-list");
    ToolCards.Add("窗口管理", "列出远程计算机上正打开的窗口的信息，并可对他们进行操作。", "/windowmgr", "bi bi-window-stack");
    ToolCards.Add("文件管理", "浏览和管理远程计算机上的所有文件，并可轻松上传和下载文件。", "/explorer", "bi bi-folder2-open");
    ToolCards.Add("实时屏幕", "查看远程计算机实时的屏幕图像，并可以截取屏幕和查看之前的截图。", "/screenshot", "bi bi-display");
    ToolCards.Add("剪贴板", "获取和更改远程计算机实时的剪贴板数据，并可查看剪贴板历史记录。", "/clipboard", "bi bi-clipboard-data");
    ToolCards.Add("内容推送", "向远程计算机的前端界面推送自定义的照片、视频、音频、网页等内容。", "/datapush", "bi bi-file-arrow-up");
    ToolCards.Add("执行命令", "在远程计算机上以各种权限与各种交互方式运行自定义的脚本指令。", "/command", "bi bi-terminal-plus");
    ToolCards.Add("发送消息", "向远程计算机前端界面发送自定义信息，支持多种显示模式。", "/message", "bi bi-chat-left-text");
    ToolCards.Add("网络设置", "更改远程计算机上的hosts信息，可实现域名黑名单和手动链接域名至指定服务器。", "/network", "bi bi-globe2");
    ToolCards.Add("系统信息", "查看远程计算机硬件以及系统软件信息，还可查看Remote Controller版本信息。", "/sysinfo", "bi bi-motherboard");

    Code += ToolCards.Output();

    HTML.Custom(Code);
    HTML.Output();

    return 0;
}

