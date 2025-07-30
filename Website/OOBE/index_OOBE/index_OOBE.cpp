// index_OOBE.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/OOBE/"

#include <iostream>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\");

    HTMLFrame HTML;
    HTML.Register(PT_INDEX, ADDRESS, "初始设置", "初始设置", "", false);

    // 检查是否已经OOBE
    std::ifstream fin;
    int _Version = 1;
    fin.open(DatabasePath() + "OOBEFlag");
    if (!fin.fail())
    {
        fin.close();
        AJAXOutput(R"(<script> window.location.href = "/"; </script>)");
        return 0;
    }

    HTML.Custom(R"(
    <div class="p-5 mb-4 bg-body-tertiary rounded-3">
      <div class="container-fluid py-7">
        <div class="row" style="display:flex; align-items:center; justify-content:center;">

        <div class="col-lg-7">
          <h1 class="display-7 fw-bold">欢迎</h1>
          <p class="fs-6">欢迎使用Coral Remote Controller！作为系统管理员，接下来您只需要完成填写下面的信息，就可以开始管理远程计算机了！</p>
        </div>
        <div class="col-lg-5">
          <div class="portfolio-info coralloginportfolio">
            <h3>root 账户密码</h3>
            <ul>
			  <p>设置root密码后，您可以在登录页面使用现在设置的密码登录root用户。借助该用户，您可以轻松地进行后台管理，并添加其他普通用户。</p>
              <form class="rcregister">
                <div class="form-floating mb-3">
                  <input class="form-control rounded-3" type="password" id="RootPassword">
                  <label for="RootPassword">密码</label>
                </div>
                <div class="form-floating mb-3">
                  <input class="form-control rounded-3" type="password" id="RootPassword2">
                  <label for="RootPassword2">确认密码</label>
                </div>
                <button type="submit" class="btn btn-primary">确定</button>
              </form>
            </ul>
          </div>
        </div>

        </div>

      </div>
    </div>

    <script src="/assets/js/md5.js"></script>
    <script>
        $('.rcregister').on('submit',function(e){
            e.preventDefault();
            var RootPassword = document.getElementById("RootPassword").value;
            var RootPassword2 = document.getElementById("RootPassword2").value;
            if (RootPassword == "")
            {
                swal({
                  title: "请输入有效信息",
                  text: "root密码不能为空",
                  icon: "error",
                  buttons: false,
                  dangerMode: true,
                });
                return;
            }
            if (RootPassword != RootPassword2)
            {
                swal({
                  title: "请输入有效信息",
                  text: "两次输入的root密码不匹配",
                  icon: "error",
                  buttons: false,
                  dangerMode: true,
                });
                return;
            }
            if (!checkPasswordComplexity(RootPassword, false))
            {
                swal({
                  title: "密码不够复杂",
                  text: "请输入长度为6位及以上的密码",
                  icon: "error",
                  buttons: false,
                  dangerMode: true,
                });
                return;
            }
            GetTextByAJAX("/OOBE/Initialize.exe?" + md5(RootPassword));
            window.location.href = "/";
        })
    </script>

)");

    HTML.Output();

    return 0;
}

