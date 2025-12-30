// index_login_cloudstorage.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/login/cloudstorage/"

#include <iostream>
#include "../../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\..\\");

    HTMLFrame HTML;
    HTML.Register(PT_NOCRUMB, ADDRESS, "共享云盘", "身份验证 - 共享云盘", "", false);

    if (!GetSettings("CloudStoragePIN", false))
    {
        AJAXOutput(R"(<script> window.location.href = "/login"; </script>)");
        return 0;
    }

    int DataTracker = GetSettings("DataTracker", true);

    HTML.Custom((std::string)R"(
<main id="main">
  <!-- ======= Portfolio Details Section ======= -->
  <section id="portfolio-details" class="portfolio-details">
    <div class="container">

    <div class="p-5 mb-4 bg-body-tertiary rounded-3">
      <div class="container-fluid py-7">
        <div class="row" style="display:flex; align-items:center; justify-content:center;">

        <div class="col-lg-7">
          <h1 class="display-7 fw-bold">共享云盘</h1>
          <p class="fs-6">您可以使用共享云盘PIN码临时访问共享云盘<br>共享云盘PIN码可以在被控计算机上查询或系统管理员处取得<br>如果您有一个账户，请<a href="/login" one-link-mark="yes">登录</a>并使用“文件管理”工具访问共享云盘</p>
        </div>
        <div class="col-lg-5">
          <div class="portfolio-info coralloginportfolio">
            <h3>身份验证</h3>
            <ul>
			  <p>请输入6位共享云盘PIN码，验证您的访问权限。</p>
              <form class="rcpinlogin">
                <div class="form-floating mb-3">
                  <input class="form-control form-control-lg rounded-3 text-primary" type="number" id="PIN">
                  <label for="PIN">PIN</label>
                </div>
                <button type="submit" class="btn btn-primary" id="submitbutton">确定</button>
              </form>
              <div style="display:none" id="pinspinner">
                <div class="d-flex align-items-center">
                  <strong role="status">正在登录...</strong>
                  <div class="spinner-border ms-auto" aria-hidden="true"></div>
                </div>
              </div>

            </ul>
          </div>
        </div>

        </div>

      </div>
    </div>

    <script>
        if (getCookie("CORAL_REMOTE_CONTROLLER_MANAGE") != "") window.location.href = "/";

        $('.rcpinlogin').on('submit',function(e){
            e.preventDefault();
            CheckPIN();
        })

        function CheckPIN() {
            var PIN = document.getElementById('PIN').value;
            if (PIN.length == 6)
            {
                var Token = GetTextByAJAX("/login/cloudstorage/checkpin.exe?" + PIN);
                if (Token == "false")
                {
                    // PIN错误
                    swal({
                        title: "PIN错误",
                        text: "请重新输入PIN",
                        icon: "error",
                        buttons: false,
                        dangerMode: true,
                    });
                    document.getElementById('PIN').value = "";
                    updateCodeItem();
                }
                else
                {
)" + (DataTracker ? R"(
            document.getElementById('PIN').disabled = true;
            document.getElementById('submitbutton').style.display = "none"
            document.getElementById('pinspinner').style.display = "block"
            
            // Track User Data
            browser.getInfo().then(function(info){
                browser.getFingerprint().then(function(fingerprint){
                    GetTextByAJAX("/login/datatracker.exe?" + HexEncode(fingerprint.value) + '+' + HexEncode(JSON.stringify(info)));
)" : "") + R"(
                    window.location.href = "/explorer";
)" + (DataTracker ? R"(
                });
            });
)" : "") + R"(
                }
            } 
        }

    </script>

    </div>
  </section><!-- End Portfolio Details Section -->

</main><!-- End #main -->
)");

    HTML.Output();
    return 0;
}
