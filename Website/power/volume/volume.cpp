// volume.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/power/volume.exe"

#include <iostream>
#include <Windows.h>
#include "DeviceVolume.h"
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\");

    if (argc != 3)
        return 0;

    HTMLFrame HTML;
    HTML.Register(PT_EMPTY, ADDRESS, "volume", "volume", "", true);

    if (HTML.User.GetUserType() == UT_GUEST)
        return 0;

    bool Mode = String2Num(argv[1]);
    int Data = String2Num(argv[2]);

    IMMDevice* device;
    IAudioEndpointVolume* endpointVolume;
    CoInitializeEx(NULL, COINIT_MULTITHREADED);
    device = GetDefaultAudioDevice();
    endpointVolume = GetAudioEndpointVolume(device);

    if (Mode)  // Mode == 1: Set Volume
        SetCurrentVolume(endpointVolume, Data);
    else       // Mode == 2: Set Mute
        SetDeviceMute(endpointVolume, Data);

    endpointVolume->Release();
    device->Release();
    CoUninitialize();

    AJAXOutput("Yes");
    std::string Action = (Mode == 1) ? argv[2] + (std::string)"%" : (Data ? "on" : "off");
    HTML.Log("成功调整了远程计算机音频：" + Action, "power", LL_INFO);

    return 0;
}
