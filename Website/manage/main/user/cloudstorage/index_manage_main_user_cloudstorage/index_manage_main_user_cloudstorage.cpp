// index_manage_main_user_cloudstorage.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/manage/main/user/cloudstorage/"

#include <iostream>
#include "../../../../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\..\\..\\..\\");

    HTMLFrame HTML;
    HTML.Register(PT_INDEX, ADDRESS, "共享云盘访问管理", "共享云盘访问管理", R"(<li><a href="/">主页</a></li><li><a href="/manage/main">后台管理</a></li><li><a href="/manage/main/user">用户管理</a></li><li>共享云盘访问管理</li>)", true);
    HTML.ManageInit();

    int CloudStoragePIN = GetSettings("CloudStoragePIN", false);
    int CloudStoragePINEditable = GetSettings("CloudStoragePINEditable", false);

    std::string PIN = RCAuth("RequestCloudPIN", "");
    std::string Code = (std::string)R"(
<div class="row gy-4">
  <div class="col-lg-6">
    <div class="portfolio-info">
      <h3>PIN</h3>
      <p> 在登录页面选择“用PIN登录共享云盘”，输入该6位PIN，即可免账户访问共享云盘。<p>
      <)" + (CloudStoragePIN ? "code" : "p") + R"( class="text-primary" style="font-size:50px; font-weight:600">)" + (CloudStoragePIN ? PIN : "功能未启用") + R"(</)" + (CloudStoragePIN ? "code" : "p") + R"(>
    </div>
  </div>
  <div class="col-lg-6">
    <div class="portfolio-info">
      <h3>设置</h3>
      <div class="form-check form-switch">
        <input class="form-check-input" type="checkbox" role="switch" id="CloudStoragePIN" )" + (CloudStoragePIN ? "checked" : "") + R"(>
        <label class="form-check-label" for="CloudStoragePIN">允许通过PIN免账户访问共享云盘</label>
      </div>
      <div class="form-check form-switch">
        <input class="form-check-input" type="checkbox" role="switch" id="CloudStoragePINEditable" )" + (CloudStoragePINEditable ? "checked" : "") + R"(>
        <label class="form-check-label" for="CloudStoragePINEditable">通过PIN访问共享云盘的用户拥有编辑权限</label>
      </div>
    </div>
  </div>
</div>
<script>
    function Switch(id, status)
    {
        GetTextByAJAX("/manage/main/settings/settingswitcher.exe?" + id + "+" + status);
    }
    document.getElementById('CloudStoragePIN').addEventListener('change', function() {
        var status = 0;
        if (this.checked) status = 1;
        Switch("CloudStoragePIN", status);
        window.location.reload();
    });
    document.getElementById('CloudStoragePINEditable').addEventListener('change', function() {
        var status = 0;
        if (this.checked) status = 1;
        Switch("CloudStoragePINEditable", status);
    });
</script>
)";

    HTML.Custom(Code);

    HTML.Output();
    return 0;

}