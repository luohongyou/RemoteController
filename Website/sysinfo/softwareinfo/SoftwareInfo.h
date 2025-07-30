// SoftwareInfo.h : ��ȡϵͳ��Ӳ����Ϣ��

// ��Ȩ������
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#ifndef RC_SINFO
#define RC_SINFO

#include <string>
#include <cstdio>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

#define NT_SUCCESS(x) ((x)>=0) 
const UINT SystemTimeInformation = 3;

typedef struct {
    LARGE_INTEGER liKeBootTime;
    LARGE_INTEGER liKeSystemTime;
    LARGE_INTEGER liExpTimeZoneBias;
    ULONG uCurrentTimeZoneId;
    DWORD dwReserved;
} SYSTEM_TIME_INFORMATION;

typedef long(__stdcall* fnNtQuerySystemInformation)(
    IN  UINT SystemInformationClass,
    OUT PVOID SystemInformation,
    IN  ULONG SystemInformationLength,
    OUT PULONG ReturnLength OPTIONAL);

static fnNtQuerySystemInformation NtQuerySystemInformation = NULL;

std::string GetBootTime()
{
    NtQuerySystemInformation = (fnNtQuerySystemInformation)GetProcAddress(LoadLibrary(L"ntdll.dll"),
        "NtQuerySystemInformation");
    if (NtQuerySystemInformation == NULL)
        return "δ֪";
    LONG status;
    SYSTEM_TIME_INFORMATION sti;
    status = NtQuerySystemInformation(SystemTimeInformation, &sti, sizeof(sti), 0);
    if (NO_ERROR != status)
        return "δ֪";
    FILETIME ft1, ft2;
    SYSTEMTIME st;
    memcpy_s(&ft1, sizeof(ft1), &sti.liKeBootTime, sizeof(sti.liKeBootTime));
    if (0 == FileTimeToLocalFileTime(&ft1, &ft2))
        return "δ֪";
    if (0 == FileTimeToSystemTime(&ft2, &st))
        return "δ֪";
    char Time[100];
    sprintf_s(Time, "%04d-%02d-%02d %02d:%02d:%02d ", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
    return Time;
}

std::string GetHostName()
{
    // ��ʼ��:�������ʼ�������´��뽫�޷�ִ��
    WSAData data;
    if (WSAStartup(MAKEWORD(1, 1), &data) != 0)
    {
        WSACleanup();
        return "δ֪";
    }
    char host[255];
    if (gethostname(host, sizeof(host)) == SOCKET_ERROR)
    {
        WSACleanup();
        return "δ֪";
    }
    else
    {
        WSACleanup();
        return host;
    }
}

std::string GetBuildLabEx()
{
    bool IsFail = 0;
    std::string sKeyValue = "";
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", 0, KEY_READ, &hKey) != ERROR_SUCCESS)
    {
        RegCloseKey(hKey);
        IsFail = 1;
    }
    char chValue[256] = { 0 };
    DWORD dwSzType = REG_SZ;
    DWORD dwSize = sizeof(chValue);
    if (RegQueryValueExA(hKey, "BuildLabEx", 0, &dwSzType, (LPBYTE)&chValue, &dwSize) != ERROR_SUCCESS)
        IsFail = 1;
    RegCloseKey(hKey);

    std::string BuildLabEx = chValue;
    if (!IsFail)
        return BuildLabEx;
    else
        return "Unknown";
}


#endif