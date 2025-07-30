// index_power.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/power/"

#include <iostream>
#include "../volume/DeviceVolume.h"
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\");

    HTMLFrame HTML;
    HTML.Register(PT_INDEX, ADDRESS, "基本控制", "基本控制", R"(<li><a href="/">主页</a></li><li>基本控制</li>)", true);

    IMMDevice* device;
    IAudioEndpointVolume* endpointVolume;
    CoInitializeEx(NULL, COINIT_MULTITHREADED);
    device = GetDefaultAudioDevice();
    endpointVolume = GetAudioEndpointVolume(device);

    std::string Volume = Num2String(GetCurrentVolume(endpointVolume));
    bool IsMuted = GetDeviceState(endpointVolume);

    endpointVolume->Release();
    device->Release();
    CoUninitialize();

    bool BlackScreenFlag = GetSettings("BlackScreen", false);

    HTML.Custom((std::string)R"(
    <div class="portfolio-info">

        <ul class="nav nav-underline">
          <li class="nav-item">
            <a class="nav-link active" data-bs-toggle="tab" data-bs-target="#power-pane" role="tab">电源控制</a>
          </li>
          <li class="nav-item">
            <a class="nav-link" data-bs-toggle="tab" data-bs-target="#volume-pane" role="tab">音量控制</a>
          </li>
          <li class="nav-item">
            <a class="nav-link" data-bs-toggle="tab" data-bs-target="#wallpaper-pane" role="tab">壁纸设置</a>
          </li>
          <li class="nav-item">
            <a class="nav-link" data-bs-toggle="tab" data-bs-target="#lock-pane" role="tab">屏幕锁定</a>
          </li>
        </ul>

        <br>
        <div class="tab-content">

          <div class="tab-pane fade show active" id="power-pane" role="tabpanel" tabindex="0">

            <div class="row">
              <div class="col-lg-4">
                <div class="form-floating mb-3">
                  <select class="form-select" id="Action" )" + (HTML.User.GetUserType() == UT_GUEST ? "disabled" : "") + R"(>
                    <option value="1" selected>关闭电源</option>
                    <option value="2">重新启动</option>
                    <option value="3">休眠</option>
                    <option value="0">取消计划的电源操作</option>
                  </select>
                  <label for="Action">操作</label>
                </div>
              </div>

              <div class="col-lg-4">
                <div class="form-floating mb-3">
                  <select class="form-select" id="IsForce" )" + (HTML.User.GetUserType() == UT_GUEST ? "disabled" : "") + R"(>
                    <option value="1" selected>强制执行</option>
                    <option value="0">非强制执行</option>
                  </select>
                  <label for="IsForce">是否强制执行</label>
                </div>
              </div>

              <div class="col-lg-4">
                <div class="form-floating mb-3">
                  <input type="number" class="form-control" id="Second" value="0" )" + (HTML.User.GetUserType() == UT_GUEST ? "disabled" : "") + R"(>
                  <label for="Second">执行时延（秒）</label>
                </div>
              </div>

              <div class="d-grid gap-2 d-md-flex justify-content-md-end">
                <button class="btn btn-success" type="button" onclick="Execute() " )" + (HTML.User.GetUserType() == UT_GUEST ? "disabled" : "") + R"(>执行</button>
              </div>
            </div>

          </div>

          <div class="tab-pane fade" id="volume-pane" role="tabpanel" tabindex="1">
            <div class="row">
              <div class="col-lg-10">
                <input type="range" class="form-range" min="0" max="100" step="1" value=")" + Volume + R"(" id="VolumeRange" )" + (IsMuted || HTML.User.GetUserType() == UT_GUEST ? "disabled" : "") + R"(>
              </div>
              <div class="col-lg-2">
                <div class="form-check form-switch">
                  <input class="form-check-input" type="checkbox" role="switch" id="IsMuted" )" + (IsMuted ? "checked" : "") + R"( )" + (HTML.User.GetUserType() == UT_GUEST ? "disabled" : "") + R"(>
                  <label class="form-check-label" for="IsMuted">静音</label>
                </div>
              </div>
            </div>
          </div>

          <div class="tab-pane fade" id="wallpaper-pane" role="tabpanel" tabindex="2">
            <div class="row g-4">
              <div class="col-lg-8">
                <h3>当前壁纸</h3>
                <div id="currentwallpaper"></div>
              </div>
              <div class="col-lg-4">
                <h3>更换壁纸</h3>
                <div id="wallpaperForm">
                  <form onsubmit="wallpapercustomSubmit(event) " class="row row-cols-lg-auto g-3 align-items-center" name="wallpaperuploadForm" enctype="multipart/form-data">
                    <div class="col-12">
                      <input class="form-control" id="wallpaperfile" type="file" name="file" accept="image/*" )" + (HTML.User.GetUserType() == UT_GUEST ? "disabled" : "") + R"(>
                    </div>
                    <div class="col-12">
                      <button type="submit" class="btn btn-primary" )" + (HTML.User.GetUserType() == UT_GUEST ? "disabled" : "") + R"(>更换</button>
                    </div>
                  </form>
                </div>
                <div class="progress" role="progressbar" id="wallpaperProgress" style="display:none">
                  <div class="progress-bar" style="width: 0%" id="uploadprogressbar" ></div>
                </div>
              </div>
            </div>
          </div>

          <div class="tab-pane fade" id="lock-pane" role="tabpanel" tabindex="3">
            <p id="lockpcstatus">远程计算机)" + (BlackScreenFlag ? "已被锁定" : "未被锁定") + R"(。</p>
)" + (HTML.User.IsForegroundUser() ? R"(<p><b class="text-danger">不支持使用前台用户锁定当前计算机。</b></p>)" : "") + R"(
            <button class="btn btn-)" + (BlackScreenFlag ? "warning" : "danger") + R"(" type="button" onclick="LockPC() " id="LockButton" )" + (HTML.User.GetUserType() == UT_GUEST || HTML.User.IsForegroundUser() ? "disabled" : "") + R"(>)" + (BlackScreenFlag ? "解锁" : "锁定") + R"(远程计算机</button>
            <div id="lockpcactions" )" + (BlackScreenFlag ? R"(style="display:none")" : "") + R"(>
              <div class="form-check">
                <input class="form-check-input" type="checkbox" id="IsCloseAll" )" + (HTML.User.GetUserType() == UT_GUEST || HTML.User.IsForegroundUser() ? "disabled" : "") + R"(>
                <label class="form-check-label" for="IsCloseAll">同时强制关闭所有窗口</label>
              </div>
              <div class="form-check">
                <input class="form-check-input" type="checkbox" id="IsNoSound" )" + (HTML.User.GetUserType() == UT_GUEST || HTML.User.IsForegroundUser() ? "disabled" : "") + R"(>
                <label class="form-check-label" for="IsNoSound">同时将远程计算机静音</label>
              </div>
            </div>
          </div>

        </div>
    </div>

    <script>
        // Power Section
        document.getElementById('Action').addEventListener('change', function() {
            var selectedValue = this.value;
            if (selectedValue == "0")  // cancel
            {
                document.getElementById("IsForce").selectedIndex = 1;
                document.getElementById("IsForce").disabled = true;
                document.getElementById("Second").value = 0;
                document.getElementById("Second").disabled = true;
            }
            else if (selectedValue == "1" || selectedValue == "2")  // shutdown / restart
            {
                document.getElementById("IsForce").disabled = false;
                document.getElementById("Second").disabled = false;
            }
            else if (selectedValue == "3")  // sleep
            {
                document.getElementById("IsForce").disabled = false;
                document.getElementById("Second").value = 0;
                document.getElementById("Second").disabled = true;
            }
        });
        document.getElementById('Second').addEventListener('change', function() {
            var secondValue = this.value;
            if (secondValue == "" || secondValue < 0)
                this.value = 0;
            else 
                this.value = Math.floor(secondValue);
        });

        function Execute()
        {
            swal({
              title: "执行电源操作",
              text: "确实要执行电源操作吗？\n\n操作：" 
                                + document.getElementById("Action").options[document.getElementById("Action").selectedIndex].text 
                + "\n执行方式：" + document.getElementById("IsForce").options[document.getElementById("IsForce").selectedIndex].text
                + "\n执行时延：" + document.getElementById("Second").value + "秒",
              icon: "warning",
              buttons: true,
              dangerMode: true,
              buttons: ["取消", "确定"],
            })
            .then((willExec) => {
              if (willExec) {
                var ans = GetTextByAJAX("/power/powercfg.exe?" + document.getElementById('Action').value 
                                                         + "+" + document.getElementById('IsForce').value 
                                                         + "+" + document.getElementById('Second').value);
                if (ans != undefined)
                    if (ans.substr(0, 3) == "Yes")
                    {
                      swal({
                        title: "执行电源操作成功",
                        text: "远程计算机可能即将关闭，Coral Remote Controller可能将暂时不可用。",
                        icon: "success",
                        buttons: false,
                        dangerMode: false,
                      });
                      return;
                    }
                swal({
                  title: "执行电源操作失败",
                  text: "出现意外错误，电源操作未被执行。",
                  icon: "error",
                  buttons: false,
                  dangerMode: true,
                });
              }
            });
        }

        // Volume Section
        document.getElementById('IsMuted').addEventListener('change', function() {
            var isMuted = this.checked;
            if (isMuted)
            {
                document.getElementById('VolumeRange').disabled = true;
                GetTextByAJAX("/power/volume.exe?0+1");
            }
            else
            {
                document.getElementById('VolumeRange').disabled = false;
                GetTextByAJAX("/power/volume.exe?0+0");
            }
        });
        document.getElementById('VolumeRange').addEventListener('change', function() {
            var Volume = document.getElementById('VolumeRange').value;
            GetTextByAJAX("/power/volume.exe?1+" + Volume);
        });

        // Wallpaper Section
        AsyncChangeTextByAJAX("currentwallpaper", "/power/getwallpaper.exe");
        
        function wallpapercustomSubmit(e) {
            e.preventDefault();

            document.getElementById('wallpaperProgress').style.display = "block";
            document.getElementById('wallpaperForm').style.display = "none";

            var file = document.getElementById('wallpaperfile').files[0];
            var formData = new FormData();
            formData.append("file", file);

            $.ajax({
                url: '/power/changewallpaper.exe',
                type: 'POST',
                cache: false,
                data: formData,
                contentType: false,
                processData: false,
                xhr: function() {
                    var xhr = $.ajaxSettings.xhr();
                    if (xhr.upload) {
                        xhr.upload.addEventListener('progress', function(event) {
                            var percentComplete = Math.floor((event.loaded / event.total) * 100);
                            $('#uploadprogressbar').width(percentComplete + '%').text(percentComplete + '%');
                        }, false);
                    }
                    return xhr;
                },
                success: function (response) {
                    document.getElementById('wallpaperProgress').style.display = "none";
                    document.getElementById('wallpaperForm').style.display = "block";
                    $('#uploadprogressbar').width('0%').text('0%');

                    if (response == "Yes") {
                      AsyncChangeTextByAJAX("currentwallpaper", "/power/getwallpaper.exe");
                      document.getElementById('wallpaperfile').value = "";
                      swal({
                        title: "更换壁纸成功",
                        text: "远程计算机的壁纸现已更改",
                        icon: "success",
                        buttons: false,
                        dangerMode: false,
                      });
                    }
                    else {
                      swal({
                        title: "更换壁纸失败",
                        text: "请上传有效的文件，远程计算机的壁纸未更改",
                        icon: "error",
                        buttons: false,
                        dangerMode: false,
                      });
                    }

                },
                error: function (xhr, status, error) {
                    document.getElementById('wallpaperProgress').style.display = "none";
                    document.getElementById('wallpaperForm').style.display = "block";
                    $('#uploadprogressbar').width('0%').text('0%');

                    console.log('Error uploading file:', error);
                    swal({
                      title: "更换壁纸失败",
                      text: "上传文件时出现错误，远程计算机的壁纸未更改",
                      icon: "error",
                      buttons: false,
                      dangerMode: false,
                    });
                }
            });
        }

        // Lock Section
        var IsLocked = )" + (BlackScreenFlag ? "1" : "0") + R"(;
        function LockPC()
        {
            swal({
              title: IsLocked ? "解除远程计算机锁定" : "锁定远程计算机",
              text: IsLocked ? "确实要解除远程计算机锁定吗？远程计算机将恢复正常使用。" : "确实要锁定远程计算机吗？远程计算机将显示全屏禁用弹窗，且鼠标键盘将被禁用。",
              icon: "warning",
              buttons: true,
              dangerMode: true,
              buttons: ["取消", "确定"],
            })
            .then((willExec) => {
              if (willExec) {
                IsLocked = (IsLocked ? 0 : 1);
                var ActionNum = 0;
                if (document.getElementById('IsCloseAll').checked) ActionNum += 2;
                if (document.getElementById('IsNoSound').checked) ActionNum += 1;
                GetTextByAJAX("/power/lockpc.exe?" + IsLocked + "+" + ActionNum);

                document.getElementById('lockpcstatus').innerHTML = "远程计算机" + (IsLocked ? "已被锁定" : "未被锁定") + "。";
                document.getElementById('LockButton').className = "btn btn-" + (IsLocked ? "warning" : "danger");
                document.getElementById('LockButton').innerHTML = (IsLocked ? "解锁" : "锁定") + "远程计算机";
                document.getElementById('lockpcactions').style.display = (IsLocked ? "none" : "block");
              }
            });
        }
    </script>
)");
    HTML.Output();

    return 0;
}
