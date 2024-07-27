// CUser.cpp : Coral Remote Controller 用户输出类和函数的实现。

// 版权声明：
// Copyright (C) 2020-2024 Coral Studio. All Rights Reserved.

#include "pch.h"
#include "CUser.h"
#include "HTMLFrame.h"

CUser::CUser()
{
	UserName = PasswordMD5 = Token = GUID = "", UserType = 100;
}

void CUser::Assign(std::string _Token)
{
	std::ifstream fin, fin2;
	fin.open(DatabasePath() + "User\\list");

	int tot, flag = 0;
	fin >> tot;

	std::string _token, _uname, _md5, _guid;
	int _utype = 100;
	for (int i = 1; i <= tot; i++)
	{
		fin >> _uname >> _guid;

		fin2.open(UserFile(_guid, "info", "token"));
		fin2 >> _token;
		fin2.close();

		if (_token == _Token)
		{
			fin2.open(UserFile(_guid, "info", "md5"));
			fin2 >> _md5;
			fin2.close();
			fin2.open(UserFile(_guid, "info", "type"));
			fin2 >> _utype;
			fin2.close();

			UserName = _uname, PasswordMD5 = _md5, Token = _token, GUID = _guid, UserType = _utype;
			flag = 1;
			break;
		}
	}

	if (!flag)
		UserName = PasswordMD5 = Token = GUID = "", UserType = 100;

	fin.close();
}

void CUser::Assign(std::string _UserName, std::string _PasswordMD5)
{
	std::ifstream fin, fin2;
	fin.open(DatabasePath() + "User\\list");

	int tot, flag = 0;
	fin >> tot;

	std::string _token, _uname, _md5, _guid;
	int _utype = 100;
	for (int i = 1; i <= tot; i++)
	{
		fin >> _uname >> _guid;

		fin2.open(UserFile(_guid, "info", "md5"));
		fin2 >> _md5;
		fin2.close();

		if (_uname == _UserName && _md5 == _PasswordMD5)
		{
			fin2.open(UserFile(_guid, "info", "token"));
			fin2 >> _token;
			fin2.close();
			fin2.open(UserFile(_guid, "info", "type"));
			fin2 >> _utype;
			fin2.close();

			UserName = _uname, PasswordMD5 = _md5, Token = _token, GUID = _guid, UserType = _utype;
			flag = 1;
			break;
		}
	}

	if (!flag)
		UserName = PasswordMD5 = Token = GUID = "", UserType = 100;

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

std::string CUser::GetToken()
{
	return Token;
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

std::string UserFile(std::string GUID, std::string Type, std::string Name)
{
	return DatabasePath() + "User\\" + GUID + "\\" + Type + "\\" + Name;
}
