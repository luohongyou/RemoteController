// CUser.h : Coral Remote Controller �û���ͺ����Ķ��塣

// ��Ȩ������
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#ifndef RC_CUSER
#define RC_CUSER

#include <string>

// �û�����
#define UT_ROOT  0
#define UT_ADMIN 1
#define UT_USER  2
#define UT_GUEST 3

class CUser
{
protected:
	std::string UserName;
	std::string PasswordMD5;
	std::string GUID;

	int UserType;

public:
	CUser();
	void Assign(std::string _GUID);
	void Assign(std::string _GUID, std::string _UserName);
	std::string GetName();
	std::string GetPasswordMD5();
	std::string GetGUID();
	int         GetUserType();

	std::string Data(std::string Name);

	int IsOOBEed();
	int IsForegroundUser();
	int IsCloudStorageUser();
};

std::string NewToken();                  // ������Token
std::string NewHostToken();              // �����±���Token
std::string NewCloudToken();             // �����������û�Token
std::string UserFile(std::string GUID, std::string Type, std::string Name);

#endif