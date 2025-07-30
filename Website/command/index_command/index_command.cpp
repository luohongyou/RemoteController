// index_command.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/command/"

#include <iostream>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\");

    HTMLFrame HTML;
    HTML.Register(PT_INDEX, ADDRESS, "执行命令", "执行命令", R"(<li><a href="/">主页</a></li><li>执行命令</li>)", true);

    HTML.Custom((std::string)R"(
    <div class="portfolio-info">

      <ul class="nav nav-underline">
        <li class="nav-item">
          <a class="nav-link active" data-bs-toggle="tab" data-bs-target="#bat-pane" role="tab">Windows 批处理脚本</a>
        </li>
      </ul>
      <br>

      <div class="tab-content">
      <div class="tab-pane fade show active" id="bat-pane" role="tabpanel" tabindex="0">
        
        <div class="row">

          <div class="col-lg-6">
            <code class="form-floating mb-3">
              <textarea class="form-control" id="Script" style="white-space:pre; height:350px" )" + (HTML.User.GetUserType() == UT_GUEST ? "disabled" : "") + R"(></textarea>
              <label for="Script">脚本</label>
            </code>
            <br>
          </div>

          <div class="col-lg-6">

            <div class="row mb-3">
              <div class="col-6">
                <div class="form-floating">
                  <select class="form-select" id="Authority" )" + (HTML.User.GetUserType() == UT_GUEST ? "disabled" : "") + R"(>
                    <option value="1" selected>前台用户</option>
                    <option value="2">系统服务</option>
                  </select>
                  <label for="Authority">运行权限</label>
                </div>
              </div>
              <div class="col-6">
                <div class="form-floating">
                  <select class="form-select" id="Interactive" )" + (HTML.User.GetUserType() == UT_GUEST ? "disabled" : "") + R"(>
                    <option value="0" selected>后台静默运行</option>
                    <option value="1">与前台用户交互</option>
                  </select>
                  <label for="Interactive">前台交互</label>
                </div>
              </div>
            </div>

            <code class="form-floating mb-3">
              <textarea class="form-control" id="Output" style="white-space:pre; height:275px" )" + (HTML.User.GetUserType() == UT_GUEST ? "disabled" : "") + R"( readonly>暂无输出</textarea>
              <label for="Script">输出</label>
            </code>

            <br>
            <div class="d-grid gap-2 d-md-flex justify-content-md-end">
              <div id="buttons">
                <button class="btn btn-outline-primary me-md-2" type="button" onclick="Upload() " )" + (HTML.User.GetUserType() == UT_GUEST ? "disabled" : "") + R"(>加载本地脚本</button>
                <button class="btn btn-success" type="button" onclick="RunScript() " )" + (HTML.User.GetUserType() == UT_GUEST ? "disabled" : "") + R"(>运行脚本</button>
              </div>
              <input type="file" id="local-script" onchange="selectLocalScript(event) " accept=".bat,.cmd,.txt" style="display: none;">
              <div class="progress" role="progressbar" id="scriptrunning" style="display:none;width:100%">
                <div class="progress-bar progress-bar-striped progress-bar-animated" style="width:100%">脚本正在运行...</div>
              </div>
            </div>

          </div>
        </div>

      </div>
      </div>

    </div>

    <script>
        document.getElementById('Authority').addEventListener('change', function() {
            var selectedValue = this.value;
            if (selectedValue == "1")  // User
            {
                document.getElementById("Interactive").disabled = false;
            }
            else if (selectedValue == "2")  // Service
            {
                document.getElementById("Interactive").selectedIndex = 0;
                document.getElementById("Interactive").disabled = true;
                document.getElementById("Output").value = "暂无输出";
                document.getElementById("Output").disabled = false;
            }
        });
        document.getElementById('Interactive').addEventListener('change', function() {
            var selectedValue = this.value;
            if (selectedValue == "0")  // Background
            {
                document.getElementById("Output").value = "暂无输出";
                document.getElementById("Output").disabled = false;
            }
            else if (selectedValue == "1")  // Foreground
            {
                document.getElementById("Output").value = "该模式不支持查看控制台输出结果";
                document.getElementById("Output").disabled = true;
            }
        });
        function Upload()
        {
            document.querySelector('#local-script').click()
        }
        function selectLocalScript(event)
        {
            const file = event.target.files[0];
            if (!file) {
                return;
            }
            const reader = new FileReader();
            reader.onload = function(e) {
                const content = e.target.result;

                var d = new TextDecoder("gb2312");
                var decoded = d.decode(content);

                document.getElementById('Script').value = decoded;
            };
            reader.readAsArrayBuffer(file);
        }
        function RunScript()
        {
            swal({
              title: "执行命令",
              text: "确实要在远程计算机上运行自定义脚本吗？",
              icon: "warning",
              buttons: true,
              dangerMode: true,
              buttons: ["取消", "确定"],
            })
            .then((willExec) => {
              if (willExec) {
                document.getElementById('buttons').style.display = "none";
                document.getElementById('scriptrunning').style.display = "block";

                var text = document.getElementById('Script').value;
                var fileName = "UserScript.bat";
 
                var formData = new FormData();
                var blob = new Blob([text], { type: 'text/plain' });
                formData.append('file', blob, fileName);

                document.getElementById('Output').value = "";

                $.ajax({
                    url: '/command/batchfilehandler.exe?' + document.getElementById('Authority').value + "+" + document.getElementById('Interactive').value,
                    type: 'POST',
                    cache: false,
                    data: formData,
                    contentType: false,
                    processData: false,
                    success: function (response) {
                      document.getElementById('Output').value = response;
                      document.getElementById('Output').scrollTo({
                          top: 0,
                          left: 0
                      });
                      document.getElementById('buttons').style.display = "block";
                      document.getElementById('scriptrunning').style.display = "none";
                    },
                    error: function (xhr, status, error) {
                      console.log(error);
                      swal({
                        title: "自定义脚本未执行",
                        text: "运行脚本时出现错误",
                        icon: "error",
                        buttons: false,
                        dangerMode: false,
                      });
                      document.getElementById('buttons').style.display = "block";
                      document.getElementById('scriptrunning').style.display = "none";
                    }
                });
              }
            });
        }

    </script>
)");
    HTML.Output();

    return 0;
}
