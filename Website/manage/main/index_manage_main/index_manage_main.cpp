// index_manage_main.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/manage/main/"

#include <iostream>
#include "../../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\..\\");

    HTMLFrame HTML;
    HTML.Register(PT_INDEX, ADDRESS, "后台管理", "后台管理", R"(<li><a href="/">主页</a></li><li>后台管理</li>)", true);
    HTML.ManageInit();

    int TodayUserCnt = 0;
    std::string Today, tDate;
    std::stringstream tempIO;
    tempIO << StandardTime(false);
    tempIO >> Today;

    std::ifstream fin, fin2;
    int cnt = 0;
    std::string _Name, _GUID;
    fin.open(DatabasePath() + "User\\list");
    fin >> cnt;
    for (int i = 1; i <= cnt; ++i)
    {
        fin >> _Name >> _GUID;

        fin2.open(UserFile(_GUID, "data", "lastrequest"));
        if (fin2.fail())
            tDate = "";
        else
            fin2 >> tDate;
        fin2.close();

        if (tDate == Today)
            TodayUserCnt++;
    }
    fin.close();

    std::string Code = (std::string)R"(
  <div class="p-5 mb-4 bg-body-tertiary rounded-3">
    <div class="container-fluid py-7">
      <div class="row" style="display:flex; align-items:center; justify-content:center;">

        <div class="col-lg-6">
          <h1 class="display-6 fw-bold">概述</h1>
          <p class="fs-6">今日活跃用户数：)" + Num2String(TodayUserCnt) + R"(</p>
        </div>
        <div class="col-lg-6">
<!--
          <canvas id="ActiveUsers"></canvas>
-->
        </div>
      </div>
    </div>
  </div>

<script>
/*
  const UserCrt = document.getElementById('ActiveUsers');
  new Chart(UserCrt, {
    data: {
      datasets: [{
        type: 'bar',
        label: 'HTML请求数',
        yAxisID: 'y-axis-2',
        data: [500, 500, 500, 500, 700, 600, 100],
        order: 3
      }, {
        type: 'line',
        label: '活跃用户数',
        yAxisID: 'y-axis-1',
        data: [3, 1, 0, 4, 2, 1, 2],
        order: 1
      }, {
        type: 'line',
        label: '访问IP地址数',
        yAxisID: 'y-axis-1',
        data: [2, 1, 0, 4, 1, 1, 2],
        order: 2
      }],
      labels: ['12/26', '12/27', '12/28', '12/29', '12/30', '12/31', '01/01']
    },

    options: {
      scales: {
        'y-axis-1': {
          type: 'linear',
          position: 'left',
          beginAtZero: true
        },
        'y-axis-2': {
          type: 'linear',
          position: 'right',
          beginAtZero: true
        }
      }
    }
  });
*/
</script>
)";

    HTMLFrame::HTMLToolCards ToolCards;
    ToolCards.Add("系统设置", "对Coral Remote Controller进行全方位的详细配置，满足您的要求。", "/manage/main/settings", "bi bi-gear-wide-connected");
    ToolCards.Add("用户管理", "对有权限访问Coral Remote Controller的用户进行控制和管理。", "/manage/main/user", "bi bi-people-fill");
    ToolCards.Add("存储管理", "管理Coral Remote Controller用户数据和临时文件占用的磁盘空间。", "/manage/main/storage", "bi bi-database-check");
    ToolCards.Add("日志查看", "查看Coral Remote Controller各组件生成的日志，以进行错误排查和事故调查。", "/manage/main/log", "bi bi-journal-text");
    if (HTML.User.GetUserType() == UT_ROOT)
        ToolCards.Add("程序更新", "上传Coral Remote Controller的更新包，对程序进行远程更新。", "/manage/main/update", "bi bi-arrow-up-circle");
    Code += ToolCards.Output();

    HTML.Custom(Code);

    HTML.Output();
    return 0;
}
