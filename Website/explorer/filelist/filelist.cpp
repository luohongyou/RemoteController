// filelist.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2024 Coral Studio. All Rights Reserved.

#define ADDRESS "/explorer/filelist.exe"

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
// #include <filesystem>
#include <Windows.h>
#include <shlwapi.h>
#include "FileIcon.h"
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

class File
{
public:
	std::string Name, Size, Date, Address;
	time_t time;
	bool Type; // 0->Folder; 1->File
	bool IsInvisible;
};

vector<File> FileList;

bool ShowHideFiles = 0, // 1: 显示
SortBy = 0,             // 1: 时间
SortDirection = 0,      // 1: 降序
CompactMode = 0;        // 1: 紧凑

static bool cmp(File &A, File &B)
{
	if (A.Type != B.Type)
		if (!SortBy || (SortBy && !SortDirection))
			return A.Type < B.Type;
		else
			return A.Type > B.Type;
	if (!SortBy)
		return (std::lexicographical_compare(
			A.Name.begin(), A.Name.end(),
			B.Name.begin(), B.Name.end(),
			[](unsigned char l, unsigned char r) {
				return std::tolower(l) > std::tolower(r);
			})) == SortDirection;
	else
		if (A.time == B.time)
			return std::lexicographical_compare(
				A.Name.begin(), A.Name.end(),
				B.Name.begin(), B.Name.end(),
				[](unsigned char l, unsigned char r) {
					return std::tolower(l) < std::tolower(r);
				});
		else
			return (A.time > B.time) == SortDirection;
}

int main(int argc, char* argv[])
{
	SetAppDirectory("\\..\\..\\");

	if (argc != 6)
		return 0;

	ShowHideFiles = (!strcmp(argv[1], "1")),    // 1: 显示
	SortBy = (!strcmp(argv[2], "1")),           // 1: 时间
	SortDirection = (!strcmp(argv[3], "1"));    // 1: 降序
	CompactMode = (!strcmp(argv[4], "1"));      // 1: 紧凑视图
	std::string Address = UTF8ToANSI(decodeURI(HexDecode(argv[5])));

	HTMLFrame HTML;
	HTML.Register(PT_EMPTY, ADDRESS, "filelist", "filelist", "", true);

	std::string Code;

	bool IsDiskList = 0;
	if (Address[Address.size() - 1] == '\n')
		Address = Address.substr(0, Address.size() - 1);

	if (Address == "empty")
		IsDiskList = 1;

	using namespace std;
	if (IsDiskList)   // 列出磁盘
	{
		Code += (std::string)R"(
<table class="table )" + (!CompactMode ? "" : "table-sm table-borderless") + R"( table-hover">
<tr>
<th></th>
<th>名称</th>
<th>类型</th>
<th>大小</th>
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
		string Count = Num2String(DiskCount);
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
			string DiskAddr, DiskType, DiskSize, Icon;
			stringstream _tempIO;
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

			fResult = GetDiskFreeSpaceExA(
				(DStr[si] + (string)":\\").c_str(),
				(PULARGE_INTEGER)&i64FreeBytesToCaller,
				(PULARGE_INTEGER)&i64TotalBytes,
				(PULARGE_INTEGER)&i64FreeBytes);
			if (fResult)
			{
				string tempT, tempF;
				stringstream tempIO;
				tempIO << std::fixed << std::setprecision(2) << (float)i64TotalBytes / 1024 / 1024 / 1024 << ' ' << (float)i64FreeBytesToCaller / 1024 / 1024 / 1024;
				tempIO >> tempT >> tempF;
				int Percent = ((((float)i64TotalBytes / 1024 / 1024 / 1024) - ((float)i64FreeBytesToCaller / 1024 / 1024 / 1024)) / ((float)i64TotalBytes / 1024 / 1024 / 1024)) * 100;
				DiskSize = tempT + "GB，" + tempF + "GB 可用";
				if (Percent < 90)
				DiskSize += R"(
<div class="progress" role="progressbar" style="height:5px">
  <div class="progress-bar" style="width: )" + Num2String(Percent) + R"(%"></div>
</div>)";
				else
					DiskSize += R"(
<div class="progress" role="progressbar" style="height:5px">
  <div class="progress-bar bg-danger" style="width: )" + Num2String(Percent) + R"(%"></div>
</div>)";
			}
			else
				DiskSize = "无法获取";

			Code += "<tr onclick=\"OpenFolder(\'" + HexEncode(encodeURI(DiskAddr.c_str())) + "\')\">\n";
			Code += R"(
<td width="4%"> <img src="/explorer/Resources/png/)" + Icon + R"(" width="25 % "/> </td>
)";
			Code += "<td>" + (string)NameBuf + " (" + DiskAddr + ")" + "</td>\n";
			Code += "<td class=\"explorergray\">" + (string)DiskType + "</td>\n";
			Code += "<td class=\"explorergray\">" + (string)DiskSize + "</td>\n";
			Code += "</tr>\n";

			si += 4;
		}

		Code += "</table>\n";
		AJAXOutput(Code);
		return 0;
	}

	// 列出文件
	if (Address[Address.size() - 1] != '\\')
		Address += '\\';

	char szFind[MAX_PATH];
	WIN32_FIND_DATAA FindFileData;

	strcpy_s(szFind, Address.c_str());
	strcat_s(szFind, "*.*");

	// 添加文件到vector
	HANDLE hFind = ::FindFirstFileA(szFind, &FindFileData);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		AJAXOutput("<!-- ListFileError -->");
		HTML.Log("尝试列出文件夹时出现错误<br>请求地址：" + Address, "explorer", LL_WARNING);
		return 0;
	}

	File file;
	while (true)
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (strcmp(FindFileData.cFileName, ".") && strcmp(FindFileData.cFileName, ".."))
			{
				char szFile[MAX_PATH];

				if (!((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) && !ShowHideFiles))
				{
					file.Name = FindFileData.cFileName;

					time_t t = NULL;
					LONGLONG ll;
					ULARGE_INTEGER ui;
					ui.LowPart = FindFileData.ftLastWriteTime.dwLowDateTime;
					ui.HighPart = FindFileData.ftLastWriteTime.dwHighDateTime;
					ll = FindFileData.ftLastWriteTime.dwHighDateTime << 32 + FindFileData.ftLastWriteTime.dwLowDateTime;
					t = ((LONGLONG)(ui.QuadPart - 116444736000000000) / 10000000);
					file.Date = StandardTimeWithoutSec(false, t);
					file.time = t;

					file.Type = 0;
					file.Size = "";
					file.IsInvisible = (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN);

					file.Address = Address + FindFileData.cFileName;

					FileList.push_back(file);
				}
			}
		}
		else
		{
			if (!((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) && !ShowHideFiles))
			{

				file.Name = FindFileData.cFileName;

				time_t t = NULL;
				LONGLONG ll;
				ULARGE_INTEGER ui;
				ui.LowPart = FindFileData.ftLastWriteTime.dwLowDateTime;
				ui.HighPart = FindFileData.ftLastWriteTime.dwHighDateTime;
				ll = FindFileData.ftLastWriteTime.dwHighDateTime << 32 + FindFileData.ftLastWriteTime.dwLowDateTime;
				t = ((LONGLONG)(ui.QuadPart - 116444736000000000) / 10000000);
				file.Date = StandardTimeWithoutSec(false, t);
				file.time = t;

				file.Type = 1;

				__int64 fileSize = (static_cast<__int64>(FindFileData.nFileSizeHigh) << 32) | FindFileData.nFileSizeLow;
				double GB = static_cast<double>(fileSize) / 1073741824.0;
				double MB = static_cast<double>(fileSize) / 1048576.0;
				double KB = static_cast<double>(fileSize) / 1024.0;
				double size;
				std::string unit;

				if (GB >= 1)
					size = GB, unit = "GB";
				else if (MB >= 1)
					size = MB, unit = "MB";
				else if (KB >= 1)
					size = KB, unit = "KB";
				else if (KB != 0)
					size = 1, unit = "KB";
				else
					size = 0, unit = "KB";

				stringstream tempIO;
				tempIO << std::fixed << std::setprecision(unit == "GB" ? 2 : 0) << size;
				tempIO >> file.Size;
				file.Size += " " + unit;

				file.IsInvisible = (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN);
				
				file.Address = Address + FindFileData.cFileName;

				FileList.push_back(file);
			}
		}
		if (!FindNextFileA(hFind, &FindFileData))
			break;
	}
	FindClose(hFind);

	// 生成代码
	Code += (std::string) + R"(
<table class="table )" + (!CompactMode ? "" : "table-sm table-borderless") + R"( table-hover">
<tr>
<th></th>
<th>名称</th>
<th>修改日期</th>
<th>类型</th>
<th>大小</th>
<th>操作</th>
</tr>
)";

	sort(FileList.begin(), FileList.end(), cmp);

	LoadFileIconInfo();
	string ExtName, Icon;

	for (auto &item : FileList)
	{
		if (item.IsInvisible)
			Code += "<tr " + (item.Type ? "" : "onclick=\"OpenFolder(\'" + HexEncode(encodeURI(item.Address.c_str())) + "\')\"") + " style=\"opacity:0.5\">\n";
		else
			Code += "<tr " + (item.Type ? "" : "onclick=\"OpenFolder(\'" + HexEncode(encodeURI(item.Address.c_str())) + "\')\"") + " >\n";

		if (item.Type)
		{
			GetFileInfo(item.Name, ExtName, Icon);
			Code += R"(
<td width="4%"> <img src="/explorer/Resources/png/)" + Icon + R"(" width="25 % "/> </td>
)";
		}
		else
			Code += R"(
<td width="4%"> <img src="/explorer/Resources/png/Folder.png" width="25 % "/> </td>
)";
		Code += "<td>" + item.Name + " </td>\n";
		Code += "<td class=\"explorergray\">" + item.Date + " </td>\n";

		if (item.Type)
			Code += "<td class=\"explorergray\">" + ExtName + "</td>\n";
		else
			Code += "<td class=\"explorergray\">文件夹</td>\n";
		if (item.Type)
			Code += "<td class=\"explorergray\">" + item.Size + "</td>\n";
		else
			Code += "<td></td>\n";

		if (item.Type)
			Code += R"(<td>	<a href="javascript:void(0);" onclick="Download(')" + HexEncode(encodeURI(item.Address.c_str())) + R"(', ')" + HexEncode(encodeURI(item.Name.c_str())) + R"(', ')" + HexEncode(encodeURI(Icon.c_str())) + R"(', 0) "><i class="bi bi-download"></i></a></td>
)";
		else
			// Code += "<td></td>\n";
			Code += R"(<td>	<a href="javascript:void(0);" onclick="DownloadFolder(')" + HexEncode(encodeURI(item.Address.c_str())) + R"(', ')" + HexEncode(encodeURI(item.Name.c_str())) + R"(', ')" + HexEncode(encodeURI("Folder.png")) + R"(', 1) "><i class="bi bi-download"></i></a></td>
)";

		Code += "</tr>\n";
	}

	Code += "</table>\n";


	AJAXOutput(Code);

	return 0;
}