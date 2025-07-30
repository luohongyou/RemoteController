// filelist.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/explorer/filelist.exe"

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
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
CompactMode = 0,        // 1: 紧凑
IsFixed = 0;            // 1: 锁定布局

static bool cmp(File &A, File &B)
{
	if (A.Type != B.Type)
		if (!SortBy || (SortBy && !SortDirection))
			return A.Type < B.Type;
		else
			return A.Type > B.Type;
	if (!SortBy)
	{
		for (size_t i = 0; i < A.Name.length() && i < B.Name.length(); ++i)
		{
			unsigned char ca = std::tolower(A.Name[i]), cb = std::tolower(B.Name[i]);
			if (isdigit(ca) && isdigit(cb))
			{
				std::string strA = A.Name.substr(i), strB = B.Name.substr(i);

				int flagA, flagB;
				for (flagA = 0; isdigit(strA[flagA]); flagA++);
				for (flagB = 0; isdigit(strB[flagB]); flagB++);
				strA = strA.substr(0, flagA);
				strB = strB.substr(0, flagB);

				if (strA.size() > 19 || strB.size() > 19)
				{
					if (strA.size() != strB.size())
						return (strA.size() > strB.size()) == SortDirection;
					else
						return (strA > strB) == SortDirection;
				}
				else
				{
					unsigned long long ia = std::stoull(strA), ib = std::stoull(strB);
					if (ia != ib)
						return (ia > ib) == SortDirection;
				}
			}
			else if (ca < cb)
				return !SortDirection;
			else if (ca > cb)
				return SortDirection;
		}
		return (A.Name.length() > B.Name.length()) == SortDirection;
		/*
		return (std::lexicographical_compare(
			A.Name.begin(), A.Name.end(),
			B.Name.begin(), B.Name.end(),
			[](unsigned char l, unsigned char r) {
				return std::tolower(l) > std::tolower(r);
			})) == SortDirection;*/
	}
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

	if (argc != 7)
		return 0;

	ShowHideFiles = (!strcmp(argv[1], "1")),    // 1: 显示
	SortBy = (!strcmp(argv[2], "1")),           // 1: 时间
	SortDirection = (!strcmp(argv[3], "1"));    // 1: 降序
	CompactMode = (!strcmp(argv[4], "1"));      // 1: 紧凑视图
	IsFixed = (!strcmp(argv[5], "1"));          // 1: 锁定布局

	std::string Address = UTF8ToANSI(decodeURI(HexDecode(argv[6])));

	HTMLFrame HTML;
	HTML.Register(PT_EMPTY, ADDRESS, "filelist", "filelist", "", true);

	if (!ExpCheckAddress(Address))
	{
		HTML.Log("用户尝试访问非法路径", "explorer", LL_ERROR);
		AJAXOutput("<!-- ListFileError -->");
		return 0;
	}

	std::string Code;

	bool IsDiskList = 0, IsCloudStorage = 0;
	if (Address[Address.size() - 1] == '\n')
		Address = Address.substr(0, Address.size() - 1);

	if (Address == "empty")
		IsDiskList = 1;
	if (Address.substr(0, 8) == "共享云盘")
		IsCloudStorage = 1;

	if (HTML.User.IsCloudStorageUser() && !IsCloudStorage)
	{
		HTML.Log("用户没有权限访问远程计算机本地路径", "explorer", LL_ERROR);
		AJAXOutput("<!-- ListFileError -->");
		return 0;
	}

	using namespace std;
	if (IsDiskList)   // 列出磁盘
	{
		Code += (std::string)R"(
<table class="table )" + (!CompactMode ? "" : "table-sm ") + R"(table-borderless table-hover user-select-none" id="FileTable">
<tr>
<th></th>
<th>名称</th>
<th>类型</th>
<th>大小</th>
)" + (CompactMode ? "" : "<th></th>")
+ R"(
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
		int cnt = 0;
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

			cnt++;
			Code += (string)"<tr id=\'File_" + Num2String(cnt) 
				+ "\' class=\"folder_item\""
				+ (CompactMode ? " ondblclick=" : " onclick=") + "\"OpenFolder(\'" + "File_" + Num2String(cnt) + "\')\" " 
				+ "name=\"" + HexEncode(encodeURI(DiskAddr.c_str())) + "\"" + ">\n";
			Code += R"(
<td width="4%"> <img src="/explorer/Resources/png/)" + Icon + R"(" width="25 % "/> </td>
)";
			Code += "<td>" + (string)NameBuf + " (" + DiskAddr + ")" + "</td>\n";
			Code += "<td class=\"explorergray\">" + (string)DiskType + "</td>\n";
			Code += "<td class=\"explorergray\">" + (string)DiskSize + "</td>\n";
			if (!CompactMode)
				Code += R"(<td> <a href="javascript:void(0);" id="File_)" + Num2String(cnt) + R"(_d" onclick="ContextMenu('File_)" + Num2String(cnt) + R"(', 1) "><i class="bi bi-three-dots"></i></a></td>
)";
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

	if (!IsCloudStorage)
		strcpy_s(szFind, Address.c_str());
	else
	{
		string realAddress = Address;
		realAddress.replace(0, 8, "storage\\sharedzone");
		strcpy_s(szFind, realAddress.c_str());
		// HTML.Log("FileList Debug: " + realAddress, "explorer", LL_INFO);
	}
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
	Code += (std::string)R"(
<table)" + (IsFixed ? R"( style="table-layout:fixed;")" : "") + R"( class="table )" + (!CompactMode ? "" : "table-sm ") + R"(table-borderless table-hover user-select-none" id="FileTable">
<tr>
<th)" + (IsFixed ? R"( style="width:40px")" : "") + R"(></th>
<th)" + (IsFixed ? (CompactMode ? R"( style="width:50%")" : R"( style="width:40%")") : "") + R"( onclick="ChangeSort('0') ">名称)" + (SortBy == 0 ? (std::string)"&nbsp;<i class=\"bi bi-" + (!SortDirection ? "sort-alpha-up" : "sort-alpha-down-alt") + " text-primary\"></i>" : "") + R"(</th>
<th)" + (IsFixed ? R"( style="width:20%")" : "") + R"( onclick="ChangeSort('1') ">修改日期)" + (SortBy == 1 ? (std::string)"&nbsp;<i class=\"bi bi-" + (!SortDirection ? "sort-numeric-up" : "sort-numeric-down-alt") + " text-primary\"></i>" : "") + R"(</th>
<th)" + (IsFixed ? R"( style="width:15%")" : "") + R"(>类型</th>
<th)" + (IsFixed ? R"( style="width:10%")" : "") + R"(>大小</th>
<th></th>
</tr>
)";

	sort(FileList.begin(), FileList.end(), cmp);

	LoadFileIconInfo();
	string ExtName, Icon;

	int cnt = 0;
	for (auto &item : FileList)
	{
		cnt++;

		Code += (std::string)"<tr class=\"" + (item.Type ? "file_item" : "folder_item")
			+ "\" id=\'File_" + Num2String(cnt)
			+ (CompactMode ? "\' ondblclick=\"" : "\' onclick=\"")
			+ (item.Type ? "OpenFile" : "OpenFolder") + "(\'" + "File_" + Num2String(cnt) +"\')\" "
		    + "name=\"" + HexEncode(encodeURI(item.Address.c_str())) + "\" "
			+ (item.IsInvisible ? "style=\"opacity:0.5\"" : "") + ">\n";

		if (item.Type)
		{
			GetFileInfo(item.Name, ExtName, Icon);
			Code += R"(
<td width="4%"> <img id="File_)" + Num2String(cnt) + R"(_i" src="/explorer/Resources/png/)" + Icon + R"(" width="25 % "/> </td>
)";
		}
		else
			Code += R"(
<td width="4%"> <img id="File_)" + Num2String(cnt) + R"(_i" src="/explorer/Resources/png/Folder.png" width="25 % "/> </td>
)";
		Code += "<td" + (std::string)(IsFixed ? R"( class="tb-ell")" : "") + ">" + item.Name + "</td>\n";
		Code += "<td class=\"explorergray" + (std::string)(IsFixed ? " tb-ell" : "") + "\">" + item.Date + " </td>\n";
		Code += "<td class=\"explorergray" + (std::string)(IsFixed ? " tb-ell" : "") + "\">" + (item.Type ? ExtName : "文件夹") + "</td>\n";
		Code += "<td class=\"explorergray" + (std::string)(IsFixed ? " tb-ell" : "") + "\">" + (item.Type ? item.Size : "") + "</td>\n";


		if (!CompactMode)
		{
			if (item.Type)
				Code += R"(<td>	<a href="javascript:void(0);" onclick="Download()" + Num2String(cnt) + R"(, 0) "><i class="bi bi-download"></i></a>&nbsp;&nbsp;
)";
			else
				Code += R"(<td>	<a href="javascript:void(0);" onclick="DownloadFolder()" + Num2String(cnt) + R"(, 1) "><i class="bi bi-download"></i></a>&nbsp;&nbsp;
)";
			Code += R"(<a href="javascript:void(0);" id="File_)" + Num2String(cnt) + R"(_d" onclick="ContextMenu('File_)" + Num2String(cnt) + R"(', )" + (item.Type ? "0" : "1") + R"() "><i class="bi bi-three-dots"></i></a></td>
)";
		}
		else
			Code += "<td></td>\n";

		Code += "</tr>\n";
	}

	Code += "</table>\n";

	if (FileList.empty())
	{
		if (IsCloudStorage)
			Code += R"(
<center>
<img src="/explorer/Resources/open_file_folder_3d.png" width="150px">
<br><br>
<h4>文件夹为空</h4>
<p>上传或转存一个项目以开始</p>
</center>
)";
		else
			Code += R"(
<p align="center">此文件夹为空。</p>
)";
	}


	AJAXOutput(Code);

	return 0;
}