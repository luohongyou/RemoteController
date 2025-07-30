// RCWebView2.h : Coral Remote Controller 本地 Edge WebView 2 应用程序通用函数

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#ifndef RC_WEBVIEW2
#define RC_WEBVIEW2

#include <winsock.h>
#include <wrl.h>
#include <wil/com.h>
#include <AccCtrl.h>
#include <AclAPI.h>
#include <WebView2.h>
#pragma comment(lib, "WS2_32.lib")

using namespace Microsoft::WRL;

HANDLE hMutex = NULL;

static wil::com_ptr<ICoreWebView2Controller> WebviewController;
static wil::com_ptr<ICoreWebView2> WebviewWindow;
static wil::com_ptr<ICoreWebView2_22> WebviewWindow22;

void myReleaseMutex()
{
    ReleaseMutex(hMutex);
    CloseHandle(hMutex);
}
void CheckWebView2Runtime()
{
    wil::unique_cotaskmem_string version_info;
    HRESULT hr = GetAvailableCoreWebView2BrowserVersionString(nullptr, &version_info);
    if (hr != S_OK || version_info == nullptr)
    {
        MessageBoxA(NULL, "Microsoft Edge Webview 2 Runtime 未安装！", "Coral Remote Controller", MB_ICONERROR | MB_OK);
        ::exit(0);
    }

    std::ifstream fin;
    fin.open(DatabasePath() + "OOBEFlag");
    int Version = 1;
    if (!fin.fail())
        fin >> Version;
    fin.close();

    if (Version < 3)
    {
        if (MessageBoxA(NULL, "Coral Remote Controller 尚未完成初始化设置，要现在完成吗？", "Coral Remote Controller", MB_ICONWARNING | MB_YESNO) == IDYES)
            ShellExecuteA(NULL, "open", "http://localhost:8088", "", NULL, SW_SHOW);
        ::exit(0);
    }
}
void CreateWebView2(HWND hWnd, std::wstring Link)
{
    wchar_t OutPath[MAX_PATH];
    int StrLen = GetTempPath(MAX_PATH, OutPath);
    std::wstring UDFPath = (std::wstring)OutPath + L"CoralRemoteController.local.WebView2";

    HRESULT res = CreateCoreWebView2EnvironmentWithOptions(nullptr, UDFPath.c_str(), nullptr,
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [hWnd, Link](HRESULT result, ICoreWebView2Environment* env) -> HRESULT
            {
                // Create a CoreWebView2Controller and get the associated CoreWebView2 whose parent is the main window hWnd
                env->CreateCoreWebView2Controller(hWnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                    [hWnd, Link](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT
                    {
                        if (controller != nullptr)
                        {
                            WebviewController = controller;
                            WebviewController->get_CoreWebView2(&WebviewWindow);
                            WebviewWindow22 = WebviewWindow.try_query<ICoreWebView2_22>();
                        }

                        // Add a few settings for the webview
                        ICoreWebView2Settings* Settings;
                        WebviewWindow->get_Settings(&Settings);
                        Settings->put_IsScriptEnabled(TRUE);
                        Settings->put_AreDefaultScriptDialogsEnabled(TRUE);
                        Settings->put_IsWebMessageEnabled(TRUE);
                        Settings->put_AreDefaultContextMenusEnabled(FALSE);
#ifdef BETA
                        Settings->put_AreDevToolsEnabled(TRUE);
#else
                        Settings->put_AreDevToolsEnabled(FALSE);
#endif
                        Settings->put_IsStatusBarEnabled(FALSE);
                        Settings->put_IsZoomControlEnabled(FALSE);

                        // Add Token Info
                        ICoreWebView2CookieManager* cookieManager;
                        ICoreWebView2Cookie* RCToken;

                        WebviewWindow22->get_CookieManager(&cookieManager);
                        std::wstring LocalToken;
                        UTF_8ToUnicode(LocalToken, RCAuth("RequestForegroundToken", ""));
                        cookieManager->CreateCookie(L"CORAL_REMOTE_CONTROLLER_MANAGE", LocalToken.c_str()
                            , L"localhost", L"/", &RCToken);
                        cookieManager->AddOrUpdateCookie(RCToken);

                        // Resize WebView to fit the bounds of the parent window
                        RECT bounds;
                        GetClientRect(hWnd, &bounds);
                        WebviewController->put_Bounds(bounds);

                        // Coral Remote Controller
                        HRESULT res = WebviewWindow->Navigate(Link.c_str());
                        std::string sres = std::to_string(res).c_str();

                        return S_OK;
                    }).Get());
                return S_OK;
            }).Get());

}

#endif