// index_datapush.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/datapush/"

#include <iostream>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\");

    HTMLFrame HTML;
    HTML.Register(PT_INDEX, ADDRESS, "内容推送", "内容推送", R"(<li><a href="/">主页</a></li><li>内容推送</li>)", true);

    HTML.Custom((std::string)R"(
    <div class="portfolio-info">

      <ul class="nav nav-underline">
        <li class="nav-item">
          <a class="nav-link active" data-bs-toggle="tab" data-bs-target="#media-pane" role="tab">影音推送</a>
        </li>
        <li class="nav-item">
          <a class="nav-link" data-bs-toggle="tab" data-bs-target="#richtext-pane" role="tab">图文推送</a>
        </li>
      </ul>
      <br>

      <div class="tab-content">
        <div class="tab-pane fade show active" id="media-pane" role="tabpanel" tabindex="0">
          In Development. Coming Soon.
        </div>
        <div class="tab-pane fade" id="richtext-pane" role="tabpanel" tabindex="1">
          In Development. Coming Soon.
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
