// chatapp.cpp : Coral Remote Controller 双向消息本地显示用户界面

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define WINDOW_CLASS L"RCCHATAPP"
#define WINDOW_TITLE L"远程聊天"

#include "framework.h"
#include "chatapp.h"
#include <Windows.h>
#include <strsafe.h>
#include <io.h>
#include <stdlib.h>
#include <string>
#include <tchar.h>
#include "../../../HTMLFrame/HTMLFrame.h"
#include "../../agent/rcagent/win32-darkmode.h"
#include "../../ui/LocalUI/RCWebView2.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    SetAppDirectory("\\..\\..\\..\\Website\\htdocs\\");

    InitDarkMode();

    // 创建互斥体
    hMutex = CreateMutexA(NULL, FALSE, "RemoteControllerChatApp");
    if (hMutex != NULL)
        if (ERROR_ALREADY_EXISTS == GetLastError())
        {
            ReleaseMutex(hMutex);
            HWND hWnd = FindWindow(WINDOW_CLASS, WINDOW_TITLE);
            ShowWindow(hWnd, SW_SHOW);
            SetForegroundWindow(hWnd);
            return 0;
        }

    CheckWebView2Runtime();

    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CHATAPP));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = WINDOW_CLASS;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    RegisterClassExW(&wcex);

    int _TDPI = GetDeviceCaps(GetDC(NULL), LOGPIXELSX);
    double DPI = _TDPI / 96.0;

    int scrWidth, scrHeight;
    scrWidth = (GetSystemMetrics(SM_CXSCREEN) - 500 * DPI) / 2;
    scrHeight = (GetSystemMetrics(SM_CYSCREEN) - 500 * DPI) / 2;

    HWND hWnd = CreateWindowW(WINDOW_CLASS, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
        scrWidth, scrHeight, 500 * DPI, 500 * DPI, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
        return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    CreateWebView2(hWnd, L"http:\\localhost:8088");  // TODO

    // 主消息循环
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CHATAPP));
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        if (g_darkModeSupported)
        {
            _AllowDarkModeForWindow(hWnd, true);
            RefreshTitleBarThemeColor(hWnd);
        }
    }
    break;
    case WM_SETTINGCHANGE:
    {
        if (IsColorSchemeChangeMessage(lParam))
        {
            g_darkModeEnabled = _ShouldAppsUseDarkMode() && !IsHighContrast();
            RefreshTitleBarThemeColor(hWnd);
        }
    }
    break;
    case WM_SIZE:
        if (WebviewController != nullptr)
        {
            RECT bounds;
            GetClientRect(hWnd, &bounds);
            WebviewController->put_Bounds(bounds);
        };
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}