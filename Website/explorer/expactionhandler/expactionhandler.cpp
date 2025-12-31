// expactionhandler.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/explorer/expactionhandler.exe"

#include <iostream>
#include <Windows.h>
#include <filesystem>
#include <io.h>
#include <fcntl.h>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

namespace fs = std::filesystem;
bool CopyDirectory(const fs::path& src_dir, const fs::path& dst_dir);
bool CloudCheckAddress(std::string Address);
std::string getAvailableFolderName(std::string OriginalFolderName, std::string Address)
{
	int cnt = 0;
	std::string FolderNameNew = OriginalFolderName;
	while (std::filesystem::exists(Address + FolderNameNew))
		FolderNameNew = OriginalFolderName + " (" + Num2String(++cnt) + ")";
	return FolderNameNew;
}
std::string getAvailableFileName(std::string OriginalFileName, std::string Address)
{
	int cnt = 0;
	std::string Ext, DisplayName;
	bool flag = 0;
	for (int i = OriginalFileName.size() - 1; i >= 0; i--)
		if (OriginalFileName[i] == '.')
		{
			Ext = "." + OriginalFileName.substr(i + 1);
			DisplayName = OriginalFileName.substr(0, i);
			flag = 1;
			break;
		}
	if (!flag)
	{
		Ext = "";
		DisplayName = OriginalFileName;
	}

	while (std::filesystem::exists(Address + OriginalFileName))
		OriginalFileName = DisplayName + " (" + Num2String(++cnt) + ")" + Ext;

	return OriginalFileName;
}

int main(int argc, char* argv[])
{
	SetAppDirectory("\\..\\..\\");

	HTMLFrame HTML;
	HTML.Register(PT_EMPTY, ADDRESS, "expactionhandler", "expactionhandler", "", true);
	if (HTML.User.GetUserType() == UT_GUEST)
	{
		HTML.Log("用户没有权限操作远程计算机上的文件", "explorer", LL_ERROR);
		return 0;
	}

	if (argc != 4)
		return 0;

	int Action = String2Num(argv[1]);
	std::string Address = UTF8ToANSI(decodeURI(HexDecode(argv[2])));
	if (Address[Address.size() - 1] == '\n')
		Address = Address.substr(0, Address.size() - 1);

	if (!ExpCheckAddress(Address))
	{
		HTML.Log("用户尝试操作非法路径", "explorer", LL_ERROR);
		return 0;
	}
	if (!CloudCheckAddress(Address) && HTML.User.IsCloudStorageUser())
	{
		HTML.Log("用户没有权限操作远程计算机的本地文件", "explorer", LL_ERROR);
		printf("No");
		return 0;
	}

	int IsCloudStorage = (Address.substr(0, 8) == "共享云盘");
	if (!IsCloudStorage && !fs::exists(Address) && Action != 4)
	{
		HTML.Log("用户尝试访问不存在的路径", "explorer", LL_ERROR);
		puts("Content-type: text/html\n");
		printf("No");
		return 0;
	}

	puts("Content-type: text/html\n");
	switch (Action)
	{
	case 1: // 在远程计算机上打开 1+addr+0
		if (HTML.User.IsCloudStorageUser())
		{
			HTML.Log("用户没有权限打开远程计算机上的文件", "explorer", LL_ERROR);
			printf("No");
			return 0;
		}

		RCAgent("remoteopen", HTML.User.GetName(), argv[2]);

		HTML.Log("已请求在远程计算机上打开文件<br>打开的文件/文件夹：" + Address, "explorer", LL_INFO);
		printf("Yes");

		break;
	case 2: // 转存到云盘 2+FileAddr+CloudAddr ->"共享云盘\\..."
	{
		if (HTML.User.IsCloudStorageUser())
		{
			HTML.Log("用户没有权限操转存远程计算机的本地文件至云盘", "explorer", LL_ERROR);
			printf("No");
			return 0;
		}

		std::ofstream fout;
		std::string cloudAddress = UTF8ToANSI(decodeURI(HexDecode(argv[3])));
		if (!ExpCheckAddress(cloudAddress))
			return 0;
		if (!CloudCheckAddress(cloudAddress))
			return 0;
		if (cloudAddress[cloudAddress.size() - 1] != '\\')
			cloudAddress += "\\";
		cloudAddress = cloudAddress.substr(cloudAddress.find_first_of('\\'));
		if (fs::is_directory(Address))
		{
			std::string FolderName = fs::path(Address).filename().string();
			std::string FolderNameNew = getAvailableFolderName(FolderName, "storage\\sharedzone" + cloudAddress);

			int ret = CopyDirectory(Address, "storage\\sharedzone" + cloudAddress + FolderNameNew);
			if (!ret)
			{
				HTML.Log("转存文件夹到共享云盘失败<br>尝试转存的项目" + Address + "<br>目标位置：共享云盘" + cloudAddress + FolderNameNew, "explorer", LL_ERROR);
				printf("No");
				return 0;
			}

			HTML.Log("成功转存文件夹到共享云盘<br>转存的项目" + Address + "<br>目标位置：共享云盘" + cloudAddress + FolderNameNew, "explorer", LL_INFO);
			printf("Yes");
		}
		else
		{
			std::string FileName = fs::path(Address).filename().string();
			FileName = getAvailableFileName(FileName, "storage\\sharedzone" + cloudAddress);

			int ret = CopyFileA(Address.c_str(), ("storage\\sharedzone" + cloudAddress + FileName).c_str(), FALSE);
			if (!ret)
			{
				HTML.Log("转存文件到共享云盘失败<br>尝试转存的项目" + Address + "<br>目标位置：共享云盘" + cloudAddress + FileName, "explorer", LL_ERROR);
				printf("No");
				return 0;
			}

			HTML.Log("成功转存文件到共享云盘<br>转存的项目" + Address + "<br>目标位置：共享云盘" + cloudAddress + FileName, "explorer", LL_INFO);
			printf("Yes");
		}
	}
	break;
	case 3: // 删除 3+addr+0
		if (!IsCloudStorage && HTML.User.IsCloudStorageUser())
		{
			HTML.Log("用户没有权限删除远程计算机上的本地文件", "explorer", LL_ERROR);
			printf("No");
			return 0;
		}

		if (IsCloudStorage)
			Address = "storage\\sharedzone" + Address.substr(Address.find_first_of('\\'));

		if (fs::is_directory(Address))
		{
			int ans = RemoveDir(Address.c_str());
			if (ans && !fs::exists(Address))
			{
				HTML.Log("已成功删除文件夹：" + Address, "explorer", LL_INFO);
				printf("Yes");
			}
			else
			{
				HTML.Log("删除文件夹 " + Address + " 失败", "explorer", LL_ERROR);
				printf("No");
			}
		}
		else
		{
			int ans = remove(Address.c_str());
			if (ans == 0)
			{
				HTML.Log("已成功删除文件：" + Address, "explorer", LL_INFO);
				printf("Yes");
			}
			else
			{
				HTML.Log("删除文件 " + Address + " 失败", "explorer", LL_ERROR);
				printf("No");
			}
		}
		break;
	case 4: // 新建文件夹 4+addr+0
		if (IsCloudStorage)
			Address = "storage\\sharedzone" + Address.substr(Address.find_first_of('\\'));
		if (fs::exists(Address))
		{
			HTML.Log("用户尝试重复创建同名文件夹", "explorer", LL_ERROR);
			printf("No");
			break;
		}
		if (CreateDirectoryA(Address.c_str(), NULL) == 0)
		{
			HTML.Log("请求新建文件夹失败：" + Address, "explorer", LL_ERROR);
			printf("No");
		}
		else
		{
			HTML.Log("已请求新建文件夹：" + Address, "explorer", LL_INFO);
			printf("Yes");
		}
		break;
	case 5: // 重命名 5+addr+newname
	{
		if (IsCloudStorage)
			Address = "storage\\sharedzone" + Address.substr(Address.find_first_of('\\'));
		std::string NewName = UTF8ToANSI(decodeURI(HexDecode(argv[3]))), NewAddress;
		if (NewName[NewName.size() - 1] == '\n')
			NewName = NewName.substr(0, NewName.size() - 1);
		NewAddress = Address.substr(0, Address.find_last_of('\\') + 1) + NewName;
		if (rename(Address.c_str(), NewAddress.c_str()) == 0)
		{
			HTML.Log("已请求重命名文件<br>原名称：" + Address + "<br>新名称：" + NewAddress, "explorer", LL_INFO);
			printf("Yes");
		}
		else
		{
			HTML.Log("请求重命名文件失败<br>原名称：" + Address + "<br>新名称：" + NewAddress, "explorer", LL_ERROR);
			printf("No");
		}
	}
	break;
	case 6: // 粘贴(复制) 6+dest+source
	case 7: // 粘贴(剪切) 7+dest+source
	{
		std::string source = UTF8ToANSI(decodeURI(HexDecode(argv[3])));

		if ((!ExpCheckAddress(source) || !CloudCheckAddress(source)) && HTML.User.IsCloudStorageUser())
		{
			HTML.Log("用户没有权限操作远程计算机的本地文件", "explorer", LL_ERROR);
			printf("No");
			return 0;
		}

		if (Address == "共享云盘")
			Address = "storage\\sharedzone";
		else if (IsCloudStorage)
			Address = "storage\\sharedzone" + Address.substr(Address.find_first_of('\\'));
		if (Address[Address.size() - 1] != '\\')
			Address += "\\";

		if (source == "共享云盘")
			source = "storage\\sharedzone";
		else if (source.substr(0, 8) == "共享云盘")
			source = "storage\\sharedzone" + source.substr(source.find_first_of('\\'));

		std::string FileName = source.substr(source.find_last_of('\\') + 1);
		if (fs::is_directory(source))
			FileName = getAvailableFolderName(FileName, Address);
		else
			FileName = getAvailableFileName(FileName, Address);

		int ans;
		if (fs::is_directory(source))
		{
			CreateDirectoryA((Address + FileName).c_str(), NULL);
			ans = CopyDirectory(source, Address + FileName);
		}
		else
			ans = CopyFileA(source.c_str(), (Address + FileName).c_str(), FALSE);

		if (!ans)
		{
			HTML.Log("请求复制文件失败：无法复制文件<br>源位置：" + source + "<br>目标位置：" + Address + FileName, "explorer", LL_ERROR);
			printf("No");
			return 0;
		}


		if (Action == 7)
		{
			if (fs::is_directory(source))
				ans = RemoveDir(source.c_str());
			else
				ans = (remove(source.c_str()) == 0);

			if (ans && !fs::exists(source))
			{
				HTML.Log("已请求移动文件<br>源位置：" + source + "<br>目标位置：" + Address + FileName, "explorer", LL_INFO);
				printf("Yes");
			}
			else
			{
				HTML.Log("请求移动文件失败：无法删除原文件<br>源位置：" + source + "<br>目标位置：" + Address + FileName, "explorer", LL_ERROR);
				printf("No");
			}
		}
		else
		{
			HTML.Log("已请求复制文件<br>源位置：" + source + "<br>目标位置：" + Address + FileName, "explorer", LL_INFO);
			printf("Yes");
		}
	}

	}

	return 0;
}
bool CopyDirectory(const fs::path& src_dir, const fs::path& dst_dir)
{
	if (!fs::exists(dst_dir)) {
		fs::create_directory(dst_dir);
	}

	for (const auto& entry : fs::recursive_directory_iterator(src_dir)) {
		const auto& path = entry.path();
		auto relative_path = path.lexically_relative(src_dir);
		fs::path dest_path = dst_dir / relative_path;

		if (fs::is_directory(path)) {
			fs::create_directories(dest_path);
		}
		else if (fs::is_regular_file(path)) {
			if (copy_file(path, dest_path, fs::copy_options::update_existing) != true)
				return false;
		}
		else {
			return false;
		}
	}

	return true;
}
bool CloudCheckAddress(std::string Address)
{
	if (Address.substr(0, 8) != "共享云盘")
		return false;
	return true;
}