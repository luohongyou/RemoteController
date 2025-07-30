// lockpc.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/power/lockpc.exe"

#include <iostream>
#include <Windows.h>
#include "../volume/DeviceVolume.h"
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\");

    if (argc != 3)
        return 0;

    HTMLFrame HTML;
    HTML.Register(PT_EMPTY, ADDRESS, "lockpc", "lockpc", "", true);

    if (HTML.User.GetUserType() == UT_GUEST || HTML.User.IsForegroundUser())
        return 0;

    int IsLock = String2Num(argv[1]);
    int Mode = String2Num(argv[2]);
    bool IsCloseAll = Mode / 2, IsSilent = Mode % 2;

    if (IsLock)
    {
        if (IsCloseAll)
        {
            std::string PIDList = RCAgent("LockPC", HTML.User.GetName(), "NoMinimize");
            std::stringstream tempIO;
            tempIO << PIDList;
            int cnt, PID;
            tempIO >> cnt;
            for (int i = 1; i <= cnt; i++)
            {
                tempIO >> PID;
                HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, PID);
                TerminateProcess(hProcess, 0);
                CloseHandle(hProcess);
            }
        }
        else
            RCAgent("LockPC", HTML.User.GetName(), "Default");

        if (IsSilent)
        {
            IMMDevice* device;
            IAudioEndpointVolume* endpointVolume;
            CoInitializeEx(NULL, COINIT_MULTITHREADED);
            device = GetDefaultAudioDevice();
            endpointVolume = GetAudioEndpointVolume(device);
            SetDeviceMute(endpointVolume, 1);
            endpointVolume->Release();
            device->Release();
            CoUninitialize();
        }

        AJAXOutput("Yes");
        HTML.Log((std::string)"成功锁定了远程计算机 (模式" + argv[2] + ")", "power", LL_INFO);

    }
    else
    {
        RCAgent("LockPC", HTML.User.GetName(), "Disable");
        HTML.Log("成功解除了对远程计算机的锁定", "power", LL_INFO);
    }

    return 0;
}
