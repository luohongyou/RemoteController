// index_manage_main_storage.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/manage/main/storage/"

#include <iostream>
#include <filesystem>
#include "../../../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../../../x64/Release/HTMLFrame.lib")
namespace fs = std::filesystem;

long long get_folder_size(const fs::path& folder_path)
{
    long long size = 0;
    for (const auto& entry : fs::recursive_directory_iterator(folder_path))
        if (entry.is_regular_file())
            size += entry.file_size();
    return size;
}
std::string size2string(__int64 fileSize)
{
    double GB = static_cast<double>(fileSize) / 1073741824.0;
    double MB = static_cast<double>(fileSize) / 1048576.0;
    double KB = static_cast<double>(fileSize) / 1024.0;
    double size;
    std::string unit;

    if (GB >= 1)
        size = GB, unit = "GB";
    else if (MB >= 1)
        size = MB, unit = "MB";
    else if (KB >= 1)
        size = KB, unit = "KB";
    else if (KB != 0)
        size = 1, unit = "KB";
    else
        size = 0, unit = "KB";

    std::stringstream tempIO;
    std::string Ans;
    tempIO << std::fixed << std::setprecision(unit == "GB" ? 2 : 0) << size;
    tempIO >> Ans;
    Ans += " " + unit;

    return Ans;
}

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\..\\..\\");

    HTMLFrame HTML;
    HTML.Register(PT_INDEX, ADDRESS, "存储管理", "存储管理", R"(<li><a href="/">主页</a></li><li><a href="/manage/main">后台管理</a></li><li>存储管理</li>)", true);
    HTML.ManageInit();

    LONGLONG All, Temp, Screenshot, UpdatePack, CloudStorage;
    All = get_folder_size("..\\..\\");
    Temp = get_folder_size("temp");
    Screenshot = get_folder_size("storage\\screenshot");
    CloudStorage = get_folder_size("storage\\sharedzone");
    UpdatePack = get_folder_size("..\\..\\ManageUI\\update\\packages");

    std::string Code = (std::string)R"(
<div class="row gy-4">
  <div class="col-lg-8">
    <div class="portfolio-info">

      <h3>存储清理</h3>

      <h5><b class="text-primary">临时文件</b></h5>
      <p>文件下载等模块产生的临时文件。<br>大小：)" + size2string(Temp) + R"(
      <button class="btn btn-sm btn-danger" type="button" onclick="Clean(1) ">删除</button></p>

      <h5><b class="text-primary">屏幕截图</b></h5>
      <p>用户截图产生的图片。<br>大小：)" + size2string(Screenshot) + R"(
      <button class="btn btn-sm btn-danger" type="button" onclick="Clean(2) ">删除</button></p>

      <h5><b class="text-primary">共享云盘</b></h5>
      <p>共享云盘上的所有文件和文件夹。<br>大小：)" + size2string(CloudStorage) + R"(
      <button class="btn btn-sm btn-danger" type="button" onclick="Clean(4) ">删除</button></p>

      <h5><b class="text-primary">更新程序包</b></h5>
      <p>上传的 Remote Controller 更新程序包。<br>大小：)" + size2string(UpdatePack) + R"(
      <button class="btn btn-sm btn-danger" type="button" onclick="Clean(3) " )" + (HTML.User.GetUserType() != UT_ROOT ? "disabled" : "") + R"(>删除</button></p>

    </div>
  </div>

  <div class="col-lg-4">
    <div class="portfolio-info">

      <h3>概述</h3>
      <p>Remote Controller 共占用远程计算机 <b>)" + size2string(All) + R"(</b> </p>

    </div>
  </div>
</div>

    <script>
        function Clean(ID)
        {
          swal({
            title: "清空存储",
            text: "确实要删除指定的存储吗？该操作不可逆。",
            icon: "warning",
            buttons: true,
            dangerMode: true,
            buttons: ["取消", "确定"],
          })
          .then((willDelete) => {
            if (willDelete) {
                GetTextByAJAX("/manage/main/storage/clearstorage.exe?" + ID);
                location.reload();
            }
          });
        }
    </script>
)";

    HTML.Custom(Code);

    HTML.Output();
    return 0;

}