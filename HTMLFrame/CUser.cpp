// CUser.cpp : Coral Remote Controller 用户输出类和函数的实现。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#include "pch.h"
#include "CUser.h"
#include "HTMLFrame.h"

CUser::CUser()
{
	UserName = PasswordMD5 = GUID = "", UserType = 100;
}

void CUser::Assign(std::string _GUID)
{
	std::ifstream fin;
	int tot;
	std::string _uname, _guid;

	if (_GUID == "00000000-0000-0000-0000-000000000000" || _GUID == "FFFFFFFF-FFFF-FFFF-FFFF-FFFFFFFFFFFF") // 本地 Webview2 请求
	{
		Assign(_GUID, "");
		return;
	}

	fin.open(DatabasePath() + "User\\list");
	fin >> tot;
	for (int i = 1; i <= tot; i++)
	{
		fin >> _uname >> _guid;
		if (_guid == _GUID)
		{
			Assign(_GUID, _uname);
			return;
		}
	}
}
void CUser::Assign(std::string _GUID, std::string _UserName)
{
	std::ifstream fin;

	if (_GUID == "00000000-0000-0000-0000-000000000000") // 本地 Webview2 请求
	{
		UserName = "ForegroundUser", PasswordMD5 = "", GUID = "00000000-0000-0000-0000-000000000000", UserType = UT_ROOT;
		return;
	}
	if (_GUID == "FFFFFFFF-FFFF-FFFF-FFFF-FFFFFFFFFFFF") // 云盘用户
	{
		UserName = "CloudStorageUser", PasswordMD5 = "", GUID = "FFFFFFFF-FFFF-FFFF-FFFF-FFFFFFFFFFFF", UserType = GetSettings("CloudStoragePINEditable", false) ? UT_USER : UT_GUEST;
		return;
	}

	GUID = _GUID, UserName = _UserName;
	fin.open(UserFile(_GUID, "info", "md5"));
	if (fin.fail())
	{
		fin.close();
		UserName = PasswordMD5 = GUID = "", UserType = 100;
	}
	fin >> PasswordMD5;
	fin.close();
	fin.open(UserFile(_GUID, "info", "type"));
	fin >> UserType;
	fin.close();
}

std::string CUser::GetName()
{
	return UserName;
}

std::string CUser::GetPasswordMD5()
{
	return PasswordMD5;
}

std::string CUser::GetGUID()
{
	return GUID;
}

int CUser::GetUserType()
{
	return UserType;
}

std::string CUser::Data(std::string Name)
{
	return DatabasePath() + "User\\" + GUID + "\\data\\" + Name;
}

int CUser::IsOOBEed()
{
	std::ifstream fin(UserFile(GUID, "info", "OOBE"));
	if (fin.fail())
		return 0;

	int Version = 0;
	fin >> Version;
	fin.close();

	return Version;
}

int CUser::IsForegroundUser()
{
	return GUID == "00000000-0000-0000-0000-000000000000";
}

int CUser::IsCloudStorageUser()
{
	return GUID == "FFFFFFFF-FFFF-FFFF-FFFF-FFFFFFFFFFFF";
}

std::string NewToken()
{
	std::string Token = "";
	srand(time(0));
	int tempcode;
	for (int i = 0; i < 200; i++)
	{
		tempcode = rand() % 36;
		if (0 <= tempcode && tempcode <= 9)
			Token += (char)(tempcode + '0');
		else
			Token += (char)(tempcode - 10 + 'A');
	}
	return Token;
}
std::string NewHostToken()
{
	std::string Token = "";
	srand(time(0));
	int tempcode;
	for (int i = 0; i < 300; i++)
	{
		tempcode = rand() % 36;
		if (0 <= tempcode && tempcode <= 9)
			Token += (char)(tempcode + '0');
		else
			Token += (char)(tempcode - 10 + 'A');
	}
	return Token;
}
std::string NewCloudToken()
{
	std::string Token = "";
	srand(time(0));
	int tempcode;
	for (int i = 0; i < 100; i++)
	{
		tempcode = rand() % 36;
		if (0 <= tempcode && tempcode <= 9)
			Token += (char)(tempcode + '0');
		else
			Token += (char)(tempcode - 10 + 'A');
	}
	return Token;
}

std::string UserFile(std::string GUID, std::string Type, std::string Name)
{
	return DatabasePath() + "User\\" + GUID + "\\" + Type + "\\" + Name;
}
