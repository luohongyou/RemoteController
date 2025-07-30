// DeviceVolume.h : Coral Remote Controller 基本控制 音频设备控制模块。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#ifndef RC_DVOLUME
#define RC_DVOLUME

#include <Windows.h>
#include <Endpointvolume.h>
#include <Mmdeviceapi.h>

// 获取默认音频设备
IMMDevice* GetDefaultAudioDevice()
{
    IMMDeviceEnumerator* deviceEnumerator = NULL;
    HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER,
        __uuidof(IMMDeviceEnumerator), (LPVOID*)&deviceEnumerator);
    if (FAILED(hr))
    {
        return NULL;
    }
    IMMDevice* defaultDevice = NULL;
    hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
    deviceEnumerator->Release();
    if (FAILED(hr))
    {
        return NULL;
    }
    return defaultDevice;
}

// 获取音量控制接口
IAudioEndpointVolume* GetAudioEndpointVolume(IMMDevice* device)
{
    IAudioEndpointVolume* endpointVolume = NULL;
    HRESULT hr = device->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER,
        NULL, (LPVOID*)&endpointVolume);
    if (FAILED(hr))
    {
        return NULL;
    }
    return endpointVolume;
}

// 获取当前音量（0-100）
int GetCurrentVolume(IAudioEndpointVolume* endpointVolume)
{
    float currentVolume = 0.0f; // 0.0 - 1.0
    HRESULT hr = endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);
    if (FAILED(hr))
    {
        return -1;
    }
    return int(currentVolume * 100); // convert to percentage
}

// 设置音量（0-100）
void SetCurrentVolume(IAudioEndpointVolume* endpointVolume, int volume)
{
    float newVolume = volume / 100.0f; // convert to scalar
    HRESULT hr = endpointVolume->SetMasterVolumeLevelScalar(newVolume, NULL);
}

// 获取静音
BOOL GetDeviceState(IAudioEndpointVolume* endpointVolume)
{
    BOOL deviceMuteState;
    HRESULT hRes = endpointVolume->GetMute(&deviceMuteState);
    return deviceMuteState;
}
// 设置静音
HRESULT SetDeviceMute(IAudioEndpointVolume* endpointVolume, int nMute)
{
    HRESULT hRes = endpointVolume->SetMute(nMute, NULL);
    return hRes;
}

#endif