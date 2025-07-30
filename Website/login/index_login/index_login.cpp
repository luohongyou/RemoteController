// index_login.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/login/"

#include <iostream>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\");

    HTMLFrame HTML;
    HTML.Register(PT_NOCRUMB, ADDRESS, "身份验证", "身份验证", "", false);

    std::string StatusCheck = RCAuth("RequestForegroundToken", "");

    int DataTracker = GetSettings("DataTracker", true);

    HTML.Custom((std::string)R"(
  <!-- ======= Hero Section ======= -->
  <section id="hero" class="d-flex align-items-center portfolio-details coralrcbackground">
    <div class="container">
      <div class="row">
        <div class="col-lg-7 pt-5 pt-lg-0 d-flex flex-column justify-content-center">
          <h1 data-aos="fade-up">欢迎使用<br><span class="text-primary">Coral Remote Controller</span></h1>
          <h2 data-aos="fade-up" data-aos-delay="400">借助Coral Remote Controller，您可以轻松地在浏览器上远程控制和管理远端计算机。</h2>
        </div>
        <div class="col-lg-5" data-aos="fade-left" data-aos-delay="200">
          <div class="portfolio-info coralloginportfolio">
              <h3>身份验证</h3>
              <ul>
				<p>请输入密码，以验证您对远程计算机的访问权限。</p>
)" + (StatusCheck == "CORAL_RC_AUTH_FAILED" ? R"(<div class="alert alert-danger" role="alert">Remote Controller 身份验证管理器疑似没有运行，请稍作等待，然后刷新页面重试。如果问题仍然存在，请尝试重新安装 Remote Controller，或联系系统管理员。</div>)" : R"(
                <form class="rclogin">
				    <div class="input-group mb-3">
  					    <span class="input-group-text">用户</span>
  					    <input type="text" class="form-control" id="CoralUser">
				    </div>
				    <div class="input-group mb-3">
  					    <span class="input-group-text">密码</span>
  					    <input type="password" class="form-control" id="CoralPassword">
				    </div>
				    <p><button id="loginbutton" class="getstarted" type="submit">登录</button>)" + (std::string)(GetSettings("CloudStoragePIN", false) ? R"(&nbsp;&nbsp;&nbsp;
                    <a href="/login/cloudstorage" one-link-mark="yes">用PIN登录共享云盘</a>)" : "") + R"(</p>
                </form>)") + R"(
			  </ul>
          </div>
        </div>
      </div>
    </div>
    
  </section><!-- End Hero -->

  <main id="main">
    <br>
    <br>
  </main><!-- End #main -->

  <script src="/assets/js/jsencrypt.min.js"></script>
  <script>
    var TrackedData; 
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

    function ToMainPage(Token)
    {
        if (Token == "change") {
            window.location.href = "/OOBE/user/changepassword";
        }
        else {
            window.location.href = "/";
        }
    }
    function TrackData()
    {
        // Track User Data
        browser.getInfo().then(function(info){
            browser.getFingerprint().then(function(fingerprint){
                TrackedData = HexEncode(fingerprint.value) + '+' + HexEncode(JSON.stringify(info));
            });
        });
    }

)" + (DataTracker ? R"(            
    TrackData();
)" : "") + R"(

    var TrackFlag = false;
    function Login()
    {
	    var User = document.getElementById("CoralUser").value;
        var Password = document.getElementById("CoralPassword").value;
        if (User == "" || Password == "")
        {
	        swal({
                title: "用户名或密码错误",
                text: "请输入有效的用户名和密码",
                icon: "error",
                buttons: false,
                dangerMode: true,
            });
	        return;
        }

        document.getElementById('loginbutton').disabled = true;
        document.getElementById('loginbutton').innerHTML = '<div class="spinner-border spinner-border-sm text-light" role="status"></div>';

        var encrypt = new JSEncrypt();
        encrypt.setPublicKey(GetTextByAJAX("/login/RSAPublicKey.exe"));
        var encrypted = encrypt.encrypt(Password);

        const binary = base64ToBinary(encrypted);
        const hexString = bytesToHex(binary);

        var Token = GetTextByAJAX("/login/checkpassword.exe?" + User + "+" + hexString);
        if (Token == "false")
        {
            document.getElementById('loginbutton').disabled = false;
            document.getElementById('loginbutton').innerHTML = '登录';

            // 密码错误
            swal({
                title: "用户名或密码错误",
                text: "请重新输入用户名和密码",
                icon: "error",
                buttons: false,
                dangerMode: true,
            });
        }
        else
        {
)" + (DataTracker ? R"(            
            if (TrackedData == undefined)
            {
                var _timeout = setTimeout(ToMainPage, 5000, Token);
                setInterval(function(timeoutID){
                    if (TrackedData != undefined && TrackFlag == false)
                    {
                        TrackFlag = true;
                        clearTimeout(timeoutID);
                        GetTextByAJAX("/login/datatracker.exe?" + TrackedData);
                        ToMainPage(Token);
                    }
                }, 100, _timeout);
                return;
            }
            else
                GetTextByAJAX("/login/datatracker.exe?" + TrackedData);
)" : "") + R"(
            ToMainPage(Token);
        } 
    }
    $('.rclogin').on('submit',function(e){
        e.preventDefault();
        Login();
    })
    if (getCookie("CORAL_REMOTE_CONTROLLER_MANAGE") != "") window.location.href = "/";
  </script>
)");

    HTML.Output();
    return 0;
}
