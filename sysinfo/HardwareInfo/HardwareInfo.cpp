// HardwareInfo.cpp : Coral Remote Controller 系统信息 硬件信息模块

// 版权声明：
// Copyright (C) 2020-2021 Coral Studio. All Rights Reserved.

#include <iostream>
#include <Windows.h>
#include <string>
#include <sstream>
#include "HardwareInfo.h"
using namespace std;
int main()
{
    puts("Content-type: text/html\n");
    string Code = R"(
<!DOCTYPE html>
<html lang="zh">
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=gb2312">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>HardwareInfo - Coral Remote Controller</title>
    <!-- Bootstrap -->
    <link href="/css/bootstrap-4.4.1.css" rel="stylesheet">
  </head>
  <body>
    <br>
    <center>
)";

    Code += "<h4> CPU 信息 </h4>\n";
    Code += "<div>" + GetCpuType() + "</div>\n";
    string Cores, Threads, Speed;
    stringstream tempIO_CPU;
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    tempIO_CPU << GetProcessorCoreCount() << ' ' << sysInfo.dwNumberOfProcessors << ' ' << GetCPUFreq();
    tempIO_CPU >> Cores >> Threads >> Speed;
    Code += "<div> " + Cores + " 核心，" + Threads + " 线程 </div>\n";
    Code += "<div> 速度：" + Speed + " GHz </div>\n";
    Code += "<br>\n";
    
    Code += "<h4> 内存信息 </h4>\n";
    const double DIV = 1024 * 1024 * 1024;
    stringstream tempIO_Memory;
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);
    tempIO_Memory << statex.dwMemoryLoad << endl;
    tempIO_Memory << statex.ullTotalPhys / DIV << ' ' << statex.ullTotalPageFile / DIV << ' ' << statex.ullTotalVirtual / DIV << endl;
    tempIO_Memory << statex.ullAvailPhys / DIV << ' ' << statex.ullAvailPageFile / DIV << ' ' << statex.ullAvailVirtual / DIV;
    string MLoad, TotalPhys, TotalPageFile, TotalVirtual, AvailPhys, AvailPageFile, AvailVirtual;
    tempIO_Memory >> MLoad >> TotalPhys >> TotalPageFile >> TotalVirtual >> AvailPhys >> AvailPageFile >> AvailVirtual;
    Code += "<div> 已使用：" + MLoad + "% </div>\n";
    Code += "<div> 物理内存：" + TotalPhys + " GB，" + AvailPhys + " GB 可用 </div>\n";
    Code += "<div> 页面文件：" + TotalPageFile + " GB，" + AvailPageFile + " GB 可用 </div>\n";
    Code += "<div> 虚拟内存：" + TotalVirtual + " GB，" + AvailVirtual + " GB 可用 </div>\n";
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
    <!-- jQuery (necessary for Bootstrap's JavaScript plugins) --> 
    <script src="/js/jquery-3.4.1.min.js"></script> 
    <!-- Include all compiled plugins (below), or include individual files as needed --> 
    <script src="/js/popper.min.js"></script> 
    <script src="/js/bootstrap-4.4.1.js"></script>
  </body>
</html>
)";
    puts(Code.c_str());
    return 0;
}
