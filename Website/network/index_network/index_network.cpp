// index_network.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/network/"

#include <iostream>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\");

    HTMLFrame HTML;
    HTML.Register(PT_INDEX, ADDRESS, "网络设置", "网络设置", R"(<li><a href="/">主页</a></li><li>网络设置</li>)", true);

    HTML.Custom((std::string)R"(
    <div class="portfolio-info">

        <ul class="nav nav-underline">
          <li class="nav-item">
            <a class="nav-link active" data-bs-toggle="tab" data-bs-target="#rule-pane" role="tab">域名规则</a>
          </li>
          <li class="nav-item">
            <a class="nav-link" data-bs-toggle="tab" data-bs-target="#todo-pane" role="tab">IN DEV</a>
          </li>
        </ul>

        <br>
        <div class="tab-content">

          <div class="tab-pane fade show active" id="rule-pane" role="tabpanel" tabindex="0">
            <div class="d-grid gap-2 d-md-flex justify-content-md-end">
              <button class="btn btn-success btn-sm" type="button" data-bs-toggle="modal" data-bs-target="#modalNewRule" )" + (HTML.User.GetUserType() == UT_GUEST ? "disabled" : "") + R"(><i class="bi bi-plus-lg"></i>&nbsp;新建规则</button>
            </div>
            <div id="rulelist" style="height:300px; overflow:auto;"></div>
          </div>

          <div class="tab-pane fade" id="todo-pane" role="tabpanel" tabindex="1">
            In Development. Coming Soon.
          </div>

        </div>
    </div>

    <script>
        AsyncChangeTextByAJAX("rulelist", "/network/listrule.exe");
    </script>

<div class="modal fade" tabindex="-1" data-bs-backdrop="static" role="dialog" id="modalNewRule">
  <div class="modal-dialog modal-dialog-centered" role="document">
    <div class="modal-content rounded-4 shadow">
      <div class="modal-header p-5 pb-4 border-bottom-0">
        <h1 class="mb-0 fs-4">新建规则</h1>
        <button type="button" class="btn-close" data-bs-dismiss="modal" aria-label="Close"></button>
      </div>

      <div class="modal-body p-5 pt-0">
        <form class="NewRule">

          <div class="form-floating mb-3">
            <input class="form-control rounded-3" id="Domain">
            <label for="Domain">域名</label>
          </div>
          <div class="form-floating mb-3">
            <input class="form-control rounded-3" id="IP">
            <label for="IP">IP地址</label>
          </div>

          <button class="w-100 mb-2 btn btn-lg rounded-3 btn-primary" type="submit" data-bs-dismiss="modal">确认</button>

        </form>
      </div>
    </div>
  </div>
</div>

<script src="/assets/js/DomainParser.js"></script>
<script>
    $('.NewRule').on('submit',function(e){
        e.preventDefault();

        var IP = document.getElementById("IP").value;
        var Domain = document.getElementById("Domain").value;

		var parser = new DomainParser(Domain);
		parser.parse();
		if (parser.hasError()) {
            swal({
              title: "域名无效",
              text: parser.getMessage(),
              icon: "error",
              buttons: false,
              dangerMode: true,
            })
            .then(() => {
                $('#modalNewRule').modal('show');
            });
			return false;
		}
		
		var str = IP;
		str = str.match(/(\d+)\.(\d+)\.(\d+)\.(\d+)/g);
		if (str == null){
			swal({
              title: "IP地址无效",
              text: "请输入有效的IP地址！",
              icon: "error",
              buttons: false,
              dangerMode: true,
            })
            .then(() => {
                $('#modalNewRule').modal('show');
            });
			return false;
		}
		else if (RegExp.$1>255 || RegExp.$2>255 || RegExp.$3>255 || RegExp.$4>255){
			swal({
              title: "IP地址无效",
              text: "请输入有效的IP地址！",
              icon: "error",
              buttons: false,
              dangerMode: true,
            })
            .then(() => {
                $('#modalNewRule').modal('show');
            });
			return false;
		}
			
		var ans = GetTextByAJAX("/network/addrule.exe?" + Domain + "+" + IP);
        AsyncChangeTextByAJAX("rulelist", "/network/listrule.exe");

        if (ans != undefined)
            if (ans.substr(0, 3) == "Yes")
            {
              swal({
                title: "成功添加了规则",
                text: "成功添加了新的自定义域名规则",
                icon: "success",
                buttons: false,
                dangerMode: false,
              });
              document.getElementById("Domain").value = "";
              document.getElementById("IP").value = "";
              return;
            }
        swal({
          title: "添加规则失败",
          text: "添加规则失败，请检查是否已经存在冲突的策略。",
          icon: "error",
          buttons: false,
          dangerMode: true,
        });
    });

    function Delete(Domain)
    {
        swal({
          title: "删除策略",
          text: "确实要删除关于域名 " + Domain + " 的策略吗？",
          icon: "warning",
          buttons: true,
          dangerMode: true,
          buttons: ["取消", "确定"],
        })
        .then((IsDelete) => {
          if (IsDelete) {
		    GetTextByAJAX("/network/deleterule.exe?" + Domain);
            AsyncChangeTextByAJAX("rulelist", "/network/listrule.exe");
          }
        });
    }
</script>

)");
    HTML.Output();

    return 0;
}
