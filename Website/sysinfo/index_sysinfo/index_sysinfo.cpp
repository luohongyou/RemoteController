// index_sysinfo.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/sysinfo/"

#include <iostream>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

using namespace std;
int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\");

    HTMLFrame HTML;
    HTML.Register(PT_INDEX, ADDRESS, "系统信息", "系统信息", R"(<li><a href="/">主页</a></li><li>系统信息</li>)", true);

    std::string Code, RCCode;

    RCCode = R"(
    <center>
)";
    RCCode += "<h4> 版本信息 </h4>\n";
    ifstream fin;
    RCCode += "<div> Coral Remote Controller 版本：" + (string)VERSION + " </div>\n";
    RCCode += "<div> 编译信息：" + GetCompileInfo() + " </div>\n";
    RCCode += "<div> Coral Remote Agent 版本：" + RCAgent("getversion", HTML.User.GetName(), "") + "</div>\n";
    RCCode += "<br>";

    RCCode += "<h4> 关于 </h4>\n";
    RCCode += "<div> Coral Remote Controller 是基于 Visual C++ 和 Apache 开发的 Windows 远程管理实用程序。 </div>\n";
    RCCode += "<div> 开发：Luo Hongyou, Coral Studio </div>\n";
    RCCode += R"(
    </center>
)";

    Code = R"(
        <div class="row gy-4">

          <div class="col-lg-6">
            <div class="portfolio-info">
              <h3>硬件信息</h3>
              <div id="hardwareinfo" style="overflow:auto; height:600px">
              </div>
            </div>
          </div>

          <div class="col-lg-6">
            <div class="portfolio-info">
              <h3>Remote Controller 信息</h3>
              <div id="rcinfo" style="overflow:auto; height:260px">)" + RCCode + R"(</div>
            </div>
            <br>
            <div class="portfolio-info">
              <h3>软件信息</h3>
              <div id="softwareinfo" style="overflow:auto; height:190px"></div>
            </div>
          </div>

        </div>

        <script>
          setInterval(function(){
			  AsyncChangeTextByAJAX("hardwareinfo",  "/sysinfo/hardwareinfo.exe");
		  }, 1000);

		  AsyncChangeTextByAJAX("hardwareinfo",  "/sysinfo/hardwareinfo.exe");
		  AsyncChangeTextByAJAX("softwareinfo",  "/sysinfo/softwareinfo.exe");

        </script>
)";

    HTML.Custom(Code);

    HTML.Output();
    return 0;
}
