// index_manage_main_update.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/manage/main/update/"

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
    HTML.Register(PT_INDEX, ADDRESS, "程序更新", "程序更新", R"(<li><a href="/">主页</a></li><li><a href="/manage/main">后台管理</a></li><li>程序更新</li>)", true);
    HTML.ManageInit();
    if (HTML.User.GetUserType() != UT_ROOT)
        return 0;

    bool BlackScreenFlag = GetSettings("BlackScreen", false);

    int PackageBuild = -1;
    std::string PackageBranch, PackageVersion, PackageSize;
    std::ifstream fin("..\\..\\ManageUI\\update\\packages\\Coral Remote Controller\\BuildInfo.ini");
    if (!fin.fail())
    {
        fin >> PackageBuild;
        fin.get();
        getline(fin, PackageVersion);
        getline(fin, PackageBranch);

        PackageSize = size2string(get_folder_size("..\\..\\ManageUI\\update\\packages\\Coral Remote Controller"));
    }
    fin.close();

    std::string Code = (std::string)R"(
<div class="row gy-4">
  <div class="col-lg-8">
    <div class="portfolio-info">
      <h3>更新程序包</h3>

      <h5><b class="text-primary">已经上传的程序包</b></h5>
)" + (PackageBuild == -1 ? "<p> 未上传任何程序包。 </p>" : R"(
      <p> 版本：)" + PackageVersion + R"( (Build )" + Num2String(PackageBuild) + R"(.)" + PackageBranch + R"() <br>
          大小：)" + PackageSize + R"( </p>)") 
        + R"(

      <h5><b class="text-primary">上传程序包</b></h5>
      <div class="col-8">
        <div class="progress" role="progressbar" id="uploadprogress" style="display:none; margin:5px;">
          <div class="progress-bar" style="width: 0%" id="uploadprogressbar" ></div>
        </div>
      </div>
      <form class="row row-cols-lg-auto g-3 align-items-center" id="uploadForm" name="uploadForm" method="post" enctype="multipart/form-data" action="uploadpackage.exe">
        <div class="col-12">
          <input class="form-control" type="file" name="file">
        </div>
        <div class="col-12">
          <button type="submit" class="btn btn-primary">上传</button>
        </div>
      </form>

      <br>
      <h5><b class="text-primary">操作</b></h5>
      <p> 注意：此处的程序更新仅支持热修补Remote Controller部分的文件，Apache服务器文件不会更新。您可以手动在远程计算机上更新这些文件。<br>
)" + ((PackageBuild >= PROGRAM_BUILD || PackageBuild == -1) ? "" : "<b class=\"text-danger\"> 当前上传的包比现有版本更旧，更新无法进行。</b> <br>") + R"(
)" + (!BlackScreenFlag ? "" : "<b class=\"text-danger\"> 请先转到“基本控制”解除对远程计算机的锁定，然后再执行更新。</b> <br>") + R"(
      <button onclick="Update() " id="updateBtn" style="margin-top:8px" class="btn btn-success")" + ((PackageBuild >= PROGRAM_BUILD) && !BlackScreenFlag ? "" : "disabled") + R"(>执行更新</button></p>
      <div class="col-8">
        <div class="progress" role="progressbar" id="updateprogress" style="display:none; margin:5px;">
          <div class="progress-bar progress-bar-striped progress-bar-animated" style="width: 100%">正在更新...</div>
        </div>
      </div>

    </div>
  </div>
  <div class="col-lg-4">
    <div class="portfolio-info">
      <h3>当前版本</h3>
      <div> <b>Remote Controller 版本：</b><br>)" + VERSION + R"( (Build )" + PROGRAM_BUILD_S + R"(.)" + BRANCH + R"()  </div>
      <div> <b>编译信息：</b><br>)" + GetCompileInfo() + R"( </div>
    </div>
  </div>
</div>

<script>
    $(document).ready(function() {
        $('#uploadForm').submit(function(e) {
            e.preventDefault();
            document.getElementById('uploadprogress').style.display = "block";
            document.getElementById('uploadForm').style.display = "none";
            var formData = new FormData($(this)[0]);
            $.ajax({
                url: $(this).attr('action'),
                type: 'POST',
                data: formData,
                contentType: false,
                processData: false,
                xhr: function() {
                    var xhr = $.ajaxSettings.xhr();
                    if (xhr.upload) {
                        xhr.upload.addEventListener('progress', function(event) {
                            var percentComplete = Math.floor((event.loaded / event.total) * 100);
                            $('#uploadprogressbar').width(percentComplete + '%').text(percentComplete + '%');
                        }, false);
                    }
                    return xhr;
                },
                success: function(data) {
                    location.reload();
                },
                error: function() {
                    document.getElementById('uploadprogress').style.display = "none";
                    document.getElementById('uploadForm').style.display = "block";
                    swal({
                        title: "错误",
                        text: "上传Coral Remote Controller更新包时出现意外错误。",
                        icon: "error",
                        buttons: false,
                        dangerMode: false,
                    });
                }
            });
        });
    });

    function Update()
    {
        swal({
          title: "更新 Coral Remote Controller",
          text: "确实要将Coral Remote Controller组件更新到)" + PackageVersion + R"( (Build )" + Num2String(PackageBuild) + R"(.)" + PackageBranch + R"()吗？\n更新过程中，请勿刷新页面。\n此操作不可逆，您将不能再回滚到当前版本！",
          icon: "warning",
          buttons: true,
          dangerMode: true,
          buttons: ["取消", "确定"],
        })
        .then((IsChange) => {
          if (IsChange) {
            document.getElementById('updateprogress').style.display = "block";
            document.getElementById('updateBtn').style.display = "none";

            $.ajax({
                type:'get', 
                url:"/temp/rcupdater.exe",
                async:true,
                success:function(ans) {
                    document.getElementById('updateprogress').style.display = "none";
                    document.getElementById('updateBtn').style.display = "block";
                    if (ans == "Yes")
                    {
                        swal({
                            title: "成功",
                            text: "您已经成功更新了Coral Remote Controller，现在您将返回登录页面。",
                            icon: "success",
                            buttons: false,
                            dangerMode: false,
                        })
                       .then(() => {
                            window.location.href = "/";
                        });
                    }
                    else
                    {
                        swal({
                            title: "错误",
                            text: "更新Coral Remote Controller时出现意外错误。",
                            icon: "error",
                            buttons: false,
                            dangerMode: false,
                        });
                    }
                }
            })

          }
        });
    }
</script>
)";

    HTML.Custom(Code);

    HTML.Output();
    return 0;

}
