// HTMLFrame.cpp : Coral Remote Controller HTML输出类和函数的实现。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

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

    // 链接用户: 读取Token
    const cgicc::CgiEnvironment& env = cgi.getEnvironment();
    for (auto cci : env.getCookieList())
    {
        if (cci.getName() == "CORAL_REMOTE_CONTROLLER_MANAGE")
        {
            Token = cci.getValue();
            break;
        }
    }

    // 链接用户: 获取GUID
    std::string UserInfo;
    if (Token != "")
        UserInfo = RCAuth("RequestGUID2NamebyToken", Token);
    if (UserInfo != "" && UserInfo != "CORAL_RC_AUTH_FAILED")
    {
        std::string _uname, _guid;
        std::stringstream tempIO;
        tempIO << UserInfo;
        tempIO >> _guid >> _uname;
        User.Assign(_guid, _uname);
    }

    // 检查云盘PIN访问是否启用
    if (User.IsCloudStorageUser() && !GetSettings("CloudStoragePIN", false))
    {
        AJAXOutput((std::string)R"(<!-- AuthcheckFailed -->
<script src="/assets/js/HTMLFrame.js?version=)" + PROGRAM_BUILD_S + R"("></script>
<script> CRLogout(); </script>)");
        exit(0);
    }

    // 检查系统是否启用
    if (!GetSettings("SystemStatus", true))
    {
        bool DisableFlag = 0;

        if ((Link != "/login/" && Link != "/login/RSAPublicKey.exe" &&
            Link.substr(0, 12) != "/manage/main" &&
            Link.substr(0, 5) != "/OOBE"))
        {
            DisableFlag = 1;
        }
        else if (Link != "/login/" && Link != "/login/RSAPublicKey.exe" && User.GetUserType() > UT_ADMIN)
        {
            DisableFlag = 1;
        }

        if (DisableFlag)
        {
            PageTitle = BrowserTitle = "系统已禁用";
            if (!User.IsForegroundUser() && User.GetGUID() != "")
                FeatureList = R"(
        <ul>
          <li class="dropdown"><a href="javascript:void(0) " class="btn-learn-more"><span>以 )" + User.GetName() + R"( 身份登录</span> <i class="bi bi-chevron-down"></i></a>
            <ul>
              <li><a href="javascript:void(0) " onclick="CRLogout() ">退出登录</a></li>
            </ul>
          </li>
        </ul>
        <i class="bi bi-list mobile-nav-toggle"></i>
)";


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
                    if (User.IsOOBEed() < OOBEVERSION && !User.IsForegroundUser())
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

    // User-Agent & IP记录
    if (User.GetGUID() != "" && !User.IsForegroundUser() && !User.IsCloudStorageUser())
    {
        // 检查User-Agent是否变化
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

        // 检查IP地址是否变化
        if (GetSettings("IPAddressCheck", true))
        {
            std::string RegAgent;
            std::ifstream fin(User.Data("IPAddress"));
            getline(fin, RegAgent);
            fin.close();

            if (RegAgent != (std::string)getenv("REMOTE_ADDR"))
            {
                AJAXOutput((std::string)R"(<!-- AuthcheckFailed -->
<script src="/assets/js/HTMLFrame.js?version=)" + PROGRAM_BUILD_S + R"("></script>
<script> CRLogout(); </script>)");

                Log("用户尝试使用不匹配的IP地址登录，已被系统阻止", "HTMLFrame", LL_WARNING);

                exit(0);
            }
        }
        else
        {
            std::ofstream fout(User.Data("IPAddress"));
            fout << getenv("REMOTE_ADDR");
            fout.close();
        }
    }

    // 将未登录用户/错误登录ForegroundUser用户重定向至登录页面
    if ((_IsCheckUser && User.GetGUID() == "") 
        || (User.IsForegroundUser() && strcmp(getenv("REMOTE_ADDR"), "::1")))
    {
        AJAXOutput((std::string)R"(<!-- AuthcheckFailed -->
<script src="/assets/js/HTMLFrame.js?version=)" + PROGRAM_BUILD_S + R"("></script>
<script> CRLogout(); </script>)");
        exit(0);
    }

    // 将越界访问的云盘用户重定向到云盘页面
    if (_IsCheckUser && User.IsCloudStorageUser() && Link.substr(0, 9) != "/explorer")
    {
        AJAXOutput((std::string)R"(<!-- AuthcheckFailed -->
<script> window.location.href = "/explorer" </script>)");
        exit(0);
    }

    // 记录Website日志
    IsLog = GetSettings("Log", true);
    if (User.GetGUID() != "" && IsLog)
    {
        std::ofstream fout(DatabasePath() + "Log\\full.log", std::ios::app);
        fout << StandardTime(false) << " " << User.GetName() << " " << Link << std::endl;
        fout.close();
    }

    // 用户最近使用时间
    if (User.GetGUID() != "" && !User.IsForegroundUser() && !User.IsCloudStorageUser())
    {
        std::ofstream fout(User.Data("lastrequest"));
        fout << StandardTime(false);
        fout.close();
    }

    // 设置菜单栏
    if (User.GetGUID() != "")
        if (!User.IsCloudStorageUser())
        FeatureList = (std::string)R"(
        <ul>
          <li><a class="nav-link" href="/">主页</a></li>
          <li class="dropdown"><a href="javascript:void(0) "><span>工具</span> <i class="bi bi-chevron-down"></i></a>
            <ul>
              <li><a href="/power">基本控制</a></li>
              <li><a href="/taskmgr">进程管理</a></li>
              <li><a href="/windowmgr">窗口管理</a></li>
              <li><a href="/explorer">文件管理</a></li>
              <li><a href="/screenshot">实时屏幕</a></li>
              <li><a href="/clipboard">剪贴板</a></li>
)"
#ifndef V4_RELEASE
+ R"(
              <li><a href="/datapush">内容推送</a></li>
)"
#endif
+ R"(
              <li><a href="/command">执行命令</a></li>
              <li><a href="/message">发送消息</a></li>
              <li><a href="/network">网络设置</a></li>
              <li><a href="/sysinfo">系统信息</a></li>
            </ul>
          </li>
          <li class="dropdown"><a href="javascript:void(0) " class="btn-learn-more"><span>以 )" + (!User.IsForegroundUser() ? User.GetName() : "前台用户") + R"( 身份登录</span> <i class="bi bi-chevron-down"></i></a>
            <ul>
)" + (!User.IsForegroundUser() ? R"(              <li><a href="/manage/account">账户设置</a></li>
)" : "") + (User.GetUserType() <= UT_ADMIN ? R"(              <li><a href="/manage/main">后台管理</a></li>
)" : "") + (!User.IsForegroundUser() ? R"(              <li><a href="javascript:void(0) " onclick="CRLogout() ">退出登录</a></li>
)" : "") + R"(
            </ul>
          </li>
        </ul>
        <i class="bi bi-list mobile-nav-toggle"></i>
)";
        else
        {
            FeatureList = R"(
        <ul>
          <li><a href="javascript:void(0) " onclick="CRLogout() ">退出共享云盘</a></li>
        </ul>
        <i class="bi bi-list mobile-nav-toggle"></i>
)";
            PageTitle = BrowserTitle = "共享云盘";
            BreadCrumb = "";
        }
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
              <li><a href="/explorer">文件管理</a></li>
              <li><a href="/screenshot">实时屏幕</a></li>
              <li><a href="/clipboard">剪贴板</a></li>
)"
#ifndef V4_RELEASE
+ R"(
              <li><a href="/datapush">内容推送</a></li>
)"
#endif
+ R"(
              <li><a href="/command">执行命令</a></li>
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
          <li class="dropdown"><a href="javascript:void(0) " class="btn-learn-more"><span>以 )" + (!User.IsForegroundUser() ? User.GetName() : "前台用户") + R"( 身份登录</span> <i class="bi bi-chevron-down"></i></a>
            <ul>
)" + (!User.IsForegroundUser() ? R"(              <li><a href="/manage/account">账户设置</a></li>
)" : "") + (User.GetUserType() <= UT_ADMIN ? R"(              <li><a href="/manage/main">后台管理</a></li>
)" : "") + (!User.IsForegroundUser() ? R"(              <li><a href="javascript:void(0) " onclick="CRLogout() ">退出登录</a></li>
)" : "") + R"(
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
        if (Level == LL_DEBUG && !GetSettings("DebugLogs", false))
            return;

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

HTMLFrame::HTMLToolCards::HTMLToolCards()
{
    size = 0;
}
void HTMLFrame::HTMLToolCards::Add(std::string _Title, std::string _Description, std::string _Link, std::string _Icon)
{
    Title[++size] = _Title;
    Description[size] = _Description;
    Link[size] = _Link;
    Icon[size] = _Icon;
}

std::string HTMLFrame::HTMLToolCards::Output()
{
    std::string Ans;

    int i = 1;
    for (int k = 1; k <= ceil(size / 3.0); k++)
    {
        Ans += R"(
<div class="row g-4">
)";
        
        for (int j = 1; j <= 3; j++)
        {
            if (i > size)
                break;

            Ans += R"(
  <div class="col-sm-4 mb-3 mb-sm-0">
    <div class="card h-100">
      <div class="card-body">
        <div class="row">
          <div class="col-lg-2">
            <i class=")" + Icon[i] + R"( text-primary" style="font-size:45px"></i>
          </div>
          <div class="col-lg-10">
            <h5 class="card-title"><b>)" + Title[i] + R"(</b></h5>
            <p class="card-text">)" + Description[i] + R"(</p>
          </div>
        </div>
        <a href=")" + Link[i] + R"(" class="btn btn-outline-primary btn-sm" style="margin-top:10px">打开</a>
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
