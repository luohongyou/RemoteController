// CUser.h : Coral Remote Controller 用户类和函数的定义。

// 版权声明：
// Copyright (C) 2020-2024 Coral Studio. All Rights Reserved.

#ifndef RC_CUSER
#define RC_CUSER

#include <string>

// 用户类型
#define UT_ROOT  0
#define UT_ADMIN 1
#define UT_USER  2
#define UT_GUEST 3

class CUser
{
protected:
	std::string UserName;
	std::string PasswordMD5;
	std::string Token;
	std::string GUID;

	int UserType;

public:
	CUser();
	void Assign(std::string _Token);
	void Assign(std::string _UserName, std::string _PasswordMD5);
	std::string GetName();
	std::string GetPasswordMD5();
	std::string GetToken();
	std::string GetGUID();
	// std::string GetLastLogin();
	int         GetUserType();

	std::string Data(std::string Name);

	int IsOOBEed();
};

std::string NewToken();                  // 生成新Token
std::string UserFile(std::string GUID, std::string Type, std::string Name);

#endif