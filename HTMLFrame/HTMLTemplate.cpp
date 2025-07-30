// HTMLTemplate.cpp : Coral Remote Controller HTML代码模板的实现。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#include "pch.h"
#include "HTMLFrame.h"
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

std::string Frame_GetHostName();

std::string HTMLFrame::OutputFrameCode(int _PageType, std::string _CustomCode)
{
	if (_PageType == PT_EMPTY)
		return R"(
<!DOCTYPE html>
<html lang="zh">

<head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
  <meta content="width=device-width, initial-scale=1.0" name="viewport">
  <meta name="color-scheme" content="light dark">
</head>

<body>
)" + _CustomCode + R"(
</body>

</html>)";

	else if (_PageType == PT_FRAME)
		return R"(
<!DOCTYPE html>
<html lang="zh" data-bs-theme="auto">

<head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
  <meta content="width=device-width, initial-scale=1.0" name="viewport">

  <title>)" + BrowserTitle + R"( - )" + Frame_GetHostName() + R"( - Coral Remote Controller</title>

  <!-- Google Fonts -->
  <link href="/assets/fonts/fonts.css" rel="stylesheet">

  <!-- Vendor CSS Files -->
  <link href="/assets/vendor/aos/aos.css" rel="stylesheet">
  <link href="/assets/vendor/bootstrap/css/bootstrap.min.css" rel="stylesheet">
  <link href="/assets/vendor/bootstrap-icons/bootstrap-icons.css" rel="stylesheet">
  <link href="/assets/vendor/glightbox/css/glightbox.min.css" rel="stylesheet">
  <link href="/assets/vendor/swiper/swiper-bundle.min.css" rel="stylesheet">
  <link href="/assets/vendor/notyf/notyf.min.css" rel="stylesheet">

  <!-- Template Main CSS File -->
  <link href="/assets/css/style.css?version=)" + PROGRAM_BUILD_S + R"(" rel="stylesheet" media="(prefers-color-scheme: light) " >
  <link href="/assets/css/style_dark.css?version=)" + PROGRAM_BUILD_S + R"(" rel="stylesheet" media="(prefers-color-scheme: dark) " >

  <!-- Addons JS File -->
  <script src="/assets/js/jquery-3.6.3.min.js"></script>
  <script src="/assets/js/sweetalert.min.js?240614"></script>
  <script src="/assets/js/bootstrap-auto-dark-mode.js"></script>
  <script src="/assets/js/chart.umd.js"></script>
  <script src="/assets/js/browser.min.js"></script>
  <script src="/assets/js/HTMLFrame.js?version=)" + PROGRAM_BUILD_S + R"("></script>
  <script src="/assets/vendor/notyf/notyf.min.js"></script>

</head>

<body>
)" + _CustomCode + R"(

  <!-- Vendor JS Files -->
  <script src="/assets/vendor/aos/aos.js"></script>
  <script src="/assets/vendor/bootstrap/js/bootstrap.bundle.min.js"></script>
  <script src="/assets/vendor/glightbox/js/glightbox.js?version=)" + PROGRAM_BUILD_S + R"("></script>
  <script src="/assets/vendor/isotope-layout/isotope.pkgd.min.js"></script>
  <script src="/assets/vendor/swiper/swiper-bundle.min.js"></script>

  <!-- Template Main JS File -->
  <script src="/assets/js/main.js?version=)" + PROGRAM_BUILD_S + R"("></script>

</body>

</html>
)";
    else if (_PageType == PT_NOCRUMB)
        return OutputFrameCode(PT_FRAME, R"(
  <!-- ======= Top Bar ======= -->
  <section id="topbar" class="d-flex align-items-center">
    <div class="container d-flex justify-content-center justify-content-md-between">
      <div class="contact-info d-flex align-items-center">
        <i class="=d-flex align-items-center"> 当前会话：)" + Frame_GetHostName() + R"(</i>
      </div>
    </div>
  </section>

  <!-- ======= Header ======= -->
  <header id="header" class="d-flex align-items-center">
    <div class="container d-flex align-items-center justify-content-between">

      <div class="logo">
        <h3><a href="/">Coral Remote Controller</a></h1>
      </div>

      <nav id="navbar" class="navbar">
)" + FeatureList + R"(
      </nav><!-- .navbar -->

    </div>
  </header><!-- End Header -->

  )" + _CustomCode + R"(

  <!-- ======= Footer ======= -->
)" +
#ifdef BETA
R"(
  <footer class="bg-success">
	<center style="color:white; padding:5px; font-size:12px"> 仅用于测试。版本 )" + GetCompileInfo() + R"( </center>
  </footer>
)"
#else
""
#endif
+ R"(
  <footer id="footer" style="background-image: url(/assets/img/background.jpg?400); background-size:100%; background-repeat:no-repeat; background-position:center;">
      <div>
	    <center>
			<br>
			<h4 style="color:white"> Coral Remote Controller </h2>
			<h5 style="color:white"> 版本 )" + VERSION + R"( </h4>
			<p style="color:white"> Copyright &copy; 2020 - 2025 Coral Studio. All Rights Reserved. </p>
			<br>
	    </center>
	  </div>
  </footer><!-- End Footer -->

  <a href="#" class="back-to-top d-flex align-items-center justify-content-center"><i class="bi bi-arrow-up-short"></i></a>
)");
    else if (_PageType == PT_INDEX)
        return OutputFrameCode(PT_NOCRUMB, R"(
  <main id="main">

    <!-- ======= Breadcrumbs ======= -->
    <section id="breadcrumbs" class="breadcrumbs">
      <div class="container">
        <div class="d-flex justify-content-between align-items-center">
          <h2>)" + PageTitle + R"(</h2>
          <ol>
)" + BreadCrumb + R"(
          </ol>
        </div>
      </div>
    </section><!-- End Breadcrumbs -->

    <!-- ======= Portfolio Details Section ======= -->
    <section id="portfolio-details" class="portfolio-details">
      <div class="container">
)" + _CustomCode + R"(
      </div>
    </section><!-- End Portfolio Details Section -->

  </main><!-- End #main -->
)");

    return "";
}

std::string Frame_GetHostName()
{
    WSAData data;
    if (WSAStartup(MAKEWORD(1, 1), &data) != 0)
    {
        WSACleanup();
        return "未知";
    }
    char host[255];
    if (gethostname(host, sizeof(host)) == SOCKET_ERROR)
    {
        WSACleanup();
        return "未知";
    }
    else
    {
        WSACleanup();
        return host;
    }
}