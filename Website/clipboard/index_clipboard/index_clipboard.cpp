// index_clipboard.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/clipboard/"

#include <iostream>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\");

    HTMLFrame HTML;
    HTML.Register(PT_INDEX, ADDRESS, "剪贴板", "剪贴板", R"(<li><a href="/">主页</a></li><li>剪贴板</li>)", true);

    HTML.Custom((std::string)R"(
    <div class="portfolio-info">

        <ul class="nav nav-underline">
          <li class="nav-item">
            <a class="nav-link active" data-bs-toggle="tab" data-bs-target="#now-pane" role="tab">实时剪贴板</a>
          </li>
          <li class="nav-item">
            <a class="nav-link" data-bs-toggle="tab" data-bs-target="#history-pane" role="tab">历史剪贴板</a>
          </li>
        </ul>

        <br>
        <div class="tab-content">

          <div class="tab-pane fade show active" id="now-pane" role="tabpanel" tabindex="0">
            <div class="d-grid gap-2 d-md-flex justify-content-md-end">
              <div class="form-check">
                <input class="form-check-input" type="checkbox" id="IsEditable" checked )" + (HTML.User.GetUserType() == UT_GUEST ? "disabled" : "") + R"(>
                <label class="form-check-label" for="IsEditable">只读</label>
              </div>
              <button class="btn btn-primary btn-sm" type="button" id="modifybtn" onclick="Modify() " disabled>修改</button>
              <button class="btn btn-success btn-sm" type="button" onclick="RefreshAll() ">刷新</button>
            </div>
            <div style="margin-top:10px;">
              <textarea id="clipboardnow" class="form-control" style="height:300px;" readonly></textarea>
            </div>
          </div>

          <div class="tab-pane fade" id="history-pane" role="tabpanel" tabindex="1">
            <div class="d-grid gap-2 d-md-flex justify-content-md-end">
              <button class="btn btn-success btn-sm" type="button" onclick="RefreshAll() ">刷新</button>)" + (std::string)(HTML.User.GetUserType() >= UT_USER ? "" : R"(
              <button class="btn btn-danger btn-sm" type="button" onclick="ClearHistory() ">清空</button>)") + R"(
            </div>
            <div id="clipboardall" style="height:400px; overflow:auto;"></div>
          </div>

        </div>
    </div>

    <script>
        document.getElementById('IsEditable').addEventListener('change', function() {
            if(this.checked) {
                document.getElementById('modifybtn').disabled = true;
                document.getElementById('clipboardnow').readOnly = true;
            } else {
                document.getElementById('modifybtn').disabled = false;
                document.getElementById('clipboardnow').readOnly = false;
            }
        });
        function RefreshNow()
        {
            document.getElementById('clipboardnow').value = GetTextByAJAX("/clipboard/getclipboardnow.exe");
        }
        function RefreshHistory()
        {
            AsyncChangeTextByAJAX("clipboardall", "/clipboard/getclipboardhistory.exe");
        }
        function RefreshAll()
        {
            RefreshNow();
            RefreshHistory();
            document.getElementById('modifybtn').disabled = true;
            document.getElementById('clipboardnow').readOnly = true;
            document.getElementById('IsEditable').checked = true;
        }

        function ClearHistory()
        {
            swal({
              title: "清空历史剪贴板",
              text: "确实删除所有保存的历史剪贴板数据吗？",
              icon: "warning",
              buttons: true,
              dangerMode: true,
              buttons: ["取消", "确定"],
            })
            .then((willExec) => {
              if (willExec) {
                GetTextByAJAX("/clipboard/clearclipboardhistory.exe");
                RefreshHistory();
              }
            });
        }
        function Modify()
        {
            GetTextByAJAX("/clipboard/modifyclipboard.exe?" + HexEncode(encodeURI(document.getElementById('clipboardnow').value)));
            RefreshNow();
            document.getElementById('modifybtn').disabled = true;
            document.getElementById('clipboardnow').readOnly = true;
            document.getElementById('IsEditable').checked = true;
            RefreshHistory();
        }

        RefreshNow();
        RefreshHistory();
    </script>
)");
    HTML.Output();

    return 0;
}
