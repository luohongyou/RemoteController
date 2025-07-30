// DeviceVolume.h : Coral Remote Controller �������� ��Ƶ�豸����ģ�顣

// ��Ȩ������
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#ifndef RC_DVOLUME
#define RC_DVOLUME

#include <Windows.h>
#include <Endpointvolume.h>
#include <Mmdeviceapi.h>

// ��ȡĬ����Ƶ�豸
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

// ��ȡ�������ƽӿ�
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

// ��ȡ��ǰ������0-100��
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

// ����������0-100��
void SetCurrentVolume(IAudioEndpointVolume* endpointVolume, int volume)
{
    float newVolume = volume / 100.0f; // convert to scalar
    HRESULT hr = endpointVolume->SetMasterVolumeLevelScalar(newVolume, NULL);
}

// ��ȡ����
BOOL GetDeviceState(IAudioEndpointVolume* endpointVolume)
{
    BOOL deviceMuteState;
    HRESULT hRes = endpointVolume->GetMute(&deviceMuteState);
    return deviceMuteState;
}
// ���þ���
HRESULT SetDeviceMute(IAudioEndpointVolume* endpointVolume, int nMute)
{
    HRESULT hRes = endpointVolume->SetMute(nMute, NULL);
    return hRes;
}

#endif