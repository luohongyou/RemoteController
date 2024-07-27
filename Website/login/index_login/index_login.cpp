// index_login.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2024 Coral Studio. All Rights Reserved.

#define ADDRESS "/login/"

#include <iostream>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\");

    HTMLFrame HTML;
    HTML.Register(PT_NOCRUMB, ADDRESS, "身份验证", "身份验证", "", false);

    // 检查是否OOBE / 需要迁移配置
    std::ifstream fin;
    int _Version = 1;
    fin.open(DatabasePath() + "OOBEFlag");
    if (fin.fail())
    {
        fin.close();
        AJAXOutput(R"(<script> window.location.href = "/OOBE"; </script>)");
        return 0;
    }
    fin >> _Version;
    if (_Version < 3)
    {
        fin.close();
        AJAXOutput(R"(<script> window.location.href = "/OOBE/update"; </script>)");
        return 0;
    }
    fin.close();

    HTML.Custom(R"(
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
                <form class="rclogin">
				    <div class="input-group mb-3">
  					    <span class="input-group-text">用户</span>
  					    <input type="text" class="form-control" id="CoralUser">
				    </div>
				    <div class="input-group mb-3">
  					    <span class="input-group-text">密码</span>
  					    <input type="password" class="form-control" id="CoralPassword">
				    </div>
				    <input class="getstarted" type="submit" value="登录">
                </form>
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

  <script src="/assets/js/md5.js"></script>
  <script>
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
        var Token = GetTextByAJAX("/login/checkpassword.exe?" + User + "+" + md5(Password));
        if (Token == "false")
        {
            // 密码错误
            swal({
                title: "用户名或密码错误",
                text: "请重新输入用户名和密码",
                icon: "error",
                buttons: false,
                dangerMode: true,
            });
        }
        else if (Token == "change")
        {
            window.location.href = "/OOBE/user/changepassword";
        }
        else
        {
            // document.cookie = "CORAL_REMOTE_CONTROLLER_MANAGE=" + Token + "; path=/";
            window.location.href = "/";
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
