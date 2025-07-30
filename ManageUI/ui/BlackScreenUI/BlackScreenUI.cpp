// LocalUI.cpp : Coral Remote Controller 屏幕锁定UI

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define STB_IMAGE_IMPLEMENTATION

#include "framework.h"
#include "BlackScreenUI.h"
#include <Windows.h>
#include <strsafe.h>
#include <io.h>
#include <stdlib.h>
#include <string>
#include <tchar.h>
#include <Iphlpapi.h>
#include <WinSock2.h>
#include <thread>
#include "stb_image.h"
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")
#pragma comment(lib, "Iphlpapi.lib")
#pragma comment(lib,"ws2_32.lib")

// 全局变量
HINSTANCE hInst;                                // 当前实例
HWND hWnd;                                      // 窗口句柄
double DPI;                                     // 高DPI显示比例
int width, height;
bool IsMinimize;

// 全局钩子句柄
HHOOK g_keyboardHook = NULL;
HHOOK g_mouseHook = NULL;

// 预定义控件信息
HDC hdcStatic;
HFONT TFont, TFont2, AppFont, AppFont2;

HWND MainTitle;
#define ID_MTitle 7000
HWND MainText;
#define ID_MText 7002
HWND ComputerInfo;
#define ID_ComInfo 7004
HWND RCInfo;
#define ID_RCInfo 7006

// 此代码模块中包含的函数的前向声明
void                CreateMainWindow(HINSTANCE hInstance);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void                BringToTopHost();
BOOL CALLBACK       MinimizeWindow(HWND hwnd, LPARAM lParam);
LRESULT CALLBACK    KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK    MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam);
void				InstallHooks();
void				UninstallHooks();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if ((std::wstring)lpCmdLine == L"Default")
		IsMinimize = 1;
	else if ((std::wstring)lpCmdLine == L"NoMinimize")
		IsMinimize = 0;
	else return 0;

	// 检查程序是否已经运行
	HWND ThWnd;
	ThWnd = FindWindow(L"CoralRCBlackScreenWindow", L"Coral Remote Cotroller 屏幕锁定");
	if (ThWnd != NULL)
	{
		ShowWindow(ThWnd, SW_SHOW);
		SetForegroundWindow(ThWnd);
		return 0;
	}

	// 初始化工作目录
	SetAppDirectory("\\..\\");  // 刻意未遵守标准定位到htdocs

	// 获取高DPI显示比例
	int TRoom = GetDeviceCaps(GetDC(NULL), LOGPIXELSX);
	DPI = TRoom / 96.0;
	width = GetSystemMetrics(SM_CXSCREEN);
	height = GetSystemMetrics(SM_CYSCREEN);

	// 初始化界面
	TFont = CreateFont(-60 * DPI, -27 * DPI, 0, 0, 600, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, L"微软雅黑");
	TFont2 = CreateFont(-24 * DPI, -11 * DPI, 0, 0, 400, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, L"微软雅黑");
	AppFont = CreateFont(-15 * DPI, -7 * DPI, 0, 0, 400, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, L"微软雅黑");
	AppFont2 = CreateFont(-12 * DPI, -6 * DPI, 0, 0, 300, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, L"微软雅黑");

	CreateMainWindow(hInstance);

	InstallHooks();
	ShowCursor(FALSE);

	if (IsMinimize)
		EnumWindows(MinimizeWindow, (LPARAM)hWnd);

	std::thread TopHost(BringToTopHost);
	TopHost.detach();

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BLACKSCREENUI));
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	UninstallHooks();
	ShowCursor(TRUE);

	return 0;
}

void ResizeBitmap(HBITMAP &newhBitmap, HBITMAP hBitmap, int originalWidth, int originalHeight, int newWidth, int newHeight)
{
	HDC hdcSrc = CreateCompatibleDC(NULL);  // 创建源DC
	HDC hdcDest = CreateCompatibleDC(NULL); // 创建目标DC
	HGDIOBJ oldBitmap = SelectObject(hdcSrc, hBitmap); // 选择源位图到源DC
	newhBitmap = CreateCompatibleBitmap(hdcSrc, newWidth, newHeight); // 创建目标位图
	SelectObject(hdcDest, newhBitmap); // 选择到目标DC
	StretchBlt(hdcDest, 0, 0, newWidth, newHeight, hdcSrc, 0, 0, originalWidth, originalHeight, SRCCOPY); // 缩放位图
	SelectObject(hdcSrc, oldBitmap); // 恢复旧的位图对象
	DeleteDC(hdcSrc); // 删除源DC
	DeleteDC(hdcDest); // 删除目标DC
}
void CreateMainWindow(HINSTANCE hInstance)
{
	HWND hDesk;
	RECT rc;
	hDesk = GetDesktopWindow();
	GetWindowRect(hDesk, &rc);

	HBITMAP hBitmap = NULL;
	HBITMAP _hBitmap = NULL;
	int width, height, channels;
	unsigned char* image_data = stbi_load("..\\assets\\BlackScreen.png", &width, &height, &channels, 0);
	if (image_data != nullptr)
	{
		HDC hdc = GetDC(NULL);
		_hBitmap = CreateCompatibleBitmap(hdc, width, height);
		HGDIOBJ oldBitmap = SelectObject(hdc, _hBitmap);
		unsigned char* bitmapData = new unsigned char[width * height * 4];

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				int index = (y * width + x) * channels;
				int bmpIndex = (y * width + x) * 4;
				bitmapData[bmpIndex] = image_data[index + 2];
				bitmapData[bmpIndex + 1] = image_data[index + 1];
				bitmapData[bmpIndex + 2] = image_data[index + 0];
				bitmapData[bmpIndex + 3] = 255;
			}
		}

		BITMAPINFO bmi;
		ZeroMemory(&bmi, sizeof(bmi));
		bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
		bmi.bmiHeader.biWidth = width;
		bmi.bmiHeader.biHeight = -height;
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biCompression = BI_RGB;
		SetDIBits(hdc, _hBitmap, 0, height, bitmapData, &bmi, DIB_RGB_COLORS);
		ResizeBitmap(hBitmap, _hBitmap, width, height, rc.right, rc.bottom);

		SelectObject(hdc, oldBitmap);
		DeleteObject(_hBitmap);
		ReleaseDC(NULL, hdc);
		DeleteDC(hdc);
		delete[] bitmapData;
		stbi_image_free(image_data);
	}

	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BLACKSCREENUI));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = CreatePatternBrush(hBitmap);
	wcex.lpszClassName = L"CoralRCBlackScreenWindow";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	RegisterClassExW(&wcex);

	hInst = hInstance;
	hWnd = CreateWindowW(L"CoralRCBlackScreenWindow", L"Coral Remote Cotroller 屏幕锁定", WS_BORDER,
		0, 0, rc.right, rc.bottom, nullptr, nullptr, hInstance, nullptr);
	if (!hWnd)
		::exit(0);

	// SetParent(hWnd, GetForegroundWindow());
	// SetParent(hWnd, NULL);

	SetWindowLong(hWnd, GWL_STYLE, WS_BORDER);
	SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, rc.right, rc.bottom, SWP_SHOWWINDOW);

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	ForceSetForegroundWindow(hWnd);
}
std::string GetHostName()
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
std::string GetIPAddresses()
{
	bool Flag = 0;
	std::string IPList;

	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);

	DWORD netCardNum = 0;
	GetNumberOfInterfaces(&netCardNum);

	int IPnumPerNetCard = 0;
	if (ERROR_BUFFER_OVERFLOW == nRel)
	{
		delete pIpAdapterInfo;
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
		nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	}
	if (ERROR_SUCCESS == nRel)
	{
		while (pIpAdapterInfo)
		{
			IPnumPerNetCard = 0;

			IP_ADDR_STRING* pIpAddrString = &(pIpAdapterInfo->IpAddressList);
			do
			{
				if (strcmp(pIpAddrString->IpAddress.String, "0.0.0.0"))
					IPList += pIpAddrString->IpAddress.String + (std::string)" ", Flag = 1;
				pIpAddrString = pIpAddrString->Next;
			} while (pIpAddrString);
			pIpAdapterInfo = pIpAdapterInfo->Next;
		}

	}
	if (pIpAdapterInfo)
		delete[] pIpAdapterInfo;

	if (Flag)
		return IPList;
	else
		return "暂无";

}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		MainTitle = CreateWindowA("static", "计算机已被锁定",
			WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE 
			| SS_CENTER,
			0, height / 2 - 140 * DPI, width, 100 * DPI,
			hWnd, (HMENU)ID_MTitle, hInst, NULL
		);
		SendMessage(MainTitle, WM_SETFONT, (WPARAM)TFont, NULL);

		MainText = CreateWindowA("static", "请使用 Coral Remote Controller 解锁计算机",
			WS_CHILD | WS_VISIBLE | SS_CENTER,
			0, height / 2 - 40 * DPI, width, 50 * DPI,
			hWnd, (HMENU)ID_MText, hInst, NULL
		);
		SendMessage(MainText, WM_SETFONT, (WPARAM)TFont2, NULL);

		ComputerInfo = CreateWindowA("static", ((std::string)"计算机名称: " + GetHostName() + "\tIP: " + GetIPAddresses()).c_str(),
			WS_CHILD | WS_VISIBLE | SS_CENTER,
			0, height / 2 + 10 * DPI, width, 50 * DPI,
			hWnd, (HMENU)ID_ComInfo, hInst, NULL
		);
		SendMessage(ComputerInfo, WM_SETFONT, (WPARAM)AppFont, NULL);

		RCInfo = CreateWindowA("static", ((std::string)"Coral Remote Controller " + VERSION + ", 版本 " + GetCompileInfo()).c_str(),
			WS_CHILD | WS_VISIBLE | SS_CENTER,
			0, height - 40 * DPI, width, 20 * DPI,
			hWnd, (HMENU)ID_RCInfo, hInst, NULL
		);
		SendMessage(RCInfo, WM_SETFONT, (WPARAM)AppFont2, NULL);
	}
	break;
	case WM_CTLCOLORSTATIC:
		hdcStatic = (HDC)wParam;
		SetTextColor(hdcStatic, RGB(255, 255, 255));
		SetBkMode(hdcStatic, TRANSPARENT);
		return (INT_PTR)GetStockObject(NULL_BRUSH);
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_CLOSE:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void BringToTopHost()
{
	while (true)
	{
		Sleep(500);

		ShowWindow(hWnd, SW_SHOW);
		HWND hDesk;
		RECT rc;
		hDesk = GetDesktopWindow();
		GetWindowRect(hDesk, &rc);
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, rc.right, rc.bottom, SWP_SHOWWINDOW);
		ForceSetForegroundWindow(hWnd);
	}
}
BOOL CALLBACK MinimizeWindow(HWND hwnd, LPARAM lParam)
{
	if (hwnd == (HWND)lParam)
		return TRUE;

	if (IsWindowVisible(hwnd) && GetParent(hwnd) == NULL)
		ShowWindow(hwnd, SW_MINIMIZE);
	return TRUE;
}

// 键盘钩子回调函数
LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0)
		return CallNextHookEx(g_keyboardHook, nCode, wParam, lParam);

	return 1;
}
// 鼠标钩子回调函数
LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0)
		return CallNextHookEx(g_mouseHook, nCode, wParam, lParam);

	return 1;
}
// 安装全局钩子
void InstallHooks()
{
	g_keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHookProc, GetModuleHandle(NULL), 0);
	g_mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, GetModuleHandle(NULL), 0);
}
// 卸载全局钩子
void UninstallHooks()
{
	if (g_keyboardHook)
	{
		UnhookWindowsHookEx(g_keyboardHook);
		g_keyboardHook = NULL;
	}

	if (g_mouseHook)
	{
		UnhookWindowsHookEx(g_mouseHook);
		g_mouseHook = NULL;
	}
}