// HTMLFrame.cpp : Coral Remote Controller HTML输出类和函数的实现。

// 版权声明：
// Copyright (C) 2020-2024 Coral Studio. All Rights Reserved.

#include "pch.h"
#include "framework.h"
#include "Encoding.h"
#include "HTMLFrame.h"

HTMLFrame::HTMLFrame()
{
	FeatureList = "";
    BreadCrumb = "";

    PageType = PT_EMPTY;
    Link = BrowserTitle = PageTitle = CustomCode = "";
    IsCheckUser = IsLog = 1;
}

void HTMLFrame::Register(int _PageType,
    std::string _Link, std::string _PageTitle, std::string _BrowserTitle, std::string _BreadCrumb,
    int _IsCheckUser)
{
    // 复制参数
    PageType = _PageType;
    Link = _Link;
    PageTitle = _PageTitle;
    BrowserTitle = _BrowserTitle;
    BreadCrumb = _BreadCrumb;
    IsCheckUser = _IsCheckUser;

    // 链接用户
    const cgicc::CgiEnvironment& env = cgi.getEnvironment();
    for (auto cci : env.getCookieList())
    {
        if (cci.getName() == "CORAL_REMOTE_CONTROLLER_MANAGE")
        {
            Token = cci.getValue();
            break;
        }
    }
    User.Assign(Token);

    // 检查系统是否启用
    if (!GetSettings("SystemStatus", true))
    {
        bool DisableFlag = 0;

        if ((Link != "/login/" &&
            Link.substr(0, 12) != "/manage/main" &&
            Link.substr(0, 5) != "/OOBE"))
        {
            DisableFlag = 1;
        }
        else if (Link != "/login/" && User.GetUserType() > UT_ADMIN)
        {
            DisableFlag = 1;
        }

        if (DisableFlag)
        {
            PageTitle = BrowserTitle = "系统已禁用";

            if (User.GetUserType() > UT_ADMIN)
            Custom(R"(
    <div class="p-5 mb-4 bg-body-tertiary rounded-3">
      <div class="container-fluid py-7">
        <h1 class="display-7 fw-bold">Remote Controller 当前不可用</h1>
        <p class="fs-6">请联系管理员以获取更多信息。</p>
      </div>
    </div>
)");
            else
            {
                // 检查管理员用户的OOBE状态
                if (Link == "/")
                    if (User.IsOOBEed() < OOBEVERSION)
                    {
                        AJAXOutput(R"(<script> window.location.href = "/OOBE/user"; </script>)");
                        ::exit(0);
                    }

                Custom(R"(
    <div class="p-5 mb-4 bg-body-tertiary rounded-3">
      <div class="container-fluid py-7">
        <h1 class="display-7 fw-bold">Remote Controller 当前不可用</h1>
        <p class="fs-6">作为管理员，您仍可以访问后台管理。系统的其他模块已被禁用。</p>
        <a class="btn btn-success" href="/manage/main">转到后台管理</a>
      </div>
    </div>
)");
            }
            Output();
            ::exit(0);
        }
    }

    // 检查User-Agent是否变化
    if (User.GetToken() != "")
    {
        if (GetSettings("UserAgentCheck", true))
        {
            std::string RegAgent;
            std::ifstream fin(User.Data("UserAgent"));
            getline(fin, RegAgent);
            fin.close();

            if (RegAgent != (std::string)getenv("HTTP_USER_AGENT"))
            {
                AJAXOutput((std::string)R"(<!-- AuthcheckFailed -->
<script src="/assets/js/HTMLFrame.js?version=)" + PROGRAM_BUILD_S + R"("></script>
<script> CRLogout(); </script>)");

                Log("用户尝试使用不匹配的UA登录，已被系统阻止", "HTMLFrame", LL_WARNING);

                exit(0);
            }
        }
        else
        {
            std::ofstream fout(User.Data("UserAgent"));
            fout << getenv("HTTP_USER_AGENT");
            fout.close();
        }
    }

    // 将未登录用户重定向至登录页面
    if (_IsCheckUser && User.GetToken() == "")
    {
        AJAXOutput((std::string)R"(<!-- AuthcheckFailed -->
<script src="/assets/js/HTMLFrame.js?version=)" + PROGRAM_BUILD_S + R"("></script>
<script> CRLogout(); </script>)");
        exit(0);
    }

    // 记录Website日志
    IsLog = GetSettings("Log", true);
    if (User.GetToken() != "" && IsLog)
    {
        std::ofstream fout(DatabasePath() + "Log\\full.log", std::ios::app);
        fout << StandardTime(false) << " " << User.GetName() << " " << Link << std::endl;
        fout.close();
    }

    // 用户最近使用时间
    if (User.GetToken() != "")
    {
        std::ofstream fout(User.Data("lastrequest"));
        fout << StandardTime(false);
        fout.close();
    }

    // 设置菜单栏
    if (User.GetToken() != "")
        FeatureList = R"(
        <ul>
          <li><a class="nav-link" href="/">主页</a></li>
          <li class="dropdown"><a href="javascript:void(0) "><span>工具</span> <i class="bi bi-chevron-down"></i></a>
            <ul>
              <li><a href="/power">基本控制</a></li>
              <li><a href="/taskmgr">进程管理</a></li>
              <li><a href="/windowmgr">窗口管理</a></li>
              <li><a href="/explorer">文件浏览</a></li>
              <li><a href="/screenshoot">屏幕截图</a></li>
              <li><a href="/clipboard">剪贴板</a></li>
              <li><a href="/message">发送消息</a></li>
              <li><a href="/network">网络设置</a></li>
              <li><a href="/sysinfo">系统信息</a></li>
            </ul>
          </li>
          <li class="dropdown"><a href="javascript:void(0) " class="btn-learn-more"><span>以 )" + User.GetName() + R"( 身份登录</span> <i class="bi bi-chevron-down"></i></a>
            <ul>
              <li><a href="/manage/account">账户设置</a></li>
)" 
+ (User.GetUserType() <= UT_ADMIN ? R"(              <li><a href="/manage/main">后台管理</a></li>)" : "")
+ R"(
              <li><a href="javascript:void(0) " onclick="CRLogout() ">退出登录</a></li>
            </ul>
          </li>
        </ul>
        <i class="bi bi-list mobile-nav-toggle"></i>
)";

}

void HTMLFrame::ManageInit()
{
    if (User.GetUserType() > UT_ADMIN)
    {
        AJAXOutput(R"(<!-- AuthcheckFailed -->
<script> window.location.href = "/"; </script>)");
        exit(0);
    }

    FeatureList = (std::string)R"(
        <ul>
          <li><a class="nav-link" href="/">主页</a></li>
          <li class="dropdown"><a href="javascript:void(0) "><span>工具</span> <i class="bi bi-chevron-down"></i></a>
            <ul>
              <li><a href="/power">基本控制</a></li>
              <li><a href="/taskmgr">进程管理</a></li>
              <li><a href="/windowmgr">窗口管理</a></li>
              <li><a href="/explorer">文件浏览</a></li>
              <li><a href="/screenshoot">屏幕截图</a></li>
              <li><a href="/clipboard">剪贴板</a></li>
              <li><a href="/message">发送消息</a></li>
              <li><a href="/network">网络设置</a></li>
              <li><a href="/sysinfo">系统信息</a></li>
            </ul>
          </li>
          <li class="dropdown"><a href="javascript:void(0) "><span>后台管理</span> <i class="bi bi-chevron-down"></i></a>
            <ul>
              <li><a href="/manage/main/settings">系统设置</a></li>
              <li><a href="/manage/main/user">用户管理</a></li>
              <li><a href="/manage/main/storage">存储管理</a></li>
              <li><a href="/manage/main/log">日志查看</a></li>
)"
+ (User.GetUserType() == UT_ROOT ? R"(              <li><a href="/manage/main/update">程序更新</a></li>)" : "")
+ R"(
            </ul>
          </li>
          <li class="dropdown"><a href="javascript:void(0) " class="btn-learn-more"><span>以 )" + User.GetName() + R"( 身份登录</span> <i class="bi bi-chevron-down"></i></a>
            <ul>
              <li><a href="/manage/account">账户设置</a></li>
              <li><a href="/manage/main">后台管理</a></li>
              <li><a href="javascript:void(0) " onclick="CRLogout() ">退出登录</a></li>
            </ul>
          </li>
        </ul>
        <i class="bi bi-list mobile-nav-toggle"></i>
)";

}

void HTMLFrame::Custom(std::string _Code)
{
    CustomCode = _Code;
}

void HTMLFrame::CustomFeatureList(std::string _Code)
{
    FeatureList = _Code;
}

void HTMLFrame::Log(std::string Event, std::string Module, int Level)
{
    if (IsLog)
    {
        std::ofstream fout(DatabasePath() + "Log\\main.log", std::ios::app);
        std::string Name = User.GetName();
        if (Name == "") Name = "SYSTEM";

        fout << StandardTime(false) << ' ' << Name << ' ' << Module << ' ' << Level << std::endl;
        fout << Event << std::endl;

        fout.close();
    }
}

void HTMLFrame::Output()
{
	puts("Content-type: text/html\n");

    std::string UTF8Code, OriginCode = OutputFrameCode(PageType, CustomCode).c_str();
    GB2312ToUTF_8(UTF8Code, (char*)OriginCode.c_str(), OriginCode.size());
    puts(UTF8Code.c_str());
}

HTMLToolCards::HTMLToolCards()
{
    size = 0;
}
void HTMLToolCards::Add(std::string _Title, std::string _Description, std::string _Link)
{
    Title[++size] = _Title;
    Description[size] = _Description;
    Link[size] = _Link;
}

std::string HTMLToolCards::Output()
{
    std::string Ans;

    int i = 1;
    for (int k = 1; k <= ceil(size / 3.0); k++)
    {
        Ans += R"(
<div class="row">
)";
        
        for (int j = 1; j <= 3; j++)
        {
            if (i > size)
                break;

            Ans += R"(
  <div class="col-sm-4 mb-3 mb-sm-0">
    <div class="card">
      <div class="card-body">
        <h5 class="card-title"><b>)" + Title[i] + R"(</b></h5>
        <p class="card-text">)" + Description[i] + R"(</p>
        <a href=")" + Link[i] + R"(" class="btn btn-outline-primary btn-sm">打开</a>
      </div>
    </div>
  </div>
)";

            i++;
        }

        Ans += R"(
</div>
<br>
)";
    }
    return Ans;
}
