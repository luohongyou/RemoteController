// changelog.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2024 Coral Studio. All Rights Reserved.

#define ADDRESS "/OOBE/user/changelog.exe"

#include <iostream>
#include "../../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\..\\");

    HTMLFrame HTML;
    HTML.Register(PT_INDEX, ADDRESS, "changelog", "changelog", "", true);

    std::string Source, line, Code;
    std::stringstream tempIO;

    Source = R"(
#Remote Controller 3.0.0 Build )" + (std::string)PROGRAM_BUILD_S + R"( (2024/07/26)
*代码重构与组件更新
启用基于Bootstrap 5.3和相关模板的新用户界面，体验焕然一新
大幅改进了页面加载和身份验证机制，页面的响应速度大大提升，刷新列表也不再出现闪烁的现象
改进了Remote Agent的调用机制，提升了部分功能的稳定性与响应速度
Remote Controller组件现在运行在64位架构上
将Apache服务器组件从2.4.46更新至2.4.59
*新功能与改进
基本控制模块现在不仅可以执行更多电源操作，还可以设置音量和壁纸
进程管理、窗口管理现在支持自动刷新列表
文件浏览模块显示更加紧凑，并引入了可编辑的地址栏、快速访问侧边栏、前进返回和上移按钮、文件排序选项和下载列表等等，且加入了打包下载文件夹的支持
屏幕截图模块现在使用了更易用的预览工具，并提供体积更小的jpg格式图像以供下载
剪贴板模块现在支持修改远程计算机的实时剪贴板，并降低了保存的历史剪贴板出错的概率
发送消息模块增加了对消息类型图标进行设置的支持
网络设置模块现在会在修改规则后对系统相关缓存进行刷新，使设置立即生效
后台管理的系统设置模块新增了多个设置项
后台管理的日志查看模块现在支持查看重新设计的基本日志和更多自动记录的详细日志
后台管理新增了存储管理和程序更新模块，支持清理Remote Controller临时文件和浏览器上进行程序更新
Remote Agent组件现在可以显示托盘图标以供前台操作，并防止自身进程意外退出
*Bug修复
解决了Remote Agent在截取屏幕时出现严重内存泄漏的问题
*已弃用的功能
仪表盘已经被回滚为主页，以后的版本可能会重新引入
窗口管理现在不再显示窗口类名，而改为显示进程PID
后台管理的系统控制模块已经被合并进入了系统设置模块
自定义系统错误页面的设置已经被移除
用户偏好设置暂时被移除，以后的版本可能会部分重新引入
文件浏览中的远程打开功能已经被移除
系统信息现在不再支持显示CPU实时频率的相关信息
系统信息中的Easter Eggs模块已被移除

#Remote Controller 3.0 Release Candidate 2 Build 441 (2024/07/25)
*新功能
管理员现在可以手动设置是否向用户展示新增功能
*Bug修复
修复了可能同时打开多个Remote Agent相关进程的问题
修复了文件浏览的磁盘列表在列出空驱动器时出现乱码的问题
修复了有些时候不能获取到实时壁纸的问题
修复了可能无法准确呈现用户最近使用时间的问题

#Remote Controller 3.0 Release Candidate 1 Build 437 (2024/07/18)
*新功能
完善了访客用户的权限限制，现在访客用户只能进行只读操作
基本控制工具中引入了壁纸设置模块
完善了Remote Controller基本日志记录以及日志管理功能
引入了程序更新模块，现在可以远程更新Remote Controller
引入了存储管理模块，管理员现在可以清理Remote Controller的临时文件和部分数据文件
*Bug修复
修复了实时剪贴板无法被刷新的问题
修复了通过基本控制休眠计算机时无法正确返回操作执行状态的问题
修复了远程计算机执行电源操作时弹出控制台窗口的问题
修复了重置系统模块没有删除屏幕截图的问题

#Remote Controller 3.0 Beta 4 Build 406 (2024/07/10)
*新功能
引入了全新的剪贴板和网络设置工具
基本控制工具中引入了音量控制模块
添加了系统设置模块以及多个设置项
添加了禁用、重置系统的功能
在被控系统前端引入了可选的托盘图标
简化了新用户的默认密码和修改密码的程序
新增了主页中的欢迎栏
在进行Remote Controller更新后，每个用户将看到新增功能页面
*Bug修复
修复了部分页面黑暗模式不完全的问题
修复了文件浏览在竖屏模式下边栏显示不当的问题
修复了Safari下载远程计算机文件出现文件格式错误的问题

#Remote Controller 3.0 Beta 3 Build 368 (2024/06/16)
*新功能
新增对黑暗模式的支持
引入了全新的基本控制、发送消息工具
引入顶栏被控电脑标识
优化了登录页面背景的显示效果
在屏幕截图工具中引入了新的预览工具
初步引入了Remote Controller日志
*Bug修复
修复了页面部分JavaScript代码未能加载的问题
修复了进程管理和窗口管理列表自动刷新导致的抖动问题

#Remote Controller 3.0 Beta 2 Build 308 (2024/04/26)
*新功能
引入了全新的进程管理、窗口管理工具
进一步优化了页面加载机制
完善了账户设置页面，引入了修改密码选项

#Remote Controller 3.0 Beta 1 Build 279 (2024/04/23)
*新功能
启用全新界面与页面加载机制，从零开始重构代码
引入了全新的屏幕截图、文件浏览、系统信息工具
初步引入了用户管理模块
引入了从旧版Coral Remote Controller进行文件格式升级的支持

#Remote Controller 2.0 Beta 5 Build 162 (2022/01/20)
(由于源代码丢失，该版为2.0的最终版本，之后的版本从Build 200开始从零重构代码)
*新功能
引入了对深色模式的支持
引入了发送消息、剪贴板模块，支持发送自定义消息和查看实时和以前的剪贴板信息
更新了主页的设计，并将其重命名为仪表盘
引入用户偏好设置模块，支持用户自定义部分外观
引入系统设置模块，支持自定义系统错误页面
优化了文件浏览等工具的设计
优化了日志模块，并更改了部分日志的储存位置

#Remote Controller 1.0 Release (2021/07/02)
*新功能
自2021年初以来，陆续引入了进程管理、窗口管理、网络设置、屏幕截图、电源控制、系统信息、文件浏览等工具
引入了用户权限模型，分为超级管理员、管理员、普通用户和访客用户
引入了系统控制、用户管理和日志查看的后台管理功能
)";

    Code = R"(
<div><p>
)";

    tempIO << Source;
    getline(tempIO, line);
    while (!tempIO.eof())
    {
        getline(tempIO, line);

        if (line == "")
            Code += "<br>\n";
        else if (line[0] == '#')
            Code += "</p><p><h4><b class=\"text-primary\">" + line.substr(1) + "</b></h4>\n";
        else if (line[0] == '*')
            Code += "</p><p><h5><b>" + line.substr(1) + "</b></h5>\n";
        else
            Code += line + "<br>\n";
    }

    Code += R"(
</p></div>
)";

    AJAXOutput(Code);

    return 0;
}