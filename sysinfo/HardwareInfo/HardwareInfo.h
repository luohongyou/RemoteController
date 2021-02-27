#pragma once
#include <Windows.h>
#include <string>
using namespace std;
#pragma warning(disable: 4996)

static const int kMaxInfoBuffer = 256;
#define  GBYTES  1073741824    
#define  MBYTES  1048576    
#define  KBYTES  1024    
#define  DKBYTES 1024.0    

DWORD deax;
DWORD debx;
DWORD decx;
DWORD dedx;

void InitCpu(DWORD veax)
{
    __asm
    {
        mov eax, veax
        cpuid
        mov deax, eax
        mov debx, ebx
        mov decx, ecx
        mov dedx, edx
    }
}
string GetCpuType()
{
    const DWORD id = 0x80000002; // start 0x80000002 end to 0x80000004  
    char cpuType[49];
    memset(cpuType, 0, sizeof(cpuType));

    for (DWORD t = 0; t < 3; t++)
    {
        InitCpu(id + t);

        memcpy(cpuType + 16 * t + 0, &deax, 4);
        memcpy(cpuType + 16 * t + 4, &debx, 4);
        memcpy(cpuType + 16 * t + 8, &decx, 4);
        memcpy(cpuType + 16 * t + 12, &dedx, 4);
    }

    return cpuType;
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
                if (buffer == NULL)return 0;
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

double GetCPUFreq()
{
    int start1, start2;
    _asm rdtsc
    _asm mov start1, eax
    Sleep(50);
    _asm rdtsc
    _asm mov start2, eax
    return (start2 - start1) / 50000.0 / 1024.0;
}