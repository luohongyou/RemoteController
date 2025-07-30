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
		if (cloudAddress[cloudAddress.size() - 1] != '\\')
			cloudAddress += "\\";
		cloudAddress = cloudAddress.substr(cloudAddress.find_first_of('\\'));
		if (fs::is_directory(Address))
		{
			std::string FolderName = fs::path(Address).filename().string();
			std::string FolderNameNew = FolderName;

			int cnt = 0;
			while (std::filesystem::exists("storage\\sharedzone" + cloudAddress + FolderNameNew))
				FolderNameNew = FolderName + " (" + Num2String(++cnt) + ")";

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
			int cnt = 0;
			std::string Ext, DisplayName;
			std::string FileName = fs::path(Address).filename().string();
			bool flag = 0;
			for (int i = FileName.size() - 1; i >= 0; i--)
				if (FileName[i] == '.')
				{
					Ext = "." + FileName.substr(i + 1);
					DisplayName = FileName.substr(0, i);
					flag = 1;
					break;
				}
			if (!flag)
			{
				Ext = "";
				DisplayName = FileName;
			}

			while (std::filesystem::exists("storage\\sharedzone" + cloudAddress + FileName))
				FileName = DisplayName + " (" + Num2String(++cnt) + ")" + Ext;

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
			HTML.Log("用户尝试操作不存在的文件夹", "explorer", LL_ERROR);
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
		break;
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
