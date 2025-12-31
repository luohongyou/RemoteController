// rcagent.cpp : Coral Remote Controller 后台操作响应程序

// 版权声明：
// Copyright (C) 2020-2026 Coral Studio. All Rights Reserved.

#define AGENTVERSION "5.0"

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <iostream>
#include <Windows.h>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <thread>
#include <Mmsystem.h>
#include <atlconv.h>
#include <ShlObj.h>
#include <shlobj_core.h>
#include <tchar.h>
#include <Iphlpapi.h>
#include "resource.h"
#include "../../../HTMLFrame/HTMLFrame.h"
#include "win32-darkmode.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "Iphlpapi.lib")
using namespace std;

std::string AgentDatabase;

HWND BackgroundhWnd;
HINSTANCE hInst;
NOTIFYICONDATA nid = { 0 };
bool IsTray = 1;

char buf[25600] = "";
std::string Action, User, Output;
std::string FileName, ShortFileName;
char output[2000];

bool BlackScreenFlag;

#define WM_NOTIFYICON WM_USER + 0x01

struct Window
{
    string Title;
    string ClassName;
    HWND hWnd;
    int WindowID;
    int PID;
} List[10001];
bool cmp(Window A, Window B)
{
    if (A.Title == B.Title)
        return A.ClassName < B.ClassName;
    return A.Title < B.Title;
}
int cnt = 0;

std::vector<int> CloseWindowsPIDs;

void CreateAgentDataFolder();

void ShootScreen(const char* filename, HWND hWnd);
void CaptureWindow(const char* filename, HWND hwnd);
void CreateBackgroundWindow();
void StartRealtimeScreenHost();
void BlackScreenBackgroundHost();
void StartBlackScreenHost(std::string Action = "Default");
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK EnumWindowsProc(HWND, LPARAM);
BOOL CALLBACK CloseWindow(HWND hwnd, LPARAM lParam);

std::string GetAgentCompileInfo();
std::string ConvertCRLFToPlatform(const std::string& input);

int APIENTRY WinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    HANDLE hMutex = NULL;
    hMutex = CreateMutexA(NULL, FALSE, "__CoralRemoteAgent");
    if (hMutex != NULL)
        if (ERROR_ALREADY_EXISTS == GetLastError())
        {
            ReleaseMutex(hMutex);
            return 0;
        }

    SetAppDirectory("\\..\\..\\..\\Website\\htdocs\\");
    CreateAgentDataFolder();

    InitDarkMode();

    srand(time(NULL));
    DWORD rlen = 0;
    HANDLE hPipe = CreateNamedPipe(TEXT("\\\\.\\Pipe\\CoralRemoteAgent"),
        PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES,
        0, 0, 5000, NULL);
    if (INVALID_HANDLE_VALUE == hPipe)
        return 0;

    hInst = hInstance;

    std::thread Window(CreateBackgroundWindow);
    Window.detach();

    if (GetSettings("Log", true))
    {
        std::ofstream fout(AgentDatabase + "rcagent.log", std::ios::app);
        fout << "\n==============================================================================\n";
        sprintf_s(output, "Coral Remote Controller, Version %s (%s)\n", VERSION, GetCompileInfo().c_str());
        fout << output;
        sprintf_s(output, "Coral Remote Agent, Version %s.%s\n", AGENTVERSION, GetAgentCompileInfo().c_str());
        fout << output;
        sprintf_s(output, "Current System Time: %s\n\n", StandardTime(false).c_str());
        fout << output;
        fout.close();
    }

    if (GetSettings("RealtimeScreen", false))
        StartRealtimeScreenHost();

    BlackScreenFlag = GetSettings("BlackScreen", false);
    StartBlackScreenHost();

    while (true)
    {
        if (!ConnectNamedPipe(hPipe, NULL))
            ;// printf("Connection failed!\n");

        while (true)
        {
            memset(buf, 0, sizeof(buf));
            if (!ReadFile(hPipe, buf, sizeof(buf), &rlen, NULL))
                break;

            else
            {
                std::stringstream tempIO;
                tempIO << buf;
                tempIO >> Action >> User;
                Output = "";

                if (GetSettings("Log", true))
                {
                    std::ofstream fout(AgentDatabase + "rcagent.log", std::ios::app);
                    sprintf_s(output, "%s\tAction:%s; User:%s\n", StandardTime(false).c_str(), Action.c_str(), User.c_str());
                    fout << output;
                    fout.close();
                }

                if (Action == "printscreen")
                {
                    int HWND_ID = 0;
                    tempIO >> HWND_ID;
                    HWND hWnd = reinterpret_cast<HWND>(HWND_ID);

                    std::stringstream tempIO;
                    tempIO << "Screenshot-" << User << '-' << StandardTime(true);
                    tempIO >> ShortFileName;
                    FileName = "storage\\screenshot\\" + ShortFileName;

                    if (hWnd != NULL)
                        CaptureWindow(FileName.c_str(), hWnd);
                    else
                        ShootScreen(FileName.c_str(), NULL);


                    Output = ShortFileName;
                }
                else if (Action == "listwindows")
                {
                    cnt = 0;
                    EnumWindows(EnumWindowsProc, 0);
                    sort(List + 1, List + 1 + cnt, cmp);
                    Output += Num2String(cnt) + "\n";
                    for (int i = 1; i <= cnt; ++i)
                    {
                        Output += List[i].Title + "\n";
                        Output += Num2String(List[i].WindowID) + "\n";
                        Output += Num2String(List[i].PID) + "\n";
                    }
                }
                else if (Action == "getfolders")
                {
                    PWSTR __Path = NULL;

                    std::string Data;
                    tempIO >> Data;
                    if (Data == "Desktop")
                        SHGetKnownFolderPath(FOLDERID_Desktop, KF_FLAG_DEFAULT, NULL, &__Path);
                    else if (Data == "Downloads")
                        SHGetKnownFolderPath(FOLDERID_Downloads, KF_FLAG_DEFAULT, NULL, &__Path);
                    else if (Data == "Documents")
                        SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_DEFAULT, NULL, &__Path);
                    else if (Data == "Pictures")
                        SHGetKnownFolderPath(FOLDERID_Pictures, KF_FLAG_DEFAULT, NULL, &__Path);
                    else if (Data == "Music")
                        SHGetKnownFolderPath(FOLDERID_Music, KF_FLAG_DEFAULT, NULL, &__Path);
                    else if (Data == "Videos")
                        SHGetKnownFolderPath(FOLDERID_Videos, KF_FLAG_DEFAULT, NULL, &__Path);
                    Output += PWSTR2string(__Path);
                }
                else if (Action == "updateformat")
                {
                    char Dir[500];
                    SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, Dir);
                    std::string OldDir = Dir + (std::string)"\\Coral Remote Conreoller\\"; // wrong spelling
                    RemoveDir(OldDir.c_str());

                    Output = "Yes";
                }
                else if (Action == "getversion")
                {
                    Output = (string)AGENTVERSION + "." + GetAgentCompileInfo();
                }
                else if (Action == "closewindow")
                {
                    int HWND_ID;
                    bool IsForce;
                    tempIO >> HWND_ID >> IsForce;

                    HWND hWnd = reinterpret_cast<HWND>(HWND_ID);

                    if (IsForce)
                    {
                        DWORD dwProcessID;
                        dwProcessID = 0;
                        GetWindowThreadProcessId(hWnd, &dwProcessID);
                        // HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, dwProcessID);
                        // ::TerminateProcess(hProcess, 0);
                        // CloseHandle(hProcess);
                        Output = Num2String(dwProcessID);
                    }
                    else
                    {
                        std::thread CloseWindow([hWnd](void) { SendMessage(hWnd, WM_CLOSE, NULL, NULL); });
                        CloseWindow.detach();
                        Output = "-1";
                    }
                }
                else if (Action == "shutdown")
                {
                    int Action, Delay;
                    bool IsForce;
                    std::string sysAction = "shutdown ";
                    tempIO >> Action >> IsForce >> Delay;

                    switch (Action)
                    {
                    case 0:  // 取消
                        sysAction += "-a";
                        break;
                    case 1:  // 关机
                        sysAction += "-s -t " + Num2String(Delay);
                        break;
                    case 2:  // 重启
                        sysAction += "-r -t " + Num2String(Delay);
                        break;
                    case 3:  // 休眠
                        sysAction += "-h";
                        break;
                    }
                    if (IsForce)
                        sysAction += " -f";

                    if (Action != 3)
                        WinExec(sysAction.c_str(), SW_HIDE);
                    else
                    {
                        std::thread Sleep([sysAction](void) {Sleep(500); WinExec(sysAction.c_str(), SW_HIDE); });
                        Sleep.detach();
                    }

                    Output = "Yes";
                }
                else if (Action == "onewaymessage")
                {
                    std::string Type, Method, Title, Content;
                    tempIO >> Type >> Method >> Title >> Content;

                    ShellExecuteA(NULL, "open", "..\\..\\ManageUI\\messager\\onewaymessager.exe", (Type + " " + Method + " " + Title + " " + Content + " " + User).c_str(), NULL, SW_SHOW);

                    Output = "Yes";
                }
                else if (Action == "getlogfile")
                {
                    Output = AgentDatabase + "rcagent.log";
                }
                else if (Action == "removelogfile")
                {
                    remove((AgentDatabase + "rcagent.log").c_str());

                    Output = "Yes";
                }
                else if (Action == "getclipboard")
                {
                    if (OpenClipboard(BackgroundhWnd))
                    {
                        HANDLE hClipboardData = GetClipboardData(CF_TEXT);
                        if (hClipboardData)
                        {
                            char* chData = static_cast<char*>(GlobalLock(hClipboardData));
                            if (chData)
                            {
                                Output = chData;
                                GlobalUnlock(hClipboardData);

                                Output = ConvertCRLFToPlatform(Output);
                            }
                        }
                        CloseClipboard();
                    }
                }
                else if (Action == "modifyclipboard")
                {
                    std::string Encoded;
                    tempIO >> Encoded;

                    std::wstring Decoded;
                    UTF_8ToUnicode(Decoded, decodeURI(HexDecode(Encoded)).c_str());

                    bool IsEmpty = 0;
                    for (int i = Decoded.size() - 1; i >= 0; i--)
                    {
                        if (Decoded[i] == '\n' || Decoded[i] == ' ' || Decoded[i] == '\r')
                            Decoded.erase(i);
                        else break;

                        if (i == 0)
                            IsEmpty = 1;
                    }

                    if (OpenClipboard(BackgroundhWnd))
                    {
                        EmptyClipboard();

                        if (!IsEmpty)
                        {
                            size_t text_len = (wcslen(Decoded.c_str()) + 1) * sizeof(wchar_t);
                            HGLOBAL hglbCopy = GlobalAlloc(GMEM_MOVEABLE, text_len);
                            wchar_t* lptstrCopy = (wchar_t*)GlobalLock(hglbCopy);
                            memcpy(lptstrCopy, Decoded.c_str(), text_len);
                            GlobalUnlock(hglbCopy);
                            SetClipboardData(CF_UNICODETEXT, hglbCopy);
                        }

                        CloseClipboard();
                    }

                    Output = "Yes";
                }
                else if(Action == "getclipboardhistory")
                {
                    Output = AgentDatabase + "clipboard.dat";
                }
                else if (Action == "gettraystatus")
                {
                    std::string Response = "1";
                    ifstream fin(AgentDatabase + "tray");
                    if (!fin.fail())
                        fin >> Response;
                    fin.close();

                    Output = Response;
                }
                else if (Action == "settraystatus")
                {
                    bool Response = 1;
                    tempIO >> Response;
                    ofstream fout(AgentDatabase + "tray");
                    fout << Response;
                    fout.close();

                    if (IsTray != Response)
                    {
                        IsTray = Response;
                        if (IsTray)
                        {
                            nid.cbSize = sizeof(NOTIFYICONDATA);
                            nid.hWnd = BackgroundhWnd;
                            nid.uID = IDI_ICON1;
                            nid.hIcon = ::LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
                            nid.uCallbackMessage = WM_NOTIFYICON;
                            nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
                            _tcscpy_s(nid.szTip, ((std::wstring)L"Coral Remote Controller\n版本 " + _T(VERSION)).c_str());
                            Shell_NotifyIcon(NIM_ADD, &nid);
                        }
                        else
                        {
                            nid.cbSize = sizeof(NOTIFYICONDATA);
                            nid.hWnd = BackgroundhWnd;
                            nid.uID = IDI_ICON1;
                            Shell_NotifyIcon(NIM_DELETE, &nid);
                        }
                    }

                    Output = "Yes";
                }
                else if (Action == "exit")
                {
                    DestroyWindow(BackgroundhWnd);

                    Output = "Yes";

                    DWORD wlen = 0;
                    WriteFile(hPipe, Output.c_str(), Output.size(), &wlen, 0);
                    FlushFileBuffers(hPipe);
                    DisconnectNamedPipe(hPipe);

                    exit(0);
                }
                else if (Action == "startprotect")
                {
                    ShellExecuteA(NULL, "open", "..\\..\\ManageUI\\agent\\agentprotect.exe", "", NULL, SW_HIDE);
                    Output = "Yes";
                }
                else if (Action == "reset")
                {
                    RemoveDir(AgentDatabase.c_str());

                    StopProcess("agentprotect.exe");
                    ShellExecuteA(NULL, "open", "..\\..\\ManageUI\\agent\\agentprotect.exe", "", NULL, SW_HIDE);

                    DestroyWindow(BackgroundhWnd);

                    Output = "Yes";
                    DWORD wlen = 0;
                    WriteFile(hPipe, Output.c_str(), Output.size(), &wlen, 0);
                    FlushFileBuffers(hPipe);
                    DisconnectNamedPipe(hPipe);

                    exit(0);
                }
                else if (Action == "getwallpaper")
                {
                    char buff[MAX_PATH];
                    memset(buff, 0, sizeof(buff));
                    SystemParametersInfoA(SPI_GETDESKWALLPAPER, MAX_PATH, buff, 0);

                    if (strcmp(buff, ""))
                    {
                        char Dir[500];
                        SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, Dir);

                        int ans = CopyFileA(((std::string)Dir + "\\Microsoft\\Windows\\Themes\\TranscodedWallpaper").c_str(), "temp\\wallpaper\\currentwallpaper.jpg", false);

                        if (ans != 0)
                            Output = "Yes";
                        else
                            Output = Num2String(GetLastError());
                    }
                    else Output = "False";
                }
                else if (Action == "setwallpaper")
                {
                    std::string FileName;
                    tempIO >> FileName;

                    char strModule[256];
                    GetModuleFileNameA(NULL, strModule, 256);
                    std::string a;
                    a.assign(strModule);
                    a = a.substr(0, a.find("ManageUI\\agent\\rcagent.exe"));
                    a += "Website\\htdocs\\temp\\wallpaper\\";

                    const char* Path = (a + FileName).c_str();
                    SystemParametersInfoA(SPI_SETDESKWALLPAPER, 0, (PVOID)Path, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);

                    Output = "Yes";
                }
                else if (Action == "rcupdate")
                {
                    ShellExecuteA(NULL, "open", "temp\\agentupdater.exe", "", NULL, SW_HIDE);

                    StopProcess("agentprotect.exe");
                    StopProcess("rcauth.exe");
                    StopProcess("LocalUI.exe");
                    StopProcess("chatapp.exe");

                    DestroyWindow(BackgroundhWnd);
                    Output = "Yes";
                    DWORD wlen = 0;
                    WriteFile(hPipe, Output.c_str(), Output.size(), &wlen, 0);
                    FlushFileBuffers(hPipe);
                    DisconnectNamedPipe(hPipe);

                    exit(0);
                }
                else if (Action == "loginevent")
                {
                    std::string IPAddress;
                    tempIO >> IPAddress;
                    if (IsTray)
                    {
                        nid.uFlags = NIF_INFO | NIF_GUID;
                        nid.dwInfoFlags = NIIF_INFO | NIIF_RESPECT_QUIET_TIME;
                        _tcscpy_s(nid.szInfo, S2WS("位于 " + IPAddress + " 的用户 " + User + " 已登录 Coral Remote Controller").c_str());
                        _tcscpy_s(nid.szTip, ((std::wstring)L"Coral Remote Controller\n版本 " + _T(VERSION)).c_str());
                        lstrcpy(nid.szInfoTitle, S2WS(User + " 已登录 Remote Controller").c_str());
                        Shell_NotifyIcon(NIM_MODIFY, &nid);
                    }
                }
                else if (Action == "pinloginevent")
                {
                    std::string IPAddress;
                    tempIO >> IPAddress;
                    if (IsTray)
                    {
                        nid.uFlags = NIF_INFO | NIF_GUID;
                        nid.dwInfoFlags = NIIF_INFO | NIIF_RESPECT_QUIET_TIME;
                        _tcscpy_s(nid.szInfo, S2WS("来自 " + IPAddress + " 的用户已通过PIN登录共享云盘").c_str());
                        _tcscpy_s(nid.szTip, ((std::wstring)L"Coral Remote Controller\n版本 " + _T(VERSION)).c_str());
                        lstrcpy(nid.szInfoTitle, L"Remote Controller 共享云盘登录");
                        Shell_NotifyIcon(NIM_MODIFY, &nid);
                    }
                }
                else if (Action == "remoteopen")
                {
                    std::string Address;
                    tempIO >> Address;
                    Address = UTF8ToANSI(decodeURI(HexDecode(Address)));
                    ShellExecuteA(NULL, "open", Address.c_str(), "", NULL, SW_SHOW);
                    Output = "Yes";
                }
                else if (Action == "runbatchfile")
                {
                    bool Inter;
                    std::string _GUID;
                    tempIO >> Inter >> _GUID;
                    if (!Inter)
                    {
                        std::thread BatchFile([_GUID](void) {
                            AllocConsole();
                            ShowWindow(GetConsoleWindow(), SW_HIDE);

                            std::string result;
                            FILE* pipe = _popen(("temp\\data\\" + _GUID + "\\UserScript.bat").c_str(), "r");
                            if (pipe)
                            {
                                char buffer[500];
                                while (fgets(buffer, sizeof(buffer), pipe))
                                    result += buffer;
                                _pclose(pipe);
                            }
                            else
                                result = "执行命令时出现错误。";
                            std::ofstream fout;
                            fout.open("temp\\data\\" + _GUID + "\\result.log");
                            fout << result;
                            fout.close();
                        });
                        BatchFile.detach();
                    }
                    else
                        ShellExecuteA(NULL, "open", ("temp\\data\\" + _GUID + "\\UserScript.bat").c_str(), "", NULL, SW_SHOW);
                    Output = "Yes";
                }
                else if (Action == "startrealtimescreen")
                {
                    StartRealtimeScreenHost();
                    Output = "Yes";
                }
                else if (Action == "LockPC")
                {
                    std::string LockAction;
                    tempIO >> LockAction;

                    Output = "Yes";

                    if (LockAction == "Default" || LockAction == "NoMinimize")
                    {
                        if (LockAction == "NoMinimize")
                        {
                            CloseWindowsPIDs.clear();
                            EnumWindows(CloseWindow, NULL);
                            Output = Num2String(CloseWindowsPIDs.size()) + " ";
                            for (auto i : CloseWindowsPIDs)
                                Output += Num2String(i) + " ";
                        }

                        SetSettings("BlackScreen", 1);
                        BlackScreenFlag = 1;
                        StartBlackScreenHost(LockAction);
                    }
                    else if (LockAction == "Disable")
                    {
                        SetSettings("BlackScreen", 0);
                        BlackScreenFlag = 0;
                        StopProcess("BlackScreenUI.exe");
                    }
                }

                DWORD wlen = 0;
                WriteFile(hPipe, Output.c_str(), Output.size(), &wlen, 0);
            }
        }
        FlushFileBuffers(hPipe);
        DisconnectNamedPipe(hPipe);
    }

    return 0;
}

void CreateAgentDataFolder()
{
    char Dir[500];
    SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, Dir);
    AgentDatabase = Dir + (std::string)"\\Coral Remote Controller\\";

    ifstream fin((string)AgentDatabase + "Version");
    if (!fin.fail())
        return;

    CreateDirectoryA(AgentDatabase.c_str(), NULL);

    ofstream fout((string)AgentDatabase + "Version");
    fout << AGENTVERSION;
    fout.close();
}

void CreateBackgroundWindow()
{
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = L"RCAGENT";
    RegisterClass(&wc);

    BackgroundhWnd = CreateWindowEx(NULL, L"RCAGENT", L"Coral Remote Agent", NULL,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, hInst, nullptr);

    if (BackgroundhWnd == nullptr)
        exit(0);

    if (!AddClipboardFormatListener(BackgroundhWnd))
        exit(0);

    ifstream fin(AgentDatabase + "tray");
    if (!fin.fail())
        fin >> IsTray;
    fin.close();

    if (IsTray)
    {
        nid.cbSize = sizeof(NOTIFYICONDATA);
        nid.hWnd = BackgroundhWnd;
        nid.uID = IDI_ICON1;
        nid.hIcon = ::LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
        nid.uCallbackMessage = WM_NOTIFYICON;
        nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
        _tcscpy_s(nid.szTip, ((std::wstring)L"Coral Remote Controller\n版本 " + _T(VERSION)).c_str());
        Shell_NotifyIcon(NIM_ADD, &nid);
    }

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    DestroyWindow(BackgroundhWnd);
}
void StartRealtimeScreenHost()
{
    StopProcess("jsmpeg_vnc_coral.exe");
    ShellExecuteA(NULL, "open", "..\\..\\ManageUI\\jsmpeg-vnc\\jsmpeg_vnc_coral.exe", "-p 8089 -i 0 -f 30 \"desktop\"", "..\\..\\ManageUI\\jsmpeg-vnc", SW_HIDE);

    if (GetSettings("Log", true))
    {
        std::ofstream fout(AgentDatabase + "rcagent.log", std::ios::app);
        sprintf_s(output, "%s\tAction:%s; User:%s\n", StandardTime(false).c_str(), "动态屏幕后台服务程序已启动，端口8089", "SYSTEM");
        fout << output;
        fout.close();
    }
}
void BlackScreenBackgroundHost()
{
    Sleep(1000);

    while (BlackScreenFlag)
    {
        if (FindWindow(L"CoralRCBlackScreenWindow", L"Coral Remote Cotroller 屏幕锁定") == NULL)
            ShellExecuteA(NULL, "open", "..\\..\\ManageUI\\ui\\BlackScreenUI.exe", "", "", SW_SHOW);
        Sleep(500);
    }
}
void StartBlackScreenHost(std::string Action)
{
    StopProcess("BlackScreenUI.exe");

    if (BlackScreenFlag)
    {
        ShellExecuteA(NULL, "open", "..\\..\\ManageUI\\ui\\BlackScreenUI.exe", Action.c_str(), "", SW_SHOW);
        std::thread BlackScreenHost(BlackScreenBackgroundHost);
        BlackScreenHost.detach();
    }
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
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
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 分析菜单选择
        switch (wmId)
        {
        case ID_OPENWEB:
            ShellExecuteA(NULL, "open", "..\\..\\ManageUI\\ui\\LocalUI.exe", "", NULL, SW_SHOW);
            break;
        case ID_CLOUDPIN:
            ShellExecuteA(NULL, "open", "..\\..\\ManageUI\\ui\\LocalUI.exe", "CloudPIN", NULL, SW_SHOW);
            break;
        case ID_EXIT:
            ShellExecuteA(NULL, "runas", "..\\..\\ManageUI\\agent\\ExitRemoteController.exe", "", NULL, SW_HIDE);
            break;
        case ID_LISTIP:
        {
            string IPList = "本机所有IP地址：\n";
            bool Flag = 0;

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
                            IPList += pIpAddrString->IpAddress.String + (string)"\n", Flag = 1;
                        pIpAddrString = pIpAddrString->Next;
                    } while (pIpAddrString);
                    pIpAdapterInfo = pIpAdapterInfo->Next;
                }

            }
            if (pIpAdapterInfo)
                delete pIpAdapterInfo;

            if (Flag)
                MessageBoxA(NULL, IPList.c_str(), "Coral Remote Controller", MB_ICONINFORMATION | MB_OK);
            else
                MessageBoxA(NULL, "本机暂无可用的IP地址", "Coral Remote Controller", MB_ICONINFORMATION | MB_OK);

            break;
        }
        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
        }
    }
    break;
    case WM_CLIPBOARDUPDATE:
        if (OpenClipboard(BackgroundhWnd))
        {
            HANDLE hClipboardData = GetClipboardData(CF_TEXT);
            if (hClipboardData)
            {
                char* chData = static_cast<char*>(GlobalLock(hClipboardData));
                if (chData)
                {
                    std::string clipboardText(chData);
                    GlobalUnlock(hClipboardData);
                    clipboardText = ConvertCRLFToPlatform(clipboardText);

                    bool IsEmpty = 0;
                    for (int i = clipboardText.size() - 1; i >= 0; i--)
                    {
                        if (clipboardText[i] == '\n' || clipboardText[i] == ' ' || clipboardText[i] == '\r')
                            clipboardText.erase(i);
                        else break;

                        if (i == 0)
                            IsEmpty = 1;
                    }

                    if (IsEmpty)
                    {
                        CloseClipboard();
                        break;
                    }

                    // AgentDatabase + "clipboard.dat"
                    std::ofstream fout(AgentDatabase + "clipboard.dat", std::ios::app);
                    
                    GUID guid;
                    CoCreateGuid(&guid);
                    std::string GUID = GuidToString(guid);
                    fout << "RCDATA-" << GUID << "\n";
                    fout << StandardTime(false) << "\n";
                    fout << clipboardText << "\n";
                    fout << "RCDATA-" << GUID << "\n";
                    fout.close();
                }
            }
            CloseClipboard();
        }
        break;
    case WM_NOTIFYICON:
        if ((wParam == IDI_ICON1) && ((lParam == WM_RBUTTONDOWN) || (lParam == WM_LBUTTONDOWN)))
        {
            POINT pt;
            GetCursorPos(&pt);
            SetForegroundWindow(::BackgroundhWnd);
            HMENU hMenu, hMenu1;
            hMenu = LoadMenu(hInst, MAKEINTRESOURCEW(IDR_MENU1));
            hMenu1 = GetSubMenu(hMenu, 0);

            WPARAM ReturnMsg;
            ReturnMsg = TrackPopupMenu(hMenu1, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD, pt.x, pt.y, NULL, hWnd, NULL);
            
            if (ReturnMsg > 0)
            {
                SendMessage(::BackgroundhWnd, WM_COMMAND, ReturnMsg, 0);
                // ShowWindow(::hWnd, SW_SHOW);
                // SetForegroundWindow(::hWnd);
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
    case WM_ENDSESSION:
        if (GetSettings("AgentProtect", true))
        {
            StopProcess("agentprotect.exe");
            if (GetSettings("Log", true))
            {
                std::ofstream fout(AgentDatabase + "rcagent.log", std::ios::app);
                sprintf_s(output, "%s\tAction:已接收到系统关闭事件，Agent 保护进程已退出; User:SYSTEM\n", StandardTime(false).c_str());
                fout << output;
                fout.close();
            }
        }
        break;
    case WM_DESTROY:
        RemoveClipboardFormatListener(hWnd);
        if (IsTray)
        {
            nid.cbSize = sizeof(NOTIFYICONDATA);
            nid.hWnd = BackgroundhWnd;
            nid.uID = IDI_ICON1;
            Shell_NotifyIcon(NIM_DELETE, &nid);
        }
        if (GetSettings("Log", true))
        {
            std::ofstream fout(AgentDatabase + "rcagent.log", std::ios::app);
            sprintf_s(output, "%s\tAction:Remote Agent 已经退出; User:SYSTEM\n", StandardTime(false).c_str());
            fout << output;
            fout.close();
        }
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}


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

    // 清理资源
    delete[] buf;
    DeleteObject(hBmpScreen);
    DeleteDC(hmdc);
}

void CaptureWindow(const char* filename, HWND hwnd)
{
    HDC hdcScreen = GetDC(NULL); // 屏幕设备上下文
    HDC hdc = GetWindowDC(hwnd); // 窗口设备上下文
    RECT rect;
    GetClientRect(hwnd, &rect); // 获取窗口客户区域

    int ScrWidth = rect.right - rect.left;
    int ScrHeight = rect.bottom - rect.top;

    HDC hmdc = CreateCompatibleDC(hdcScreen); // 创建兼容的内存设备上下文
    HBITMAP hBmpScreen = CreateCompatibleBitmap(hdcScreen, ScrWidth, ScrHeight); // 创建兼容的位图
    HBITMAP holdbmp = (HBITMAP)SelectObject(hmdc, hBmpScreen); // 选中位图到内存设备上下文

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

    BitBlt(hmdc, 0, 0, ScrWidth, ScrHeight, hdc, 0, 0, SRCCOPY); // 复制窗口到内存设备上下文

    // 在这里你可以处理hbmp，例如保存为文件或者进行其他操作
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


    // 清理资源
    delete[] buf;
    DeleteObject(hBmpScreen);
    DeleteDC(hmdc);
    ReleaseDC(hwnd, hdc);
    ReleaseDC(NULL, hdcScreen);
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    char szTitle[200];
    char szClass[200];
    GetWindowTextA(hwnd, szTitle, sizeof(szTitle));
    GetClassNameA(hwnd, szClass, sizeof(szClass));
    if (szTitle[0] != '\0' && IsWindowVisible(hwnd))
    {
        /*
        if (strlen(szTitle) > 30)
            szTitle[29] = szTitle[30] = szTitle[31] = '.', szTitle[32] = '\0';
        if (strlen(szClass) > 25)
            szClass[24] = szClass[25] = szClass[26] = '.', szClass[27] = '\0';*/
        List[++cnt].Title = szTitle;
        List[cnt].ClassName = szClass;
        List[cnt].hWnd = hwnd;
        List[cnt].WindowID = reinterpret_cast<int>(hwnd);

        DWORD dwProcessID;
        dwProcessID = 0;
        GetWindowThreadProcessId(hwnd, &dwProcessID);
        
        List[cnt].PID = dwProcessID;
    }
    return TRUE;
}
BOOL CALLBACK CloseWindow(HWND hwnd, LPARAM lParam)
{
    if (IsWindowVisible(hwnd) && GetParent(hwnd) == NULL)
    {
        DWORD dwProcessID;
        dwProcessID = 0;
        GetWindowThreadProcessId(hwnd, &dwProcessID);
        if (dwProcessID == GetCurrentProcessId())
            return TRUE;

        CloseWindowsPIDs.push_back(dwProcessID);
    }
    return TRUE;
}

std::string GetAgentCompileInfo()
{
    std::string BetaCompileInfo;
    const char* month_names[] = { "Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec" };
    std::string timetemp;
    timetemp = __DATE__;
    if (timetemp[4] == ' ')
        timetemp[4] = '0';
    std::string year = timetemp.substr(9, 2);
    std::string day = timetemp.substr(4, 2);
    char month[5];
    std::string s_month = timetemp.substr(0, 3);
    int tmonth;
    for (int i = 0; i <= 11; i++)
        if (s_month == month_names[i])
        {
            tmonth = i + 1;
            break;
        }
    if (tmonth < 10)
        month[0] = '0', month[1] = tmonth + '0', month[2] = '\0';
    else
        month[0] = '1', month[1] = tmonth % 10 + '0', month[2] = '\0';
    timetemp = __TIME__;
    std::string hour = timetemp.substr(0, 2);
    std::string min = timetemp.substr(3, 2);
    BetaCompileInfo = year + month + day + hour + min;
    return BetaCompileInfo;
}
std::string ConvertCRLFToPlatform(const std::string& input)
{
    std::string platformNewLine = "\n";
    std::string result = input;
    size_t start_pos = 0;
    while ((start_pos = result.find("\r\n", start_pos)) != std::string::npos)
    {
        result.replace(start_pos, 2, platformNewLine);
        start_pos += platformNewLine.length();
    }
    return result;
}