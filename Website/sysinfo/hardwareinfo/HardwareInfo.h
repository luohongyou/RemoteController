// HardwareInfo.h : 获取系统的硬件信息。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#ifndef RC_HINFO
#define RC_HINFO

#include <Windows.h>
#include <string>
using namespace std;
#pragma warning(disable: 4996)

static const int kMaxInfoBuffer = 256;
#define  GBYTES  1073741824    
#define  MBYTES  1048576    
#define  KBYTES  1024    
#define  DKBYTES 1024.0    

string GetCPUName()
{
    bool IsFail = 0;
    std::string sKeyValue = "";
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hKey) != ERROR_SUCCESS)
    {
        RegCloseKey(hKey);
        IsFail = 1;
    }
    char chValue[256] = { 0 };
    DWORD dwSzType = REG_SZ;
    DWORD dwSize = sizeof(chValue);
    if (RegQueryValueExA(hKey, "ProcessorNameString", 0, &dwSzType, (LPBYTE)&chValue, &dwSize) != ERROR_SUCCESS)
        IsFail = 1;
    RegCloseKey(hKey);

    string CPU = chValue;
    if (!IsFail)
        return CPU;
    else
        return "Unknown";
}

string GetMemoryInfo()
{
    std::string memory_info;
    MEMORYSTATUSEX statusex;
    statusex.dwLength = sizeof(statusex);
    if (GlobalMemoryStatusEx(&statusex))
    {
        unsigned long long total = 0, remain_total = 0;
        double decimal_total = 0;
        remain_total = statusex.ullTotalPhys % GBYTES;
        total = statusex.ullTotalPhys / GBYTES;
        if (remain_total > 0)
            decimal_total = (remain_total / MBYTES) / DKBYTES;

        decimal_total += (double)total;
        char  buffer[kMaxInfoBuffer];
        sprintf_s(buffer, kMaxInfoBuffer, "%.2f GB", decimal_total);
        memory_info.append(buffer);
    }
    return memory_info;
}

DWORD GetProcessorCoreCount()
{
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION buffer = NULL;
    DWORD returnLength = 0;
    DWORD processCoreCount = 0;
    while (true)
    {
        DWORD ro = GetLogicalProcessorInformation(buffer, &returnLength);
        if (ro == FALSE)
        {
            if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
            {
                if (buffer)free(buffer);
                buffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)malloc(returnLength);
                if (buffer == NULL) return 0;
            }
            else
                return 0;
        }
        else break;
    }
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION ptr = buffer;
    DWORD byteOffest = 0;
    while (byteOffest + sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION) <= returnLength)
    {
        if (ptr->Relationship == RelationProcessorCore)
            ++processCoreCount;

        byteOffest += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
        ++ptr;
    }
    free(buffer);
    return processCoreCount;
}

#endif