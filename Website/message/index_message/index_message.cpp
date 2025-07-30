// index_message.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/message/"

#include <iostream>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\");

    HTMLFrame HTML;
    HTML.Register(PT_INDEX, ADDRESS, "发送消息", "发送消息", R"(<li><a href="/">主页</a></li><li>发送消息</li>)", true);
    
    HTML.Custom((std::string)R"(
    <div class="portfolio-info">

        <ul class="nav nav-underline">
          <li class="nav-item">
            <a class="nav-link active" data-bs-toggle="tab" data-bs-target="#single-pane" role="tab">单向通知</a>
          </li>
          <li class="nav-item">
            <a class="nav-link" data-bs-toggle="tab" data-bs-target="#double-pane" role="tab">双向消息</a>
          </li>
        </ul>

        <br>
        <div class="tab-content">

          <div class="tab-pane fade show active" id="single-pane" role="tabpanel" tabindex="0">
          <div class="row">

          <div class="col-lg-6">
            <div class="form-floating mb-3">
              <input type="text" class="form-control" id="SingleTitle" )" + (HTML.User.GetUserType() == UT_GUEST ? "disabled" : "") + R"(>
              <label for="SingleTitle">标题</label>
            </div>
            <div class="form-floating mb-3">
              <select class="form-select" id="SingleMessageType" )" + (HTML.User.GetUserType() == UT_GUEST ? "disabled" : "") + R"(>
                <option value="1" selected>信息</option>
                <option value="2">警告</option>
                <option value="3">错误</option>
              </select>
              <label for="SingleMessageType">消息类型</label>
            </div>
            <div class="form-floating mb-3">
              <select class="form-select" id="SingleMessageMethod" )" + (HTML.User.GetUserType() == UT_GUEST ? "disabled" : "") + R"(>
                <option value="1" selected>系统通知</option>
                <option value="2">前台弹窗</option>
              </select>
              <label for="SingleMessageMethod">展示方式</label>
            </div>
          </div>

          <div class="col-lg-6">
            <div class="form-floating mb-3">
              <textarea class="form-control" id="SingleContent" style="height:205px" )" + (HTML.User.GetUserType() == UT_GUEST ? "disabled" : "") + R"(></textarea>
              <label for="SingleContent">内容</label>
            </div>
            <div class="d-grid gap-2 d-md-flex justify-content-md-end">
              <button class="btn btn-outline-primary me-md-2" type="button" onclick="Oneway_Clear() " )" + (HTML.User.GetUserType() == UT_GUEST ? "disabled" : "") + R"(>清空</button>
              <button class="btn btn-success" type="button" onclick="Oneway_Send() " )" + (HTML.User.GetUserType() == UT_GUEST ? "disabled" : "") + R"(>发送消息</button>
            </div>
          </div>
          </div>

          </div>

          <div class="tab-pane fade" id="double-pane" role="tabpanel" tabindex="1">
            In Development. Coming Soon.
          </div>

        </div>
    </div>

    <script>
        function Oneway_Clear()
        {
            document.getElementById('SingleTitle').value = "";
            document.getElementById('SingleContent').value = "";
        }
        function Oneway_Send()
        {
            swal({
              title: "发送消息",
              text: "确实要向远程计算机发送自定义消息吗？",
              icon: "warning",
              buttons: true,
              dangerMode: true,
              buttons: ["取消", "确定"],
            })
            .then((willExec) => {
              if (willExec) {
                var ans = GetTextByAJAX("/message/onewaymessage.exe?" + document.getElementById('SingleMessageType').value 
                                                         + "+" + document.getElementById('SingleMessageMethod').value 
                                                         + "+" + HexEncode(encodeURI(document.getElementById('SingleTitle').value))
                                                         + "+" + HexEncode(encodeURI(document.getElementById('SingleContent').value)));
                if (ans != undefined)
                    if (ans.substr(0, 3) == "Yes")
                    {
                      swal({
                        title: "成功发送了消息",
                        text: "成功向远程计算机发送了自定义消息。",
                        icon: "success",
                        buttons: false,
                        dangerMode: false,
                      });
                      Oneway_Clear();
                      return;
                    }
                swal({
                  title: "发送消息失败",
                  text: "出现意外错误，自定义消息未被发送。",
                  icon: "error",
                  buttons: false,
                  dangerMode: true,
                });
              }
            });
        }

    </script>
)");
    HTML.Output();

    return 0;
}
