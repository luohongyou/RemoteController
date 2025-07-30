// LegacyFormat.h : Coral Remote Controller 旧文件存储格式支持。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#ifndef RC_LEGACY
#define RC_LEGACY

#include <Windows.h>
#include <string>
#include <fstream>

// Coral Remote Controller 1.0 - 2.0 File Format & 3.0 Tokens on the Disk

#define UTL_GUEST 0
#define UTL_USER  1
#define UTL_ADMIN 2
#define UTL_ROOT  3

class LegacyUser : public CUser
{
protected:
	std::string Token;      // Deprecated in 4.0
public:
	LegacyUser()
	{
		Token = "";
	}
	std::string GetToken()  // Deprecated in 4.0
	{
		return Token;
	}
	void LegacyLoadUser(std::string _UserName, std::string _PasswordMD5)  // Version 2.0, Deprecated in 3.0
	{
		std::ifstream fin;
		fin.open(DatabasePath() + "User\\list");
		int tot;
		fin >> tot;
		std::string _token, _uname, _md5;
		UserName = PasswordMD5 = Token = "";
		UserType = UTL_GUEST;

		for (int i = 1; i <= tot; i++)
		{
			fin >> _token >> _uname >> _md5;
			if (_uname == _UserName && _md5 == _PasswordMD5)
			{
				UserName = _uname, PasswordMD5 = _md5, Token = _token;
				break;
			}
		}

		fin.close();

		if (UserName == "")
			return;

		std::string AuthList;
		fin.open(DatabasePath() + "User\\authlist\\" + UserName);
		getline(fin, AuthList);
		fin.close();
		for (int i = AuthList.size() - 1; i >= 0; i--)
			if ('0' <= AuthList[i] && AuthList[i] <= '5')
			{
				UserType = AuthList[i] - '0';
				break;
			}

		switch (UserType)
		{
		case UTL_ROOT:
			UserType = UT_ROOT;
			break;
		case UTL_ADMIN:
			UserType = UT_ADMIN;
			break;
		case UTL_USER:
			UserType = UT_USER;
			break;
		case UTL_GUEST:
			UserType = UT_GUEST;
			break;
		}
	}
	void LegacyLoadUser(std::string _Token)  // Version 2.0, Deprecated in 3.0
	{
		std::ifstream fin;
		fin.open(DatabasePath() + "User\\list");
		int tot;
		fin >> tot;
		std::string _token, _uname, _md5;
		UserName = PasswordMD5 = Token = "";
		UserType = UTL_GUEST;

		for (int i = 1; i <= tot; i++)
		{
			fin >> _token >> _uname >> _md5;
			if (_token == _Token)
			{
				UserName = _uname, PasswordMD5 = _md5, Token = _token;
				break;
			}
		}

		fin.close();

		if (UserName == "")
			return;

		std::string AuthList;
		fin.open(DatabasePath() + "User\\authlist\\" + UserName);
		getline(fin, AuthList);
		fin.close();
		for (int i = AuthList.size() - 1; i >= 0; i--)
			if ('0' <= AuthList[i] && AuthList[i] <= '5')
			{
				UserType = AuthList[i] - '0';
				break;
			}
		switch (UserType)
		{
		case UTL_ROOT:
			UserType = UT_ROOT;
			break;
		case UTL_ADMIN:
			UserType = UT_ADMIN;
			break;
		case UTL_USER:
			UserType = UT_USER;
			break;
		case UTL_GUEST:
			UserType = UT_GUEST;
			break;
		}
	}
	void LegacyLoadUser_v3(std::string _UserName, std::string _PasswordMD5)  // Version 3.0, Deprecated in 4.0
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
				if (!fin2.fail())  // for v4.0 and higher
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
	void LegacyLoadUser_v3(std::string _Token)  // Version 3.0, Deprecated in 4.0
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
			if (!fin2.fail())  // for v4.0 and higher
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
};

#endif // !RC_LEGACY
