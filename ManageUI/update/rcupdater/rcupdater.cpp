// rcupdater.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2024 Coral Studio. All Rights Reserved.

#define ADDRESS "/temp/rcupdater.exe" // 复制到该路径

#include <iostream>
#include <filesystem>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")
namespace fs = std::filesystem;

void copy_directory(const fs::path& src_dir, const fs::path& dst_dir) 
{
    if (!fs::exists(dst_dir)) 
        fs::create_directory(dst_dir);

    for (const auto& entry : fs::recursive_directory_iterator(src_dir)) 
    {
        const auto& path = entry.path();
        auto relative_path = path.lexically_relative(src_dir);
        fs::path dest_path = dst_dir / relative_path;

        if (fs::is_directory(path)) 
            fs::create_directories(dest_path);
        else if (fs::is_regular_file(path)) 
            copy_file(path, dest_path, fs::copy_options::overwrite_existing);
    }
}
int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\..\\..\\"); // .../Coral Remote Controller/

    HTMLFrame HTML;
    HTML.Register(PT_EMPTY, ADDRESS, "rcupdater", "rcupdater", "", true);
    HTML.ManageInit();
    if (HTML.User.GetUserType() != UT_ROOT)
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
    int PackageBuild = -1;
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
    if (PackageBuild < PROGRAM_BUILD)
    {
        printf("No");
        return 0;
    }

    // 执行更新
    // Remote Agent 自动退出和重启
    RCAgent("rcupdate", HTML.User.GetName(), ""); 
    // 复制文件
    copy_directory("ManageUI\\update\\packages\\Coral Remote Controller\\Website\\htdocs", "Website\\htdocs");
    copy_directory("ManageUI\\update\\packages\\Coral Remote Controller\\ManageUI", "ManageUI");
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
