// hardwareinfo.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/sysinfo/hardwareinfo.exe"

#include <iostream>
#include "../../../HTMLFrame/HTMLFrame.h"
#include "HardwareInfo.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

using namespace std;
int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\");

    HTMLFrame HTML;
    HTML.Register(PT_INDEX, ADDRESS, "hardwareinfo", "hardwareinfo", "", true);

    string Code = R"(
    <center>
)";

    Code += "<h4> CPU 信息 </h4>\n";
    Code += "<div>" + GetCPUName() + "</div>\n";
    string Cores, Threads;// , Speed;
    stringstream tempIO_CPU;
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    tempIO_CPU << GetProcessorCoreCount() << ' ' << sysInfo.dwNumberOfProcessors; // << ' ' << GetCPUFreq();
    tempIO_CPU >> Cores >> Threads;// >> Speed;
    Code += "<div> " + Cores + " 核心，" + Threads + " 线程 </div>\n";
    // Code += "<div> 速度：" + Speed + " GHz </div>\n";
    Code += "<br>\n";

    Code += "<h4> 内存信息 </h4>\n";
    const double DIV = 1024 * 1024 * 1024;
    stringstream tempIO_Memory;
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);
    tempIO_Memory << statex.dwMemoryLoad << endl;
    tempIO_Memory << statex.ullTotalPhys / DIV << ' ' << statex.ullTotalPageFile / DIV << endl;
    tempIO_Memory << statex.ullAvailPhys / DIV << ' ' << statex.ullAvailPageFile / DIV;
    string MLoad, TotalPhys, TotalPageFile, AvailPhys, AvailPageFile;
    tempIO_Memory >> MLoad >> TotalPhys >> TotalPageFile >> AvailPhys >> AvailPageFile;
    Code += "<div> 已使用：" + MLoad + "% </div>\n";
    Code += "<div> 物理内存：" + TotalPhys + " GB，" + AvailPhys + " GB 可用 </div>\n";
    Code += "<div> 页面文件：" + TotalPageFile + " GB，" + AvailPageFile + " GB 可用 </div>\n";
    Code += "<br>\n";

    Code += "<h4> 存储设备 </h4>\n";
    int DiskCount = 0;
    DWORD DiskInfo = GetLogicalDrives();
    while (DiskInfo)
    {
        if (DiskInfo & 1)
            ++DiskCount;
        DiskInfo = DiskInfo >> 1;
    }
    stringstream tempIO_Disk;
    string Count;
    tempIO_Disk << DiskCount;
    tempIO_Disk >> Count;
    Code += "<div> 逻辑卷数量：" + Count + "</div>\n";
    int DSLength = GetLogicalDriveStringsA(0, NULL);
    char* DStr = new char[DSLength];
    GetLogicalDriveStringsA(DSLength, DStr);
    int DType;
    int si = 0;
    BOOL fResult;
    unsigned _int64 i64FreeBytesToCaller;
    unsigned _int64 i64TotalBytes;
    unsigned _int64 i64FreeBytes;
    for (int i = 0; i < DSLength / 4; ++i)
    {
        string DiskInfo;
        stringstream _tempIO;
        _tempIO << DStr[si];
        _tempIO >> DiskInfo;
        DiskInfo += ":";
        DType = GetDriveTypeA(DStr + i * 4);
        if (DType == DRIVE_FIXED)
            DiskInfo += " 硬盘，\t";
        else if (DType == DRIVE_CDROM)
            DiskInfo += " 光驱，\t";
        else if (DType == DRIVE_REMOVABLE)
            DiskInfo += " 可移动存储，\t";
        else if (DType == DRIVE_REMOTE)
            DiskInfo += " 网络位置，\t";
        else if (DType == DRIVE_RAMDISK)
            DiskInfo += " 虚拟磁盘，\t";
        else if (DType == DRIVE_UNKNOWN)
            DiskInfo += " 未知，\t";
        fResult = GetDiskFreeSpaceExA(
            (DStr[si] + (string)":\\").c_str(),
            (PULARGE_INTEGER)&i64FreeBytesToCaller,
            (PULARGE_INTEGER)&i64TotalBytes,
            (PULARGE_INTEGER)&i64FreeBytes);
        if (fResult)
        {
            string tempT, tempF;
            stringstream tempIO;
            tempIO << (float)i64TotalBytes / 1024 / 1024 / 1024 << ' ' << (float)i64FreeBytesToCaller / 1024 / 1024 / 1024;
            tempIO >> tempT >> tempF;
            DiskInfo += tempT + "GB，" + tempF + "GB 可用";
        }
        else
            DiskInfo += "错误";
        Code += "<div>" + DiskInfo + "</div>\n";
        si += 4;
    }
    Code += "<br>\n";

    Code += "<h4> 电池信息 </h4>\n";
    SYSTEM_POWER_STATUS sysPowerStatus;
    GetSystemPowerStatus(&sysPowerStatus);
    if (sysPowerStatus.BatteryFlag != 128)
    {
        Code += "<div> 电池存在：是 </div>\n";
        Code += "<div> 交流电源接入：" + (string)(sysPowerStatus.ACLineStatus ? "是" : "否") + " </div>\n";
        string BPercent, Hour_s, Min_s, Second_s;
        stringstream tempIO_Battery;
        int Hour, Min, Second;
        Second = sysPowerStatus.BatteryLifeTime;
        Min = Second / 60, Second %= 60;
        Hour = Min / 60, Min %= 60;
        tempIO_Battery << (int)sysPowerStatus.BatteryLifePercent << ' ' << Hour << ' ' << Min << ' ' << Second;
        tempIO_Battery >> BPercent >> Hour_s >> Min_s >> Second_s;
        Code += "<div> 电量百分比：" + BPercent + "% </div>\n";
        Code += "<div> 剩余电池时间：" + (string)(sysPowerStatus.BatteryLifeTime == -1 ? "未知" : Hour_s + " 时 " + Min_s + " 分 " + Second_s + " 秒") + " </div>\n";
    }
    else
        Code += "<div> 电池存在：否 </div>\n";
    Code += "<br>\n";

    Code += R"(
    </center>
)";

    AJAXOutput(Code);

    return 0;
}
