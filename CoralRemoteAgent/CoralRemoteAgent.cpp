// CoralRemoteAgent.cpp : Coral Remote Controller 后台操作响应程序

// 版权声明：
// Copyright (C) 2020-2021 Coral Studio. All Rights Reserved.

#define VERSION "1.0 beta 2"

#include <iostream>
#include <Windows.h>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <Mmsystem.h>
#pragma comment(lib, "winmm.lib")
using namespace std;

ifstream fin;
ofstream fout;

ofstream _log;

void ShootScreen(const char*, HWND);

struct Window
{
    string Title;
    string ClassName;
    HWND hWnd;
    int WindowID;
} List[10001];
bool cmp(Window A, Window B)
{
    if (A.Title == B.Title)
        return A.ClassName < B.ClassName;
    return A.Title < B.Title;
}
int cnt = 0;
BOOL CALLBACK EnumWindowsProc(HWND, LPARAM);

int main()
{
    // 初始化工作目录
    char strModule[256];
    GetModuleFileNameA(NULL, strModule, 256);
    std::string a;
    a.assign(strModule);
    a.append("\\..\\");
    SetCurrentDirectoryA(a.c_str());
    fin.open("config.ini");
    getline(fin, a);
    fin.close();
    SetCurrentDirectoryA(a.c_str());

    _log.open("temp\\log\\agent.html", ios::app);

    _log << "-----------------" << endl;
    _log << endl;

    _log << "Work Folder: " << a << endl;
    _log << endl;

    _log << " - Version: " << VERSION << endl;
    _log << endl;
    fout.open("agent\\version");
    fout << VERSION;
    fout.close();

    fin.open("sysinfo\\AIDA64\\Reports\\Report.htm");
    if (fin.fail())
    {
        ShellExecute(NULL, L"open", L"sysinfo\\AIDA64\\aida64.exe", L" /r /hw /mhtml /LANGCN /silent", NULL, SW_HIDE);
        _log << " - Create AIDA64 Report" << endl;
        _log << endl;
    }
    fin.close();

    remove("temp\\action\\printscreen");
    remove("temp\\action\\printscreen.success");
    remove("temp\\action\\enumwindows");
    remove("temp\\action\\enumwindows.success");
    remove("temp\\action\\closewindow");
    remove("temp\\action\\closewindow.success");
    remove("temp\\action\\runapp");
    remove("temp\\action\\runapp.success");
    remove("temp\\action\\PlayClassBegin");
    remove("temp\\action\\PlayClassEnd");

    while (true)
    {
        // 屏幕截图 截取屏幕
        fin.open("temp\\action\\printscreen");
        if (fin.good())
        {
            _log << " - PrintScreen Begin" << endl;
            fin.close();
            string FileName, ShortFileName;
            time_t now = time(0);
            struct tm ltm;
            localtime_s(&ltm, &now);
            stringstream tempIO;
            tempIO << "Screenshoot-" << 1900 + ltm.tm_year << '-' << 1 + ltm.tm_mon << '-' << ltm.tm_mday << '-' << ltm.tm_hour << '-' << ltm.tm_min << '-' << ltm.tm_sec << ".bmp";
            tempIO >> ShortFileName;
            FileName = "screenshoot\\cache\\" + ShortFileName;
            ShootScreen(FileName.c_str(), NULL);
            remove("temp\\action\\printscreen");
            fout.open("temp\\action\\printscreen.success");
            fout << ShortFileName;
            fout.close();
            _log << " - Save to file: " << ShortFileName << endl;
            _log << " - PrintScreen End" << endl;
            _log << endl;
        }
        else fin.close();
        Sleep(20);

        // 窗口管理 列出窗口
        fin.open("temp\\action\\enumwindows");
        if (fin.good())
        {
            _log << " - EnumWindows Begin" << endl;
            fin.close();
            remove("temp\\action\\enumwindows");
            cnt = 0;
            EnumWindows(EnumWindowsProc, 0);
            sort(List + 1, List + 1 + cnt, cmp);
            fout.open("temp\\action\\enumwindows.success");
            fout << cnt << endl;
            for (register int i = 1; i <= cnt; ++i)
            {
                fout << List[i].Title << endl;
                fout << List[i].ClassName << endl;
                fout << List[i].WindowID << endl;
            }
            fout.close();
            _log << " - EnumWindows End" << endl;
            _log << endl;
        }
        else fin.close();
        Sleep(20);

        // 窗口管理 关闭窗口
        fin.open("temp\\action\\closewindow");
        if (fin.good())
        {
            _log << " - CloseWindow Begin" << endl;
            int HWND_ID;
            bool IsForce;
            fin >> HWND_ID >> IsForce;
            fin.close();
            remove("temp\\action\\closewindow");
            _log << " - Window ID: " << HWND_ID << endl;
            HWND hWnd = reinterpret_cast<HWND>(HWND_ID);
            if (IsForce)
            {
                DWORD dwProcessID;
                dwProcessID = 0;
                GetWindowThreadProcessId(hWnd, &dwProcessID);
                HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, dwProcessID);
                ::TerminateProcess(hProcess, 0);
                CloseHandle(hProcess);

            }
            else
                SendMessage(hWnd, WM_CLOSE, NULL, NULL);
            fout.open("temp\\action\\closewindow.success");
            fout.close();
            _log << " - CloseWindow End" << endl;
            _log << endl;
        }
        else fin.close();
        Sleep(20);

        // 执行文件
        fin.open("temp\\action\\runapp");
        if (fin.good())
        {
            _log << " - RunAPP Begin" << endl;
            string Command;
            getline(fin, Command);
            fin.close();
            remove("temp\\action\\runapp");
            _log << " - Run: " << Command << endl;
            ShellExecuteA(NULL, "open", ("runapp\\upload\\" + Command).c_str(), "", NULL, SW_SHOW);
            fout.open("temp\\action\\runapp.success");
            fout.close();
            _log << " - RunAPP End" << endl;
            _log << endl;
        }
        else fin.close();
        Sleep(20);


        // Easter Eggs ClassBegin
        fin.open("temp\\action\\PlayClassBegin");
        if (fin.good())
        {
            _log << " - PlaySound ClassBegin" << endl;
            fin.close();
            remove("temp\\action\\PlayClassBegin");
            PlaySound(TEXT("sysinfo\\EasterEggs\\Resources\\ClassBegin.wav"), NULL, SND_FILENAME | SND_ASYNC);
            _log << endl;
        }
        else fin.close();
        Sleep(20);

        // Easter Eggs ClassEnd
        fin.open("temp\\action\\PlayClassEnd");
        if (fin.good())
        {
            _log << " - PlaySound ClassEnd" << endl;
            fin.close();
            remove("temp\\action\\PlayClassEnd");
            PlaySound(TEXT("sysinfo\\EasterEggs\\Resources\\ClassEnd.wav"), NULL, SND_FILENAME | SND_ASYNC);
            _log << endl;
        }
        else fin.close();
        Sleep(20);
    }

    return 0;
}

// function
void ShootScreen(const char* filename, HWND hWnd)
{
    HDC hdc = CreateDC(L"DISPLAY", NULL, NULL, NULL);
    int ScrWidth = 0, ScrHeight = 0;
    RECT rect = { 0 };
    if (hWnd == NULL)
    {
        ScrWidth = GetDeviceCaps(hdc, HORZRES);
        ScrHeight = GetDeviceCaps(hdc, VERTRES);
    }
    else
    {
        GetWindowRect(hWnd, &rect);
        ScrWidth = rect.right - rect.left;
        ScrHeight = rect.bottom - rect.top;
    }
    HDC hmdc = CreateCompatibleDC(hdc);

    HBITMAP hBmpScreen = CreateCompatibleBitmap(hdc, ScrWidth, ScrHeight);
    HBITMAP holdbmp = (HBITMAP)SelectObject(hmdc, hBmpScreen);

    BITMAP bm;
    GetObject(hBmpScreen, sizeof(bm), &bm);

    BITMAPINFOHEADER bi = { 0 };
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = bm.bmWidth;
    bi.biHeight = bm.bmHeight;
    bi.biPlanes = bm.bmPlanes;
    bi.biBitCount = bm.bmBitsPixel;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = bm.bmHeight * bm.bmWidthBytes;
    // 图片的像素数据
    char* buf = new char[bi.biSizeImage];
    BitBlt(hmdc, 0, 0, ScrWidth, ScrHeight, hdc, rect.left, rect.top, SRCCOPY);
    GetDIBits(hmdc, hBmpScreen, 0L, (DWORD)ScrHeight, buf, (LPBITMAPINFO)&bi, (DWORD)DIB_RGB_COLORS);

    BITMAPFILEHEADER bfh = { 0 };
    bfh.bfType = ((WORD)('M' << 8) | 'B');
    bfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bi.biSizeImage;
    bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    HANDLE hFile = CreateFileA(filename, GENERIC_WRITE, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    DWORD dwWrite;
    WriteFile(hFile, &bfh, sizeof(BITMAPFILEHEADER), &dwWrite, NULL);
    WriteFile(hFile, &bi, sizeof(BITMAPINFOHEADER), &dwWrite, NULL);
    WriteFile(hFile, buf, bi.biSizeImage, &dwWrite, NULL);
    CloseHandle(hFile);
    hBmpScreen = (HBITMAP)SelectObject(hmdc, holdbmp);
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    char szTitle[200];
    char szClass[200];
    GetWindowTextA(hwnd, szTitle, sizeof(szTitle));
    GetClassNameA(hwnd, szClass, sizeof(szClass));
    if (szTitle[0] != '\0' && IsWindowVisible(hwnd))
    {
        if (strlen(szTitle) > 30)
            szTitle[29] = szTitle[30] = szTitle[31] = '.', szTitle[32] = '\0';
        if (strlen(szClass) > 25)
            szClass[24] = szClass[25] = szClass[26] = '.', szClass[27] = '\0';
        List[++cnt].Title = szTitle;
        List[cnt].ClassName = szClass;
        List[cnt].hWnd = hwnd;
        List[cnt].WindowID = reinterpret_cast<int>(hwnd);
    }
    return TRUE;
}
