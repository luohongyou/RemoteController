// index_OOBE_user.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/OOBE/user/"

#include <iostream>
#include "../../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\..\\");

    HTMLFrame HTML;
    HTML.Register(PT_INDEX, ADDRESS, "欢迎", "欢迎", "", true);
    HTML.CustomFeatureList(R"(
        <ul>
          <li class="dropdown"><a href="javascript:void(0) " class="btn-learn-more"><span>以 )" + HTML.User.GetName() + R"( 身份登录</span> <i class="bi bi-chevron-down"></i></a>
            <ul>
              <li><a href="javascript:void(0) " onclick="CRLogout() ">退出登录</a></li>
            </ul>
          </li>
        </ul>
        <i class="bi bi-list mobile-nav-toggle"></i>
)");

    // 检查用户的OOBE状态
    if (HTML.User.IsOOBEed() >= OOBEVERSION)
    {
        AJAXOutput(R"(<script> window.location.href = "/"; </script>)");
        return 0;
    }

    int StrongPassword = GetSettings("StrongPassword", false);

    if (HTML.User.GetPasswordMD5() == "e10adc3949ba59abbe56e057f20f883e") // Step 1: 初始设置 - 设置新密码
        HTML.Custom((std::string)R"(
    <div class="p-5 mb-4 bg-body-tertiary rounded-3">
      <div class="container-fluid py-7">
        <div class="row" style="display:flex; align-items:center; justify-content:center;">

        <div class="col-lg-7">
          <h1 class="display-7 fw-bold">初始设置</h1>
          <p class="fs-6">欢迎使用Coral Remote Controller！现在程序将引导您完成用户初始化设置。<br>首先，您需要更换默认的密码。<br>您需要设置一个长度为)" + (StrongPassword ? "8位及以上，包含大小写字母和数字" : "6位及以上") + R"(的密码。</p>
        </div>
        <div class="col-lg-5">
          <div class="portfolio-info coralloginportfolio">
            <h3>账户密码</h3>
            <ul>
			  <p>请给您的账户设置一个新密码。</p>
              <form class="rcregister">
                <div class="form-floating mb-3">
                  <input class="form-control rounded-3" type="password" id="Password">
                  <label for="Password">密码</label>
                </div>
                <div class="form-floating mb-3">
                  <input class="form-control rounded-3" type="password" id="Password2">
                  <label for="Password2">确认密码</label>
                </div>
                <button type="submit" class="btn btn-primary">确定</button>
              </form>
            </ul>
          </div>
        </div>

        </div>

      </div>
    </div>

    <script src="/assets/js/jsencrypt.min.js"></script>
    <script>
        function base64ToBinary(base64) {
          const binaryString = atob(base64);
          const bytes = new Uint8Array(binaryString.length);
          for (let i = 0; i < binaryString.length; i++) {
            bytes[i] = binaryString.charCodeAt(i);
          }
          return bytes;
        }
        function bytesToHex(bytes) {
          return Array.from(bytes).map(b => b.toString(16).padStart(2, '0')).join('');
        }
        $('.rcregister').on('submit',function(e){
            e.preventDefault();
            var Password = document.getElementById("Password").value;
            var Password2 = document.getElementById("Password2").value;
            if (Password == "")
            {
                swal({
                  title: "请输入有效信息",
                  text: "密码不能为空",
                  icon: "error",
                  buttons: false,
                  dangerMode: true,
                });
                return;
            }
            if (Password != Password2)
            {
                swal({
                  title: "请输入有效信息",
                  text: "两次输入的密码不匹配",
                  icon: "error",
                  buttons: false,
                  dangerMode: true,
                });
                return;
            }
            if (Password == "123456")
            {
                swal({
                  title: "密码不够复杂",
                  text: "密码不能为默认密码",
                  icon: "error",
                  buttons: false,
                  dangerMode: true,
                });
                return;
            }
            if (!checkPasswordComplexity(Password, )" + (StrongPassword ? "true" : "false") + R"())
            {
                swal({
                  title: "密码不够复杂",
                  text: "请输入长度为)" + (StrongPassword ? "8位及以上，包含大小写字母和数字" : "6位及以上") + R"(的密码",
                  icon: "error",
                  buttons: false,
                  dangerMode: true,
                });
                return;
            }

            var encrypt = new JSEncrypt();
            encrypt.setPublicKey(GetTextByAJAX("/login/RSAPublicKey.exe"));
            var encrypted1 = encrypt.encrypt("123456");
            var encrypted2 = encrypt.encrypt(Password);

            const binary1 = base64ToBinary(encrypted1);
            const binary2 = base64ToBinary(encrypted2);
            const hexString1 = bytesToHex(binary1);
            const hexString2 = bytesToHex(binary2);

            GetTextByAJAX("/OOBE/user/newpassword.exe?" + hexString1 + "+" + hexString2);
            window.location.href = "/OOBE/user/";
        })
    </script>
)");
    // else if (...)    // Step 2: 初始设置 - 个性化设置

    else    // Step N: 新增功能
    {
        int NewFeature = GetSettings("NewFeature", true);

        if (NewFeature)
            HTML.Custom(R"(
    <svg xmlns="http://www.w3.org/2000/svg" class="d-none">
      <symbol id="window" viewBox="0 0 16 16">
        <path d="M4.5 6a.5.5 0 1 0 0-1 .5.5 0 0 0 0 1M6 6a.5.5 0 1 0 0-1 .5.5 0 0 0 0 1m2-.5a.5.5 0 1 1-1 0 .5.5 0 0 1 1 0"/>
        <path d="M12 1a2 2 0 0 1 2 2 2 2 0 0 1 2 2v8a2 2 0 0 1-2 2H4a2 2 0 0 1-2-2 2 2 0 0 1-2-2V3a2 2 0 0 1 2-2zM2 12V5a2 2 0 0 1 2-2h9a1 1 0 0 0-1-1H2a1 1 0 0 0-1 1v8a1 1 0 0 0 1 1m1-4v5a1 1 0 0 0 1 1h10a1 1 0 0 0 1-1V8zm12-1V5a1 1 0 0 0-1-1H4a1 1 0 0 0-1 1v2z"/>
      </symbol>
      <symbol id="shield" viewBox="0 0 16 16">
        <path d="M5.338 1.59a61 61 0 0 0-2.837.856.48.48 0 0 0-.328.39c-.554 4.157.726 7.19 2.253 9.188a10.7 10.7 0 0 0 2.287 2.233c.346.244.652.42.893.533q.18.085.293.118a1 1 0 0 0 .101.025 1 1 0 0 0 .1-.025q.114-.034.294-.118c.24-.113.547-.29.893-.533a10.7 10.7 0 0 0 2.287-2.233c1.527-1.997 2.807-5.031 2.253-9.188a.48.48 0 0 0-.328-.39c-.651-.213-1.75-.56-2.837-.855C9.552 1.29 8.531 1.067 8 1.067c-.53 0-1.552.223-2.662.524zM5.072.56C6.157.265 7.31 0 8 0s1.843.265 2.928.56c1.11.3 2.229.655 2.887.87a1.54 1.54 0 0 1 1.044 1.262c.596 4.477-.787 7.795-2.465 9.99a11.8 11.8 0 0 1-2.517 2.453 7 7 0 0 1-1.048.625c-.28.132-.581.24-.829.24s-.548-.108-.829-.24a7 7 0 0 1-1.048-.625 11.8 11.8 0 0 1-2.517-2.453C1.928 10.487.545 7.169 1.141 2.692A1.54 1.54 0 0 1 2.185 1.43 63 63 0 0 1 5.072.56"/>
        <path d="M10.854 5.146a.5.5 0 0 1 0 .708l-3 3a.5.5 0 0 1-.708 0l-1.5-1.5a.5.5 0 1 1 .708-.708L7.5 7.793l2.646-2.647a.5.5 0 0 1 .708 0"/>
      </symbol>
      <symbol id="tools" viewBox="0 0 16 16">
        <path d="M1 0 0 1l2.2 3.081a1 1 0 0 0 .815.419h.07a1 1 0 0 1 .708.293l2.675 2.675-2.617 2.654A3.003 3.003 0 0 0 0 13a3 3 0 1 0 5.878-.851l2.654-2.617.968.968-.305.914a1 1 0 0 0 .242 1.023l3.27 3.27a.997.997 0 0 0 1.414 0l1.586-1.586a.997.997 0 0 0 0-1.414l-3.27-3.27a1 1 0 0 0-1.023-.242L10.5 9.5l-.96-.96 2.68-2.643A3.005 3.005 0 0 0 16 3q0-.405-.102-.777l-2.14 2.141L12 4l-.364-1.757L13.777.102a3 3 0 0 0-3.675 3.68L7.462 6.46 4.793 3.793a1 1 0 0 1-.293-.707v-.071a1 1 0 0 0-.419-.814zm9.646 10.646a.5.5 0 0 1 .708 0l2.914 2.915a.5.5 0 0 1-.707.707l-2.915-2.914a.5.5 0 0 1 0-.708M3 11l.471.242.529.026.287.445.445.287.026.529L5 13l-.242.471-.026.529-.445.287-.287.445-.529.026L3 15l-.471-.242L2 14.732l-.287-.445L1.268 14l-.026-.529L1 13l.242-.471.026-.529.445-.287.287-.445.529-.026z"/>
      </symbol>
      <symbol id="code" viewBox="0 0 16 16">
        <path d="M10.478 1.647a.5.5 0 1 0-.956-.294l-4 13a.5.5 0 0 0 .956.294zM4.854 4.146a.5.5 0 0 1 0 .708L1.707 8l3.147 3.146a.5.5 0 0 1-.708.708l-3.5-3.5a.5.5 0 0 1 0-.708l3.5-3.5a.5.5 0 0 1 .708 0m6.292 0a.5.5 0 0 0 0 .708L14.293 8l-3.147 3.146a.5.5 0 0 0 .708.708l3.5-3.5a.5.5 0 0 0 0-.708l-3.5-3.5a.5.5 0 0 0-.708 0"/>
      </symbol>
    </svg>

    <div class="portfolio-info">

        <div>
          <h3 align="center"> Coral Remote Controller 已经成功更新到 <span class="text-primary"> )" + (std::string)VERSION + R"( </span> ！</h3>
        </div>

        <ul class="nav nav-underline">
          <li class="nav-item">
            <a class="nav-link active" data-bs-toggle="tab" data-bs-target="#feature-pane" role="tab">新增功能</a>
          </li>
          <li class="nav-item">
            <a class="nav-link" data-bs-toggle="tab" data-bs-target="#log-pane" role="tab">更新日志</a>
          </li>

        </ul>

        <br>
        <div class="tab-content">

          <div class="tab-pane fade show active" id="feature-pane" role="tabpanel" tabindex="0">
            <ul class="d-grid gap-4 my-5 list-unstyled small">
              <li class="d-flex gap-4">
                <svg class="bi text-primary flex-shrink-0" width="48" height="48" fill="currentColor"><use xlink:href="#window"/></svg>
                <div>
                  <h5 class="mb-0">文件管理和实时屏幕</h5>
                  许多以前就有的工具在本次更新中变得功能更加强大。现在您可以完全控制远程计算机上的文件，并查看远程计算机的实时动态屏幕。此外，文件管理中新增的共享云盘还支持使用PIN免账户访问，该设置可以在被控端托盘图标或用户管理中找到。
                </div>
              </li>
              <li class="d-flex gap-4">
                <svg class="bi text-primary flex-shrink-0" width="48" height="48" fill="currentColor"><use xlink:href="#tools"/></svg>
                <div>
                  <h5 class="mb-0">执行命令和内容推送</h5>
                 本次更新还添加了许多新工具。借助执行命令，您可以远程执行Windows批处理脚本。内容推送可以帮助您在远端计算机前端呈现指定的内容。
                </div>
              </li>
              <li class="d-flex gap-4">
                <svg class="bi text-primary flex-shrink-0" width="48" height="48" fill="currentColor"><use xlink:href="#shield"/></svg>
                <div>
                  <h5 class="mb-0">更安全的身份验证机制</h5>
                  Remote Controller 4.0大幅改进了身份验证机制，保证您的账户和远端计算机的安全。
                </div>
              </li>
            </ul>
          </div>

          <div class="tab-pane fade" id="log-pane" role="tabpanel" tabindex="2">
            <div id="changelog" style="overflow:auto; height:500px;"></div>
          </div>

        </div>

        <div class="d-grid gap-2 d-md-flex justify-content-md-end">
          <button class="btn btn-success" align="right" onclick="Finish() ">立即体验</button>
        </div>

    </div>

    <script>
        function Finish()
        {
            GetTextByAJAX("/OOBE/user/finishuserOOBE.exe");
            window.location.href = "/";
        }
        AsyncChangeTextByAJAX("changelog", "/OOBE/user/changelog.exe");
    </script>
)");
        else
            HTML.Custom(R"(
    <script>
        GetTextByAJAX("/OOBE/user/finishuserOOBE.exe");
        window.location.href = "/";
    </script>
)");
    }

    HTML.Output();

    return 0;
}

