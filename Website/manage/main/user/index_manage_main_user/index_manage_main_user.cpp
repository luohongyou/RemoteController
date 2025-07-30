// index_manage_main_user.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/manage/main/user/"

#include <iostream>
#include "../../../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\..\\..\\");

    HTMLFrame HTML;
    HTML.Register(PT_INDEX, ADDRESS, "用户管理", "用户管理", R"(<li><a href="/">主页</a></li><li><a href="/manage/main">后台管理</a></li><li>用户管理</li>)", true);
    HTML.ManageInit();

    std::string Code = (std::string)R"(
    <div class="portfolio-info">
      <div class="row gy-4">
        <div class="col-lg-8 col-6">
          <h3>所有用户</h3>
        </div>
        <div class="col-lg-4 col-6" align="right">
          <a href="/manage/main/user/cloudstorage" class="btn btn-primary btn-sm" type="button">共享云盘访问管理</a>
          <button class="btn btn-success btn-sm" type="button" data-bs-toggle="modal" data-bs-target="#modalNewUser"><i class="bi bi-plus-lg"></i> 添加用户</button>
        </div>
      </div>
      <div id="userlist" style="overflow:auto; height:350px"></div>
    </div>

<div class="modal fade" tabindex="-1" data-bs-backdrop="static" role="dialog" id="modalNewUser">
  <div class="modal-dialog modal-dialog-centered" role="document">
    <div class="modal-content rounded-4 shadow">
      <div class="modal-header p-5 pb-4 border-bottom-0">
        <h1 class="mb-0 fs-4">添加用户</h1>
        <button type="button" class="btn-close" data-bs-dismiss="modal" aria-label="Close"></button>
      </div>

      <div class="modal-body p-5 pt-0">
        <form class="NewUser">
          <div class="form-floating mb-3">
            <input class="form-control rounded-3" id="UserName">
            <label for="UserName">用户名</label>
          </div>
          <div class="form-floating mb-3">
            <select class="form-select rounded-3" id="UserType">
              <option value="2" selected>标准用户</option>
)" + (HTML.User.GetUserType() == UT_ROOT ?
    R"(                      <option value="1">管理员</option>
)" : "") +
R"(                      <option value="3">访客用户</option>
            </select>
            <label for="UserType">权限</label>
          </div>
          <button class="w-100 mb-2 btn btn-lg rounded-3 btn-primary" type="submit" data-bs-dismiss="modal">确认</button>

        </form>
      </div>
    </div>
  </div>
</div>
<script>
    function LoadUserList()
    {
        AsyncChangeTextByAJAX("userlist", "/manage/main/user/listuser.exe");
    }
    $('.NewUser').on('submit',function(e){
        e.preventDefault();

        var User = document.getElementById("UserName").value;
        var Type = document.getElementById("UserType").value;

        if (User == "")
        {
            swal({
              title: "用户名无效",
              text: "请填写用户名",
              icon: "error",
              buttons: false,
              dangerMode: true,
            })
            .then(() => {
                $('#modalNewUser').modal('show');
            });
            return;
        }
        const regex = /[a-zA-Z0-9]{2,10}$/
        if (!regex.test(User))
        {
            swal({
              title: "用户名无效",
              text: "请输入有效的用户名：2-10位大小写字母/数字",
              icon: "error",
              buttons: false,
              dangerMode: true,
            })
            .then(() => {
                $('#modalNewUser').modal('show');
            });
            return;
        }

        var ans = GetTextByAJAX("/manage/main/user/adduser.exe?" + User + "+" + Type);
        if (ans.substring(0, 2) == "No")
        {
            swal({
              title: "创建用户失败",
              text: "已经有请求名称的用户存在",
              icon: "error",
              buttons: false,
              dangerMode: true,
            })
            .then(() => {
                $('#modalNewUser').modal('show');
            });
            return;
        }
        else
            swal({
              title: "创建用户成功",
              text: "默认密码为“123456”",
              icon: "success",
              buttons: false,
              dangerMode: false,
            });

        document.getElementById("UserName").value = "";
        LoadUserList();
    })
    function DeleteUser(Name)
    {
        swal({
          title: "删除用户",
          text: "确实要删除用户：" + Name + " ？",
          icon: "warning",
          buttons: true,
          dangerMode: true,
          buttons: ["取消", "确定"],
        })
        .then((IsDelete) => {
          if (IsDelete) {
            var ans = GetTextByAJAX("/manage/main/user/deleteuser.exe?" + Name);
            if (ans.substring(0, 2) == "No")
            {
                swal({
                  title: "删除用户失败",
                  text: "无法删除该用户",
                  icon: "error",
                  buttons: false,
                  dangerMode: true,
                });
                return;
            }
            else
                swal({
                  title: "成功删除用户",
                  text: "成功删除了用户：" + Name,
                  icon: "success",
                  buttons: false,
                  dangerMode: false,
                });
            LoadUserList();
          }
        });
    }
    LoadUserList();
</script>
)";

    HTML.Custom(Code);

    HTML.Output();
    return 0;

}