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

    .pincontainer {
        display: flex;
        flex-direction: column;
        align-items: center;
        padding-top: calc(15 / 6.0 * 1vw);
    }
 
    .codeBox {
        width: calc(250 / 6.0 * 1vw);
        display: flex;
        justify-content: space-between;
        position: relative;
        margin-bottom: calc(15 / 6.0 * 1vw);
    }
 
    .codeItem {
        width: calc(50 / 6.0 * 1vw);
        height: calc(50 / 6.0 * 1vw);
        font-size: calc(24 / 6.0 * 1vw);
        text-align: center;
        line-height: calc(50 / 6.0 * 1vw);
        border: calc(1 / 6.0 * 1vw) solid #ccc;
        border-radius: calc(10 / 6.0 * 1vw);
        transition: all 0.2s linear;
        position: relative;
    }
 
    .codeInput {
        width: 100%;
        height: 100%;
        position: absolute;
        opacity: 0;
    }
 
    .pincontainer span:nth-child(1) {
        font-size: calc(20 / 6.0 * 1vw);
        margin-bottom: calc(10 / 6.0 * 1vw);
    }
 
    .pincontainer span:nth-child(2) {
        font-size: calc(12 / 6.0 * 1vw);
        margin-bottom: calc(30 / 6.0 * 1vw);
    }
 
    .pincontainer :last-child ,.pincontainer span .phone{
        font-size: calc(12 / 6.0 * 1vw);
        color: rgb(105, 161, 230);
    }
 
    .active {
        border: calc(3 / 6.0 * 1vw) solid rgb(105, 161, 230);
    }
 
    .codeItem.active::after {
        content: "";
        position: absolute;
        top: 50%;
        left: 50%;
        transform: translate(-50%, -50%);
        width: calc(2 / 6.0 * 1vw);
        height: calc(24 / 6.0 * 1vw);
        background-color: black;
        animation: blink 1s step-end infinite;
    }

@media (min-width: 992px) {
    .pincontainer {
        display: flex;
        flex-direction: column;
        align-items: center;
        padding-top: calc(15 / 12.0 * 1vw);
    }
 
    .codeBox {
        width: calc(250 / 12.0 * 1vw);
        display: flex;
        justify-content: space-between;
        position: relative;
        margin-bottom: calc(15 / 12.0 * 1vw);
    }
 
    .codeItem {
        width: calc(50 / 12.0 * 1vw);
        height: calc(50 / 12.0 * 1vw);
        font-size: calc(24 / 12.0 * 1vw);
        text-align: center;
        line-height: calc(50 / 12.0 * 1vw);
        border: calc(1 / 12.0 * 1vw) solid #ccc;
        border-radius: calc(10 / 12.0 * 1vw);
        transition: all 0.2s linear;
        position: relative;
    }
 
    .codeInput {
        width: 100%;
        height: 100%;
        position: absolute;
        opacity: 0;
    }
 
    .pincontainer span:nth-child(1) {
        font-size: calc(20 / 12.0 * 1vw);
        margin-bottom: calc(10 / 12.0 * 1vw);
    }
 
    .pincontainer span:nth-child(2) {
        font-size: calc(12 / 12.0 * 1vw);
        margin-bottom: calc(30 / 12.0 * 1vw);
    }
 
    .pincontainer :last-child ,.pincontainer span .phone{
        font-size: calc(12 / 12.0 * 1vw);
        color: rgb(105, 161, 230);
    }
 
    .active {
        border: calc(3 / 12.0 * 1vw) solid rgb(105, 161, 230);
    }
 
    .codeItem.active::after {
        content: "";
        position: absolute;
        top: 50%;
        left: 50%;
        transform: translate(-50%, -50%);
        width: calc(2 / 12.0 * 1vw);
        height: calc(24 / 12.0 * 1vw);
        background-color: black;
        animation: blink 1s step-end infinite;
    }
}
 
    @keyframes blink {
        50% {
            opacity: 0;
        }
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
              <div class="pincontainer">
                <div class="codeBox">
                    <div class="codeItem active"></div>
                    <div class="codeItem"></div>
                    <div class="codeItem"></div>
                    <div class="codeItem"></div>
                    <div class="codeItem"></div>
                    <div class="codeItem"></div>
                    <input type="text" class="codeInput" id="PIN" maxlength="6" type="number" />
                </div>
              </div>

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


        const codeItems = document.querySelectorAll(".codeItem");
        const inputBox = document.querySelector(".codeInput");
        inputBox.focus();

        inputBox.addEventListener("input", (e) => {
            changeActiveItem();
            updateCodeItem();
 
            if (inputBox.value.length === 6) {
                CheckPIN();
            }
 
        });

        function changeActiveItem() {
            codeItems.forEach((item) => {
                item.classList.remove("active");
            });
 
            const inputValue = inputBox.value.length;
 
            if (inputValue === 0) {
                codeItems[0].classList.add("active");
            }
 
            if (inputValue && inputValue < 6) {
                codeItems[inputValue].classList.add("active");
            }
        }

        function updateCodeItem() {
            const arrCode = inputBox.value;
            codeItems.forEach((item, index) => {
                arrCode[index] ? item.innerText = arrCode[index] : item.innerText = "";
            });
        }


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
