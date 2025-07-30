// index_manage_main_settings.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/manage/main/settings/"

#include <iostream>
#include "../../../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\..\\..\\");

    HTMLFrame HTML;
    HTML.Register(PT_INDEX, ADDRESS, "系统设置", "系统设置", R"(<li><a href="/">主页</a></li><li><a href="/manage/main">后台管理</a></li><li>系统设置</li>)", true);
    HTML.ManageInit();

    int Tray = String2Num(RCAgent("gettraystatus", HTML.User.GetName(), ""));
    int IsLoginEvent = (bool)(GetSettings("loginevent", true) && Tray);
    int AutoRefresh = GetSettings("AutoRefresh", true);
    int AgentProtect = GetSettings("AgentProtect", true);
    int Log = GetSettings("Log", true);
    int DataTracker = GetSettings("DataTracker", true);
    int NewFeature = GetSettings("NewFeature", true);

    int StrongPassword = GetSettings("StrongPassword", false);
    int UserAgentCheck = GetSettings("UserAgentCheck", true);
    int IPAddressCheck = GetSettings("IPAddressCheck", true);
    int PasswordExpired = GetSettings("PasswordExpired", true);

    int DebugLogs = GetSettings("DebugLogs", false);
    int RealtimeScreen = GetSettings("RealtimeScreen", false);

    int SystemStatus = GetSettings("SystemStatus", true);

    std::string Code = (std::string)R"(
    <div class="portfolio-info">

      <div class="row gy-4">
        <div class="col-lg-6">

          <h5><b class="text-primary">基础设置</b></h5>
          <div class="form-check form-switch">
            <input class="form-check-input" type="checkbox" role="switch" id="Tray" )" + (Tray ? "checked" : "") + R"(>
            <label class="form-check-label" for="Tray">被控端显示托盘图标</label>
          </div>
          <div class="form-check form-switch">
            <input class="form-check-input" type="checkbox" role="switch" id="loginevent" )" + (IsLoginEvent ? "checked" : "") + R"( )" + (Tray ? "" : "disabled") + R"(>
            <label class="form-check-label" for="loginevent">有用户登录时在前台显示通知提示</label>
          </div>
          <div class="form-check form-switch">
            <input class="form-check-input" type="checkbox" role="switch" id="AutoRefresh" )" + (AutoRefresh ? "checked" : "") + R"(>
            <label class="form-check-label" for="AutoRefresh">在进程管理和窗口管理中启用自动刷新</label>
          </div>
          <div class="form-check form-switch">
            <input class="form-check-input" type="checkbox" role="switch" id="AgentProtect" )" + (AgentProtect ? "checked" : "") + R"(>
            <label class="form-check-label" for="AgentProtect">防止 Remote Agent 意外退出</label>
          </div>
          <div class="form-check form-switch">
            <input class="form-check-input" type="checkbox" role="switch" id="Log" )" + (Log ? "checked" : "") + R"( )" + (HTML.User.GetUserType() != UT_ROOT ? "disabled" : "") + R"(>
            <label class="form-check-label" for="Log">启用 Remote Controller 日志</label>
          </div>
          <div class="form-check form-switch">
            <input class="form-check-input" type="checkbox" role="switch" id="DataTracker" )" + (DataTracker ? "checked" : "") + R"( )" + (HTML.User.GetUserType() != UT_ROOT ? "disabled" : "") + R"(>
            <label class="form-check-label" for="Log">收集用户浏览器遥测数据</label>
          </div>
          <div class="form-check form-switch">
            <input class="form-check-input" type="checkbox" role="switch" id="NewFeature" )" + (NewFeature ? "checked" : "") + R"(>
            <label class="form-check-label" for="NewFeature">更新后向用户展示新增功能和改进</label>
          </div>
          <br>

          <h5><b class="text-primary">安全设置</b></h5>
          <div class="form-check form-switch">
            <input class="form-check-input" type="checkbox" role="switch" id="StrongPassword" )" + (StrongPassword ? "checked" : "") + R"(>
            <label class="form-check-label" for="StrongPassword">强制使用强密码</label>
          </div>
          <div class="form-check form-switch">
            <input class="form-check-input" type="checkbox" role="switch" id="PasswordExpired" )" + (PasswordExpired ? "checked" : "") + R"(>
            <label class="form-check-label" for="PasswordExpired">未使用的凭据自动过期</label>
          </div>
          <div class="form-check form-switch">
            <input class="form-check-input" type="checkbox" role="switch" id="IPAddressCheck" )" + (IPAddressCheck ? "checked" : "") + R"(>
            <label class="form-check-label" for="IPAddressCheck">验证用户的登录来自单一IP地址</label>
          </div>
          <div class="form-check form-switch">
            <input class="form-check-input" type="checkbox" role="switch" id="UserAgentCheck" )" + (UserAgentCheck ? "checked" : "") + R"(>
            <label class="form-check-label" for="UserAgentCheck">验证用户的登录来自单一浏览器</label>
          </div>

        </div>
        <div class="col-lg-6">

          <h5><b class="text-primary">实验性功能</b></h5>
          <div class="form-check form-switch">
            <input class="form-check-input" type="checkbox" role="switch" id="DebugLogs" )" + (DebugLogs ? "checked" : "") + R"(>
            <label class="form-check-label" for="DebugLogs">启用调试日志</label>
          </div>
          <div class="form-check form-switch">
            <input class="form-check-input" type="checkbox" role="switch" id="RealtimeScreen" )" + (RealtimeScreen ? "checked" : "") + R"(>
            <label class="form-check-label" for="RealtimeScreen">启用实时屏幕中的动态屏幕</label>
          </div>
          <br>

          <h5><b class="text-primary">可用性与重置</b></h5>
          <p>打开或关闭系统。关闭系统后，仅有管理员可通过手动输入“/login”地址登录并进入后台管理页面。其他用户与系统的其他模块将被禁用。<br>
          <button class="btn btn-warning" type="button" style="margin-top:8px" onclick="SwitchSystem() " id="SystemSwtichBtn">)" + (SystemStatus ? "关闭" : "启用") + R"( Coral Remote Controller</button></p>
          <p>清除Remote Controller的所有账户、设置、日志和临时文件，重置完毕后，您需要重新进行初始设置。<br>
          <button class="btn btn-danger" type="button" style="margin-top:8px" onclick="ResetSystem() " )" + (HTML.User.GetUserType() != UT_ROOT ? "disabled" : "") + R"(>重置 Coral Remote Controller</button></p>

        </div>

      </div>
    </div>

    <script>
        function Switch(id, status)
        {
            GetTextByAJAX("/manage/main/settings/settingswitcher.exe?" + id + "+" + status);
        }

        document.getElementById('Tray').addEventListener('change', function() {
            var status = 0;
            if (this.checked) status = 1;
            Switch("Tray", status);
            if (!this.checked) 
            {
                document.getElementById('loginevent').checked = false;
                document.getElementById('loginevent').disabled = true;
                Switch("loginevent", 0);
            }
            else document.getElementById('loginevent').disabled = false;
        });
        document.getElementById('loginevent').addEventListener('change', function() {
            var status = 0;
            if (this.checked) status = 1;
            Switch("loginevent", status);
        });
        document.getElementById('AutoRefresh').addEventListener('change', function() {
            var status = 0;
            if (this.checked) status = 1;
            Switch("AutoRefresh", status);
        });
        document.getElementById('AgentProtect').addEventListener('change', function() {
            var status = 0;
            if (this.checked) status = 1;
            Switch("AgentProtect", status);
        });
        document.getElementById('Log').addEventListener('change', function() {
            var status = 0;
            if (this.checked) status = 1;
            Switch("Log", status);
        });
        document.getElementById('DataTracker').addEventListener('change', function() {
            var status = 0;
            if (this.checked) status = 1;
            Switch("DataTracker", status);
        });
        document.getElementById('NewFeature').addEventListener('change', function() {
            var status = 0;
            if (this.checked) status = 1;
            Switch("NewFeature", status);
        });

        document.getElementById('StrongPassword').addEventListener('change', function() {
            var status = 0;
            if (this.checked)
            {
                status = 1;
                swal({
                  title: "更改密码复杂度要求",
                  text: "确实要强制使用强密码吗？修改设置后所有用户在下次登录时将被要求修改密码。",
                  icon: "warning",
                  buttons: true,
                  dangerMode: true,
                  buttons: ["取消", "确定"],
                })
                .then((IsChange) => {
                  if (IsChange) Switch("StrongPassword", status);
                  else this.checked = false;
                });
            }
            else Switch("StrongPassword", status);
        });
        document.getElementById('PasswordExpired').addEventListener('change', function() {
            var status = 0;
            if (this.checked) status = 1;
            Switch("PasswordExpired", status);
        });
        document.getElementById('UserAgentCheck').addEventListener('change', function() {
            var status = 0;
            if (this.checked) status = 1;
            Switch("UserAgentCheck", status);
        });
        document.getElementById('IPAddressCheck').addEventListener('change', function() {
            var status = 0;
            if (this.checked) status = 1;
            Switch("IPAddressCheck", status);
        });

        document.getElementById('DebugLogs').addEventListener('change', function() {
            var status = 0;
            if (this.checked) status = 1;
            Switch("DebugLogs", status);
        });
        document.getElementById('RealtimeScreen').addEventListener('change', function() {
            var status = 0;
            if (this.checked)
            {
                status = 1;
                swal({
                  title: "实验性功能 - 启用动态屏幕",
                  text: "确实要启用动态屏幕吗？该功能处于早期开发阶段，启用后将在后台持续占用CPU资源（可能导致续航和性能下降），并可能尚不稳定。",
                  icon: "warning",
                  buttons: true,
                  dangerMode: true,
                  buttons: ["取消", "确定"],
                })
                .then((IsChange) => {
                  if (IsChange) Switch("RealtimeScreen", status);
                  else this.checked = false;
                });
            }
            else Switch("RealtimeScreen", status);
        });

        function SwitchSystem()
        {
            swal({
              title: ")" + (SystemStatus ? "关闭" : "启用") + R"( Coral Remote Controller",
              text: "确实要)" + (SystemStatus ? "关闭" : "启用") + R"(Coral Remote Controller吗？)" + (SystemStatus ? "\\n关闭系统后，仅有管理员可通过手动输入“/login”地址登录并进入后台管理页面。其他用户与系统的其他模块将被禁用。" : "") + R"(",
              icon: "warning",
              buttons: true,
              dangerMode: true,
              buttons: ["取消", "确定"],
            })
            .then((IsChange) => {
              if (IsChange) {
                Switch("SystemStatus", )" + (SystemStatus ? "0" : "1") + R"();
                location.reload();
              }
            });
        }
        function ResetSystem()
        {
            swal({
              title: "重置 Coral Remote Controller",
              text: "确实要重置Coral Remote Controller吗？\n此操作不可逆，这将永久清空当前的所有用户、设置和数据！",
              icon: "warning",
              buttons: true,
              dangerMode: true,
              buttons: ["取消", "确定"],
            })
            .then((IsChange) => {
              if (IsChange) {
                GetTextByAJAX("/manage/main/settings/reset.exe");
                swal({
                    title: "成功",
                    text: "您已经成功重置了Coral Remote Controller，现在系统将引导您重新完成初始化设置。",
                    icon: "success",
                    buttons: false,
                    dangerMode: false,
                })
                .then(() => {
                    window.location.href = "/"
                });
              }
            });
        }
        
    </script>
)";

    HTML.Custom(Code);

    HTML.Output();
    return 0;

}