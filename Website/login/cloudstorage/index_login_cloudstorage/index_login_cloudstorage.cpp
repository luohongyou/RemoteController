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
<style>
  .code-input-container {
    display: flex;
  }
  .code-input {
    width: 50px;
    height: 25px;
    font-weight:700;
    font-size:35px; 
    margin: 0 3px;
    text-align: center;
    padding: .1rem .1rem !important; 
  }
</style>
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
              <code class="form-floating mb-3">
                <div class="code-input-container">
                <input class="form-control form-control-lg rounded-3 text-primary code-input" maxlength="1" type="text" id="PIN1" >
                <input class="form-control form-control-lg rounded-3 text-primary code-input" maxlength="1" type="text" id="PIN2" >
                <input class="form-control form-control-lg rounded-3 text-primary code-input" maxlength="1" type="text" id="PIN3" >
                <input class="form-control form-control-lg rounded-3 text-primary code-input" maxlength="1" type="text" id="PIN4" >
                <input class="form-control form-control-lg rounded-3 text-primary code-input" maxlength="1" type="text" id="PIN5" >
                <input class="form-control form-control-lg rounded-3 text-primary code-input" maxlength="1" type="text" id="PIN6" >
                </div>
              </code>
              &nbsp;<div class="spinner-border spinner-border text-primary" id="pinspinner" role="status" style="display:none"></div>
            </ul>
          </div>
        </div>

        </div>

      </div>
    </div>

    <script>
        if (getCookie("CORAL_REMOTE_CONTROLLER_MANAGE") != "") window.location.href = "/";
        document.querySelectorAll('input[type="text"]').forEach((input, index) => {
          input.addEventListener('input', (e) => {
            var thisValue = document.querySelectorAll('input[type="text"]')[index].value;
            if (!(0 <= thisValue && thisValue <= 9))
              document.querySelectorAll('input[type="text"]')[index].value = "";
          });
          input.addEventListener('keydown', (e) => {
            e.preventDefault();
            if (e.key === 'Backspace') {
              document.querySelectorAll('input[type="text"]')[index].value = "";
              if (index > 0)
                document.querySelectorAll('input[type="text"]')[index - 1].focus();
            }
            else if (e.key === 'Delete')
                document.querySelectorAll('input[type="text"]')[index].value = "";
            else if (e.keyCode >= 48 && e.keyCode <= 57) {
              // e.preventDefault();
              document.querySelectorAll('input[type="text"]')[index].value = e.keyCode - 48;
              if (index < 5)
                document.querySelectorAll('input[type="text"]')[index + 1].focus();
              else {
                  var hiddenInput = document.createElement('input');
                  hiddenInput.type = 'text';
                  hiddenInput.style.position = 'absolute';
                  hiddenInput.style.left = '-9999px';
                  hiddenInput.style.top = '-9999px';
                  hiddenInput.style.opacity = '0';
                  hiddenInput.tabIndex = -1;
                  document.body.appendChild(hiddenInput);
                  hiddenInput.focus();
                  document.body.removeChild(hiddenInput);
                  hiddenInput = null;

                  setTimeout(CheckPIN(), 500);
              }
            }
            else if (e.key === 'ArrowLeft' && index > 0) {
              document.querySelectorAll('input[type="text"]')[index - 1].focus();
            }
            else if (e.key === 'ArrowRight' && index < 5) {
              document.querySelectorAll('input[type="text"]')[index + 1].focus();
            }
          });
        });

        function CheckPIN() {
            var PIN = document.getElementById('PIN1').value + 
                      document.getElementById('PIN2').value + 
                      document.getElementById('PIN3').value + 
                      document.getElementById('PIN4').value + 
                      document.getElementById('PIN5').value + 
                      document.getElementById('PIN6').value;
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
                }
                else
                {
)" + (DataTracker ? R"(
            document.getElementById('PIN1').disabled = true;
            document.getElementById('PIN2').disabled = true;
            document.getElementById('PIN3').disabled = true;
            document.getElementById('PIN4').disabled = true;
            document.getElementById('PIN5').disabled = true;
            document.getElementById('PIN6').disabled = true;
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
