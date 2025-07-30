// onewaymessager.cpp : Coral Remote Controller 单向通知本地显示用户界面

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "framework.h"
#include "onewaymessager.h"

#include <Windows.h>
#include <thread>
#include <sstream>
#include <string>
#include <atlconv.h>
#include <CommCtrl.h>
#include <commdlg.h>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")
#pragma comment(lib, "Comctl32.lib")

#define WM_NOTIFYICON WM_USER + 0x01

using namespace std;

std::wstring AppTitle, wTime, wUserName;

void TaskTopmost();
LRESULT CALLBACK TrayProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPSTR     lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    std::stringstream tempIO;
    tempIO << lpCmdLine;

    // MessageBoxA(NULL, lpCmdLine, "Debug", MB_OK);

    int Type = 0, Method = 0;
    std::string Title, Content, User;
    tempIO >> Type >> Method >> Title >> Content >> User;

    std::wstring tTitle, tContent;
    char cTitle[1000], cContent[4000];
    strcpy_s(cTitle, decodeURI(HexDecode(Title)).c_str());
    strcpy_s(cContent, decodeURI(HexDecode(Content)).c_str());

    UTF_8ToUnicode(tTitle, cTitle);
    UTF_8ToUnicode(tContent, cContent);

    std::string sAppTitle = StandardTime(0);
    UTF_8ToUnicode(wTime, sAppTitle);
    UTF_8ToUnicode(wUserName, User);

    if (Method == 2)
    {
        AppTitle = L"来自 " + wUserName + L" 的消息 - " + wTime;

        std::thread Topmost(TaskTopmost);
        Topmost.detach();

        PCWSTR MainIcon = TD_INFORMATION_ICON;
        switch (Type)
        {
        case 1:
            MainIcon = TD_INFORMATION_ICON;
            break;
        case 2:
            MainIcon = TD_WARNING_ICON;
            break;
        case 3:
            MainIcon = TD_ERROR_ICON;
            break;
        }

        InitCommonControls();
        TASKDIALOGCONFIG config = { 0 };
        config.cbSize = sizeof(config);
        config.hInstance = hPrevInstance;
        config.dwCommonButtons = 0;
        config.pszMainIcon = MainIcon;
        config.pszWindowTitle = AppTitle.c_str();
        config.hwndParent = NULL;
        config.pszMainInstruction = tTitle.c_str();
        config.pszContent = tContent.c_str();
        TaskDialogIndirect(&config, NULL, NULL, NULL);
    }
    else if (Method == 1)
    {
        AppTitle = L"来自 " + wUserName + L" 的消息";

        // 注册窗口类
        WNDCLASSEXW wcex;
        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = TrayProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = hInstance;
        wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ONEWAYMESSAGER));
        wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszMenuName = NULL;
        wcex.lpszClassName = L"RCONEWAYNOTIFY";
        wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
        RegisterClassExW(&wcex);

        HWND TrayhWnd = CreateWindowW(L"RCONEWAYNOTIFY", L"TrayWindow", WS_POPUPWINDOW,
            0, 0, 0, 0, nullptr, nullptr, hInstance, nullptr);

        if (!TrayhWnd)
            ::exit(0);

        ShowWindow(TrayhWnd, SW_HIDE);
        UpdateWindow(TrayhWnd);

        NOTIFYICONDATA nid = { 0 };
        nid.cbSize = sizeof(NOTIFYICONDATA);
        nid.hWnd = TrayhWnd;
        nid.uID = IDI_SMALL;
        nid.hIcon = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));
        nid.uCallbackMessage = WM_NOTIFYICON;
        nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
        _tcscpy_s(nid.szTip, AppTitle.c_str());
        Shell_NotifyIcon(NIM_ADD, &nid);

        DWORD MainIcon = NIIF_INFO;
        switch (Type)
        {
        case 1:
            MainIcon = NIIF_INFO;
            break;
        case 2:
            MainIcon = NIIF_WARNING;
            break;
        case 3:
            MainIcon = NIIF_ERROR;
            break;
        }

        nid.uFlags = NIF_INFO | NIF_GUID;
        nid.dwInfoFlags = MainIcon | NIIF_RESPECT_QUIET_TIME;
        _tcscpy_s(nid.szInfo, tContent.c_str());
        _tcscpy_s(nid.szTip, AppTitle.c_str());
        lstrcpy(nid.szInfoTitle, tTitle.c_str());
        nid.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));
        Shell_NotifyIcon(NIM_MODIFY, &nid);

        Sleep(10000);

        nid.cbSize = sizeof(NOTIFYICONDATA);
        nid.hWnd = TrayhWnd;
        nid.uID = IDI_SMALL;
        Shell_NotifyIcon(NIM_DELETE, &nid);
    }

    return 0;
}

void TaskTopmost()
{
    HWND hWnd = NULL;
    while (hWnd == NULL)
    {
        hWnd = FindWindow(NULL, AppTitle.c_str());
        if (hWnd != NULL)
            SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        Sleep(500);
    }
}

LRESULT CALLBACK TrayProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}