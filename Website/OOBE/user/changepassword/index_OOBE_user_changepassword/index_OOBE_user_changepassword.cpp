// index_OOBE_user_changepassword.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/OOBE/user/changepassword/"

#include <iostream>
#include "../../../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\..\\..\\");

    HTMLFrame HTML;
    HTML.Register(PT_INDEX, ADDRESS, "更改密码", "更改密码", "", true);
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

    // 检查用户的密码状态
    bool ChangePassword;
    std::ifstream fin(HTML.User.Data("ChangePassword"));
    ChangePassword = !fin.fail();
    fin.close();

    if (!ChangePassword)
    {
        AJAXOutput(R"(<script> window.location.href = "/"; </script>)");
        return 0;
    }

    int StrongPassword = GetSettings("StrongPassword", false);

    HTML.Custom((std::string)R"(
    <div class="p-5 mb-4 bg-body-tertiary rounded-3">
      <div class="container-fluid py-7">
        <div class="row" style="display:flex; align-items:center; justify-content:center;">

        <div class="col-lg-7">
          <h1 class="display-7 fw-bold">密码需要修改</h1>
          <p class="fs-6">管理员已要求您修改您的Remote Controller密码。<br>您需要设置一个长度为)" + (StrongPassword ? "8位及以上，包含大小写字母和数字" : "6位及以上") + R"(的密码。</p>
        </div>
        <div class="col-lg-5">
          <div class="portfolio-info coralloginportfolio">
            <h3>账户密码</h3>
            <ul>
			  <p>请给您的账户设置一个新密码。</p>
              <form class="rcregister">
                <div class="form-floating mb-3">
                  <input class="form-control rounded-3" type="password" id="OldPassword">
                  <label for="OldPassword">原密码</label>
                </div>
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
            var Old = document.getElementById("OldPassword").value;
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
            var encrypted1 = encrypt.encrypt(Old);
            var encrypted2 = encrypt.encrypt(Password);

            const binary1 = base64ToBinary(encrypted1);
            const binary2 = base64ToBinary(encrypted2);
            const hexString1 = bytesToHex(binary1);
            const hexString2 = bytesToHex(binary2);

            GetTextByAJAX("/OOBE/user/changepassword/forcechangepw.exe?" + hexString1 + "+" + hexString2);
            window.location.href = "/";
        })
    </script>
)");

    HTML.Output();

    return 0;
}

