// index_OOBE_update.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/OOBE/update/"

#include <iostream>
#include "../../../../HTMLFrame/HTMLFrame.h"
#include "LegacyFormat.h"
#pragma comment(lib, "../../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\..\\");

    std::string Token = (argc == 2) ? argv[1] : "";

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
    if (_Version >= 4)
    {
        fin.close();
        AJAXOutput(R"(<script> window.location.href = "/"; </script>)");
        return 0;
    }
    fin.close();

    HTMLFrame HTML;
    if (_Version != 3)
        HTML.Register(PT_INDEX, ADDRESS, "迁移配置", "迁移配置", "", false);
    else
    {
        HTML.Register(PT_INDEX, ADDRESS, "迁移配置", "迁移配置", "", true);
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
        if (HTML.User.GetUserType() > UT_ADMIN)
        {
            HTML.Custom(R"(
    <div class="p-5 mb-4 bg-body-tertiary rounded-3">
      <div class="container-fluid py-7">
        <h1 class="display-7 fw-bold">需要执行更新后配置</h1>
        <p class="fs-6">请联系管理员以获取更多信息。</p>
      </div>
    </div>
)");
            HTML.Output();
            exit(0);
        }
    }

    if (_Version != 3)
    {
        if (Token != "")
        {
            LegacyUser User;
            User.LegacyLoadUser(Token);

            if (User.GetUserType() != UT_ROOT)
            {
                AJAXOutput(R"(<script> window.location.href = "/OOBE/update"; </script>)");
                return 0;
            }

            // 升级UI
            HTML.Custom(R"(
    <div class="p-5 mb-4 bg-body-tertiary rounded-3">
      <div class="container-fluid py-7">
        <div class="row" style="display:flex; align-items:center; justify-content:center;">

        <div class="col-lg-7">
          <h1 class="display-7 fw-bold">迁移配置</h1>
          <p class="fs-6">
            将Coral Remote Controller 1.0或2.0的配置文件升级至最新版本。<br>
            <b>可以迁移的数据：</b>现有的用户<br>
            <b>无法迁移的数据：</b>屏幕截图、剪贴板、系统日志<br>
            <b>已经弃用的内容：</b>错误页面设置、用户个性化设置<br>
            此外，您也可以选择不迁移数据，从头开始配置Coral Remote Controller。<br>
            <b>注意：完成配置迁移后，您将不能再使用旧版Coral Remote Controller。</b>
          </p>
        </div>
        <div class="col-lg-5">
          <div class="portfolio-info coralloginportfolio">
            <h3>配置迁移选项</h3>
            <ul>
			  <p>选择您想要迁移的现有数据与设置。</p>
              <div class="mb-3">
		        <select class="form-select" id="updateoption">
                  <option value="1" selected>保留现有用户</option>
                  <option value="0">删除所有现有内容</option>
                </select>
              </div>
              <button onclick="UpdateComfirm() " class="btn btn-primary">确定</button>
            </ul>
          </div>
        </div>

        </div>

      </div>
    </div>

    <script src="/assets/js/jsencrypt.min.js"></script>
    <script>
        function Update(Action)
        {
            GetTextByAJAX("/OOBE/update/updateformat.exe?" + Action + "+)" + Token + R"(");
            window.location.href = "/";
        }
        function UpdateComfirm()
        {
            var Action = document.getElementById("updateoption").value;
            if (Action == "0")
                swal({
                  title: "删除所有现有内容",
                  text: "确实要删除所有现有内容吗？该操作不可逆。",
                  icon: "warning",
                  buttons: true,
                  dangerMode: true,
                  buttons: ["取消", "确定"],
                })
                .then((willDelete) => {
                  if (willDelete) {
                      Update(Action);
                  }
                });
            else Update(Action);
        }
    </script>
)");
        }
        else // 1.0 - 2.0
        {
            // root身份验证UI
            HTML.Custom(R"(
    <div class="p-5 mb-4 bg-body-tertiary rounded-3">
      <div class="container-fluid py-7">
        <div class="row" style="display:flex; align-items:center; justify-content:center;">

        <div class="col-lg-7">
          <h1 class="display-7 fw-bold">迁移配置</h1>
          <p class="fs-6">欢迎升级到全新的Coral Remote Controller！在体验全新功能之前，Coral Remote Controller的配置文件需要进行更新。</p>
        </div>
        <div class="col-lg-5">
          <div class="portfolio-info coralloginportfolio">
            <h3>root 身份验证</h3>
            <ul>
			  <p>验证您的root身份后，系统将引导您迁移现有的数据与设置。</p>
              <form class="rclogin">
			    <div class="input-group mb-3">
  				  <span class="input-group-text">密码</span>
  				  <input type="password" class="form-control" id="RootPassword">
			    </div>
                <button type="submit" class="btn btn-primary">验证</button>
              </form>
            </ul>
          </div>
        </div>

        </div>

      </div>
    </div>

    <script src="/assets/js/md5.js"></script>
    <script>
        $('.rclogin').on('submit',function(e){
            e.preventDefault();
            var RootPassword = document.getElementById("RootPassword").value;
            if (RootPassword == "")
            {
                swal({
                  title: "请输入有效信息",
                  text: "root密码不能为空",
                  icon: "error",
                  buttons: false,
                  dangerMode: true,
                });
                return;
            }
            var Token = GetTextByAJAX("/OOBE/update/legacycheckroot.exe?" + md5(RootPassword));
            if (Token != "false")
                window.location.href = "/OOBE/update/?" + Token;
            else
                swal({
                  title: "密码错误",
                  text: "请输入正确的root密码",
                  icon: "error",
                  buttons: false,
                  dangerMode: true,
                });
        })
    </script>
)");
        } // end: 1.0 - 2.0
    }
    else // Version == 3
    {
        HTML.Custom(R"(
    <div class="p-5 mb-4 bg-body-tertiary rounded-3">
      <div class="container-fluid py-7">
        <div class="row" style="display:flex; align-items:center; justify-content:center;">

        <div class="col-lg-7">
          <h1 class="display-7 fw-bold">迁移配置</h1>
          <p class="fs-6">
            将Coral Remote Controller 3.0的配置文件升级至最新版本。<br>
            <b>可以迁移的数据：</b>所有内容<br>
            <b>注意：完成配置迁移后，您将不能再使用3.0版本的Coral Remote Controller。</b>
          </p>
        </div>
        <div class="col-lg-5">
          <div class="portfolio-info coralloginportfolio">
            <h3>配置迁移</h3>
            <ul>
			  <p>点击下面的按钮开始迁移。此操作不可逆。</p>
              <button onclick="Update() " class="btn btn-primary">迁移</button>
            </ul>
          </div>
        </div>

        </div>

      </div>
    </div>

    <script>
        function Update()
        {
            GetTextByAJAX("/OOBE/update/updateformat.exe?1+NEWVERSION");
            window.location.href = "/";
        }
    </script>
)");

    }

    HTML.Output();
    return 0;
}
