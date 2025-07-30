// index_manage_account.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/manage/account/"

#include <iostream>
#include "../../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\..\\");

    HTMLFrame HTML;
    HTML.Register(PT_INDEX, ADDRESS, "账户设置", "账户设置", R"(<li><a href="/">主页</a></li><li>账户设置</li>)", true);

    std::string Code;

    std::string UserAuthText;
    switch (HTML.User.GetUserType())
    {
    case UT_ROOT:
        UserAuthText = "超级管理员";
        break;
    case UT_ADMIN:
        UserAuthText = "管理员";
        break;
    case UT_USER:
        UserAuthText = "标准用户";
        break;
    case UT_GUEST:
        UserAuthText = "访客用户";
        break;
    }

    int StrongPassword = GetSettings("StrongPassword", false);

    Code = R"(
<div class="p-5 mb-4 bg-body-tertiary rounded-3">
    <div class="container-fluid py-7">
      <div class="row" style="display:flex; align-items:center; justify-content:center;">

        <div class="col-lg-8">
          <h1 class="display-6 fw-bold"><i class="bi bi-person-circle"></i>&nbsp;)" + HTML.User.GetName() + R"(</h1>
          <p class="fs-6">权限：)" + UserAuthText + R"(</p>
        </div>

        <div class="col-lg-4">
          <div class="col-sm-12 mb-sm-0">
            <div class="card">
              <div class="card-body">
                <div class="row">
                  <div class="col-lg-2">
                    <i class="bi bi-key text-primary" style="font-size:40px"></i>
                  </div>
                  <div class="col-lg-10">
                    <h5 class="card-title"><b>更改密码</b></h5>
                    <p class="card-text">更改账户的密码，以防止因为密码泄露造成的事故。</p>
                  </div>
                </div>
                <button data-bs-toggle="modal" data-bs-target="#modalChangePassword" class="btn btn-outline-primary btn-sm" style="margin-top:10px">更改</button>
              </div>
            </div>
          </div>
        </div>

      </div>
    </div>
  </div>


  <div class="modal fade" tabindex="-1" data-bs-backdrop="static" role="dialog" id="modalChangePassword">
    <div class="modal-dialog modal-dialog-centered" role="document">
      <div class="modal-content rounded-4 shadow">
        <div class="modal-header p-5 pb-4 border-bottom-0">
          <h1 class="mb-0 fs-4">更改密码</h1>
          <button type="button" class="btn-close" data-bs-dismiss="modal" aria-label="Close"></button>
        </div>

        <div class="modal-body p-5 pt-0">
          <form class="PasswordArea">
            <div class="form-floating mb-3">
              <input class="form-control rounded-3" id="OldPassword" type="password">
              <label for="OldPassword">原密码</label>
            </div>
            <div class="form-floating mb-3">
              <input class="form-control rounded-3" id="NewPassword" type="password">
              <label for="NewPassword">新密码</label>
            </div>
            <div class="form-floating mb-3">
              <input class="form-control rounded-3" id="NewPassword2" type="password">
              <label for="NewPassword2">重复新密码</label>
            </div>
            <button class="w-100 mb-2 btn btn-lg rounded-3 btn-primary" type="submit" data-bs-dismiss="modal">确认</button>
          </form>
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

  $('.PasswordArea').on('submit',function(e){
      e.preventDefault();

      var Old = document.getElementById("OldPassword").value;
      var New = document.getElementById("NewPassword").value;
      var New2 = document.getElementById("NewPassword2").value;

      if (Old == "" || New == "" || New2 == "")
      {
          swal({
            title: "填写的信息无效",
            text: "请完整填写所有信息",
            icon: "error",
            buttons: false,
            dangerMode: true,
          })
          .then(() => {
              $('#modalChangePassword').modal('show');
          });
          return;
      }
      if (New != New2)
      {
          swal({
            title: "填写的信息无效",
            text: "请输入两个相同的新密码",
            icon: "error",
            buttons: false,
            dangerMode: true,
          })
          .then(() => {
              $('#modalChangePassword').modal('show');
          });
          return;
      }

      if (New == "123456")
      {
          swal({
            title: "密码不够复杂",
            text: "密码不能为默认密码",
            icon: "error",
            buttons: false,
            dangerMode: true,
          })
          .then(() => {
              $('#modalChangePassword').modal('show');
          });
          return;
      }
      if (!checkPasswordComplexity(New, )" + (StrongPassword ? "true" : "false") + R"())
      {
          swal({
            title: "密码不够复杂",
            text: "请输入长度为)" + (StrongPassword ? "8位及以上，包含大小写字母和数字" : "6位及以上") + R"(的密码",
            icon: "error",
            buttons: false,
            dangerMode: true,
          })
          .then(() => {
              $('#modalChangePassword').modal('show');
          });
          return;
      }

      var encrypt = new JSEncrypt();
      encrypt.setPublicKey(GetTextByAJAX("/login/RSAPublicKey.exe"));
      var encrypted1 = encrypt.encrypt(Old);
      var encrypted2 = encrypt.encrypt(New);

      const binary1 = base64ToBinary(encrypted1);
      const binary2 = base64ToBinary(encrypted2);
      const hexString1 = bytesToHex(binary1);
      const hexString2 = bytesToHex(binary2);

      var ans = GetTextByAJAX("/manage/account/changepassword.exe?" + hexString1 + "+" + hexString2);
      if (ans.substring(0, 2) == "No")
      {
          swal({
            title: "原密码错误",
            text: "请输入正确的原密码",
            icon: "error",
            buttons: false,
            dangerMode: true,
          })
          .then(() => {
              $('#modalChangePassword').modal('show');
          });
          return;
      }
      else
          swal({
            title: "更改密码成功",
            text: "稍后您需要使用新密码重新登录Coral Remote Controller",
            icon: "success",
            buttons: false,
            dangerMode: false,
          })
          .then(() => {
              CRLogout();
          });
  })

</script>
)";

    HTML.Custom(Code);

    HTML.Output();
    return 0;
}