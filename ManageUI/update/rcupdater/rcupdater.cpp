// rcupdater.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/temp/rcupdater.exe" // 复制到该路径

#include <iostream>
#include <filesystem>
#include <chrono>
#include "../../../HTMLFrame/HTMLFrame.h"
#include "../../../../../Website/OOBE/update/index_OOBE_update/LegacyFormat.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")
namespace fs = std::filesystem;

HTMLFrame HTML;
std::chrono::steady_clock::time_point t1, t2;

void copy_directory(const fs::path& src_dir, const fs::path& dst_dir) 
{
    if (!fs::exists(dst_dir)) 
    {
        t1 = std::chrono::high_resolution_clock::now();
        fs::create_directory(dst_dir);
        t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
        HTML.Log("创建文件夹：" + dst_dir.string() + "<br>用时：" + Num2String(duration.count()) + " ms", "rcupdater", LL_DEBUG);
    }

    for (const auto& entry : fs::recursive_directory_iterator(src_dir)) 
    {
        const auto& path = entry.path();
        auto relative_path = path.lexically_relative(src_dir);
        fs::path dest_path = dst_dir / relative_path;

        if (fs::is_directory(path))
        {
            t1 = std::chrono::high_resolution_clock::now();
            fs::create_directories(dest_path);
            t2 = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
            HTML.Log("创建文件夹：" + dest_path.string() + "<br>用时：" + Num2String(duration.count()) + " ms", "rcupdater", LL_DEBUG);

        }
        else if (fs::is_regular_file(path)) 
        {
            t1 = std::chrono::high_resolution_clock::now();
            copy_file(path, dest_path, fs::copy_options::overwrite_existing);
            t2 = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
            HTML.Log("复制新文件：" + dest_path.string() + "<br>用时：" + Num2String(duration.count()) + " ms", "rcupdater", LL_DEBUG);
        }
    }
}
int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\..\\..\\"); // .../Coral Remote Controller/


    // 兼容 3.0
    bool IsLegacyUser = 1;
    LegacyUser LUser;
    std::string Token;
    const cgicc::CgiEnvironment& env = HTML.cgi.getEnvironment();
    for (auto cci : env.getCookieList())
    {
        if (cci.getName() == "CORAL_REMOTE_CONTROLLER_MANAGE")
        {
            Token = cci.getValue();
            break;
        }
    }
    LUser.LegacyLoadUser_v3(Token);
    if (LUser.GetToken() != "" && LUser.GetUserType() != UT_ROOT)
    {
        printf("No");
        return 0;
    }

    if (LUser.GetToken() == "")
    {
        HTML.Register(PT_EMPTY, ADDRESS, "rcupdater", "rcupdater", "", true);
        HTML.ManageInit();
        if (HTML.User.GetUserType() != UT_ROOT)
        {
            printf("No");
            return 0;
        }
        IsLegacyUser = 0;
    }

    if (GetSettings("BlackScreen", false))
        return 0;

    puts("Content-type: text/html\n");

    std::ifstream fin;
    // 检查是否已经更新
    fin.open("Website\\htdocs\\temp\\Updated.ini");
    if (!fin.fail())
    {
        fin.close();
        printf("No");
        return 0;
    }
    fin.close();

    // 检查更新包
    int PackageBuild = -1, OldPackageBuild = 0;
    std::string PackageBranch, PackageVersion;
    fin.open("ManageUI\\update\\packages\\Coral Remote Controller\\BuildInfo.ini");
    if (fin.fail())
    {
        fin.close();
        printf("No");
        return 0;
    }
    fin >> PackageBuild;
    fin.get();
    getline(fin, PackageVersion);
    getline(fin, PackageBranch);
    fin.close();

    fin.open("BuildInfo.ini");
    if (!fin.fail())
        fin >> OldPackageBuild;
    fin.close();

    if (PackageBuild < OldPackageBuild)
    {
        printf("No");
        return 0;
    }

    // 执行更新
    // Remote Agent 自动退出和重启
    RCAgent("rcupdate", IsLegacyUser ? LUser.GetName() : HTML.User.GetName(), "");
    // 退出前台UI
    StopProcess("LocalUI.exe");
    StopProcess("chatapp.exe");
    StopProcess("jsmpeg_vnc_coral.exe");
    // 复制文件
    copy_directory("ManageUI\\update\\packages\\Coral Remote Controller\\Website\\htdocs", "Website\\htdocs");
    copy_directory("ManageUI\\update\\packages\\Coral Remote Controller\\ManageUI", "ManageUI");
    CopyFileA("ManageUI\\update\\packages\\Coral Remote Controller\\Website\\conf\\httpd.conf", "Website\\conf\\httpd.conf", false);
    CopyFileA("ManageUI\\update\\packages\\Coral Remote Controller\\install.bat", "install.bat", false);
    CopyFileA("ManageUI\\update\\packages\\Coral Remote Controller\\uninstall.bat", "uninstall.bat", false);
    CopyFileA("ManageUI\\update\\packages\\Coral Remote Controller\\BuildInfo.ini", "BuildInfo.ini", false);

    // 删除更新文件
    RemoveDir("ManageUI\\update\\packages");
    CreateDirectoryA("ManageUI\\update\\packages", NULL);

    // 完成更新标记
    std::ofstream fout("Website\\htdocs\\temp\\Updated.ini");
    fout << PackageBuild;
    fout.close();

    printf("Yes");
    HTML.Log("已经完成 Remote Controller 更新<br>版本 " + PackageVersion + " (Build " + Num2String(PackageBuild) + "." + PackageBranch + ")", "manage", LL_INFO);

    return 0;

}
