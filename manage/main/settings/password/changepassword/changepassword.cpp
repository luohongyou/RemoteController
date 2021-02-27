// changepassword.cpp : Coral Remote Controller 后台设置 更改密码模块

// 版权声明：
// Copyright (C) 2020-2021 Coral Studio. All Rights Reserved.

#include <iostream>
#include <fstream>
#include <Windows.h>
#include <string>
using namespace std;
ofstream fout;
int main(int argc, char* argv[])
{
    puts("Content-type: text/html\n");
    string Code = R"(
<!DOCTYPE html>
<html lang="zh">
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=gb2312">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>更改密码 - Coral Remote Controller</title>
    <!-- Bootstrap -->
    <link href="/css/bootstrap-4.4.1.css" rel="stylesheet">
  </head>
  <body>
    <nav class="navbar navbar-expand-lg navbar-light bg-light">
      <a class="navbar-brand" href="/">Coral Remote Controller</a>
      <button class="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarSupportedContent" aria-controls="navbarSupportedContent" aria-expanded="false" aria-label="Toggle navigation"> <span class="navbar-toggler-icon"></span> </button>
      </div>
    </nav>
    <header>
      <div class="jumbotron">
        <div class="container">
          <div class="row">
            <div class="col-12">
              <h1 class="text-center">更改密码</h1>
              <p class="text-center">
)";
    if (!strcmp(argv[1], "root"))
    {
        fout.open("js\\check_manage.js");
        fout << "var Code = \"" << argv[2] << "\";" << endl;
        fout << R"(

function getCookie(cname) {
	var name = cname + "=";
    var decodedCookie = decodeURIComponent(document.cookie);
    var ca = decodedCookie.split(';');
    for(var i = 0; i <ca.length; i++) {
        var c = ca[i];
        while (c.charAt(0) == ' ') {
            c = c.substring(1);
        }
        if (c.indexOf(name) == 0) {
            return c.substring(name.length, c.length);
        }
    }
    return "";
}

if (getCookie("CORAL_REMOTE_CONTROLLER_BACKGROUND_MANAGE") != Code)
{
	window.location.href="/manage/login";
}
)";
        fout.close();

        fout.open("js\\check_manage_login.js");
        fout << "var Code = \"" << argv[2] << "\";" << endl;
        fout << R"(

function getCookie(cname) {
	var name = cname + "=";
    var decodedCookie = decodeURIComponent(document.cookie);
    var ca = decodedCookie.split(';');
    for(var i = 0; i <ca.length; i++) {
        var c = ca[i];
        while (c.charAt(0) == ' ') {
            c = c.substring(1);
        }
        if (c.indexOf(name) == 0) {
            return c.substring(name.length, c.length);
        }
    }
    return "";
}

function OK()
{
	var Password = document.getElementById("CoralPassword").value;
	if (md5(Password) == Code){
		document.cookie = "CORAL_REMOTE_CONTROLLER_BACKGROUND_MANAGE=" + Code + "; path=/";
		window.location.href = "/manage/main";
	}
	else
		window.alert("Wrong password. Try again.");
	document.getElementById("CoralPassword").value = "";
}
)";
        fout.close();

        Code += "成功更改了Coral Remote Controller后台管理密码";
    }
    else if (!strcmp(argv[1], "user"))
    {
        fout.open("js\\check.js");
        fout << "var Code = \"" << argv[2] << "\";" << endl;
        fout << R"(

var Code_1 = Code;
function getCookie(cname) {
	var name = cname + "=";
    var decodedCookie = decodeURIComponent(document.cookie);
    var ca = decodedCookie.split(';');
    for(var i = 0; i <ca.length; i++) {
        var c = ca[i];
        while (c.charAt(0) == ' ') {
            c = c.substring(1);
        }
        if (c.indexOf(name) == 0) {
            return c.substring(name.length, c.length);
        }
    }
    return "";
}

if (getCookie("CORAL_REMOTE_CONTROLLER_MANAGE") != Code)
{
	window.location.href="/login";
}
)";
        fout.close();

        fout.open("js\\check_login.js");
        fout << "var Code = \"" << argv[2] << "\";" << endl;
        fout << R"(

function getCookie(cname) {
	var name = cname + "=";
    var decodedCookie = decodeURIComponent(document.cookie);
    var ca = decodedCookie.split(';');
    for(var i = 0; i <ca.length; i++) {
        var c = ca[i];
        while (c.charAt(0) == ' ') {
            c = c.substring(1);
        }
        if (c.indexOf(name) == 0) {
            return c.substring(name.length, c.length);
        }
    }
    return "";
}

function OK()
{
	var Password = document.getElementById("CoralPassword").value;
	if (md5(Password) == Code){
		document.cookie = "CORAL_REMOTE_CONTROLLER_MANAGE=" + Code + "; path=/";
		window.location.href = "/";
	}
	else
		window.alert("Wrong password. Try again.");
	document.getElementById("CoralPassword").value = "";
}
)";
        fout.close();

        Code += "成功更改了Coral Remote Controller身份验证密码";
    }    

    Code += R"(
</p>
              <center>
				  <a class="btn btn-outline-success my-2 my-sm-0" href="/manage/main/settings" title="" one-link-mark="yes">返回</a>
			  </center>
            </div>
          </div>
        </div>
      </div>
    </header>
    <footer class="text-center">
      <div class="container">
        <div class="row">
          <div class="col-12">
            <p>Copyright (c) 2020-2021 Coral Studio. All rights reserved.</p>
          </div>
        </div>
      </div>
    </footer>
    <!-- jQuery (necessary for Bootstrap's JavaScript plugins) --> 
    <script src="/js/jquery-3.4.1.min.js"></script> 
    <!-- Include all compiled plugins (below), or include individual files as needed --> 
    <script src="/js/popper.min.js"></script> 
    <script src="/js/bootstrap-4.4.1.js"></script>
  </body>
</html>
)";
    puts(Code.c_str());
    return 0;
}