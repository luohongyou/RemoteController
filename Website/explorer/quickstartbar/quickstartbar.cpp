// quickstartbar.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/explorer/quickstartbar.exe"

#include <iostream>
#include <Windows.h>
#include <shlwapi.h>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
	SetAppDirectory("\\..\\..\\");

	HTMLFrame HTML;
	HTML.Register(PT_EMPTY, ADDRESS, "quickstartbar", "quickstartbar", "", true);

	std::string Code;

	Code += R"(
<table class="table table-hover table-borderless quickstartbar">
	<tr onclick="OpenFolderByHex('') ">
		<td width="4%"> <img src="/explorer/Resources/png/Home.png" width="18 % "/> </td>
		<td>主文件夹</td>
	</tr>
	<tr onclick="OpenFolderByHex('2545352538352542312545342542412541422545342542412539312545372539422539380a') ">
		<td width="4%"> <img src="/explorer/Resources/png/Cloud.png" width="18 % "/> </td>
		<td>共享云盘</td>
	</tr>
)";

	std::string Path; 

	Path = RCAgent("getfolders", HTML.User.GetName(), "Desktop");
	Code += R"(
	<tr onclick="OpenFolderByHex(')" + HexEncode(encodeURI(Path.c_str())) + R"(') ">
		<td width="4%"> <img src="/explorer/Resources/png/Desktop.png" width="18 % "/> </td>
		<td>桌面</td>
	</tr>
)";
	Path = RCAgent("getfolders", HTML.User.GetName(), "Downloads");
	Code += R"(
	<tr onclick="OpenFolderByHex(')" + HexEncode(encodeURI(Path.c_str())) + R"(') ">
		<td width="4%"> <img src="/explorer/Resources/png/Downloads.png" width="18 % "/> </td>
		<td>下载</td>
	</tr>
)";
	Path = RCAgent("getfolders", HTML.User.GetName(), "Documents");
	Code += R"(
	<tr onclick="OpenFolderByHex(')" + HexEncode(encodeURI(Path.c_str())) + R"(') ">
		<td width="4%"> <img src="/explorer/Resources/png/Documents.png" width="18 % "/> </td>
		<td>文档</td>
	</tr>
)";
	Path = RCAgent("getfolders", HTML.User.GetName(), "Pictures");
	Code += R"(
	<tr onclick="OpenFolderByHex(')" + HexEncode(encodeURI(Path.c_str())) + R"(') ">
		<td width="4%"> <img src="/explorer/Resources/png/Pictures.png" width="18 % "/> </td>
		<td>图片</td>
	</tr>
)";
	Path = RCAgent("getfolders", HTML.User.GetName(), "Music");
	Code += R"(
	<tr onclick="OpenFolderByHex(')" + HexEncode(encodeURI(Path.c_str())) + R"(') ">
		<td width="4%"> <img src="/explorer/Resources/png/Music1.png" width="18 % "/> </td>
		<td>音乐</td>
	</tr>
)";
	Path = RCAgent("getfolders", HTML.User.GetName(), "Videos");
	Code += R"(
	<tr onclick="OpenFolderByHex(')" + HexEncode(encodeURI(Path.c_str())) + R"(') ">
		<td width="4%"> <img src="/explorer/Resources/png/Videos.png" width="18 % "/> </td>
		<td>视频</td>
	</tr>
)";


	char _str[MAX_PATH], SystemC;
	GetSystemDirectoryA(_str, MAX_PATH);
	SystemC = _str[0];
	int DiskCount = 0;
	DWORD DiskInfo = GetLogicalDrives();
	while (DiskInfo)
	{
		if (DiskInfo & 1)
			++DiskCount;
		DiskInfo = DiskInfo >> 1;
	}
	std::string Count = Num2String(DiskCount);
	int DSLength = GetLogicalDriveStringsA(0, NULL);
	char* DStr = new char[DSLength];
	GetLogicalDriveStringsA(DSLength, DStr);
	int DType;
	int si = 0;
	for (int i = 0; i < DSLength / 4; ++i)
	{
		std::string DiskAddr, DiskType, Icon;
		std::stringstream _tempIO;
		_tempIO << DStr[si];
		_tempIO >> DiskAddr;
		DiskAddr += ":";
		DType = GetDriveTypeA(DStr + i * 4);
		if (DType == DRIVE_FIXED)
			DiskType = "本地磁盘", Icon = "Disk.png";
		else if (DType == DRIVE_CDROM)
			DiskType = "光盘驱动器", Icon = "Disk_DVD.png";
		else if (DType == DRIVE_REMOVABLE)
			DiskType = "U 盘", Icon = "Disk.png";
		else if (DType == DRIVE_REMOTE)
			DiskType = "网络位置", Icon = "Disk_Network.png";
		else if (DType == DRIVE_RAMDISK)
			DiskType = "虚拟磁盘", Icon = "Disk.png";
		else if (DType == DRIVE_UNKNOWN)
			DiskType = "未知", Icon = "Disk.png";

		if (DiskAddr[0] == SystemC)
			Icon = "Disk_SYSTEM.png";

		LPSTR NameBuf = new char[100];
		DWORD namesize = 100;
		DWORD serialnumber;
		DWORD maxlen;
		DWORD fileflag;
		LPSTR sysnamebuf = new char[100];
		DWORD sysnamesize = 100;
		memset(NameBuf, 0, 100);
		::GetVolumeInformationA(DStr + i * 4, NameBuf, namesize, &serialnumber, &maxlen, &fileflag, sysnamebuf, sysnamesize);
		if (!strcmp(NameBuf, ""))
			NameBuf = (LPSTR)DiskType.c_str();

		Code += R"(
	<tr onclick="OpenFolderByHex(')" + HexEncode(encodeURI(DiskAddr.c_str())) + R"(') ">
		<td width="4%"> <img src="/explorer/Resources/png/)" + Icon + R"(" width="18 % "/> </td>
		<td>)" + (std::string)NameBuf + " (" + DiskAddr + ")" + R"(</td>
	</tr>
)";

		si += 4;
	}

	Code += "</table>";

	AJAXOutput(Code);

	return 0;
}
