// updateformat.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2024 Coral Studio. All Rights Reserved.

#define ADDRESS "/OOBE/update/updateformat.exe"

#include <iostream>
#include <vector>
#include "../../../../HTMLFrame/HTMLFrame.h"
#include "../index_OOBE_update/LegacyFormat.h"
#pragma comment(lib, "../../../../x64/Release/HTMLFrame.lib")

std::vector<LegacyUser> UserList;
int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\..\\");

    if (argc != 3)
        return 0;

    bool IsUpdate = String2Num(argv[1]);
    LegacyUser User;
    User.LegacyLoadUser(argv[2]);

    if (User.GetUserType() != UT_ROOT)
    {
        AJAXOutput("false");
        return 0;
    }

    RemoveDir("screenshoot\\cache");
    RemoveDir("temp");

    CreateDirectoryA("screenshoot\\cache", NULL);
    CreateDirectoryA("temp", NULL);
    CreateDirectoryA("temp\\download", NULL);
    CreateDirectoryA("temp\\wallpaper", NULL);

    RCAgent("updateformat", "root", "");

    if (IsUpdate)
    {
        std::ifstream fin;
        std::ofstream fout, fout2;
        fin.open(DatabasePath() + "User\\list");
        int tot;
        fin >> tot;
        std::string _token;

        for (int i = 1; i <= tot; i++)
        {
            fin >> _token;
            User.LegacyLoadUser(_token);
            UserList.push_back(User);
            fin >> _token >> _token;
        }

        RemoveDir(DatabasePath().c_str());
        CreateDirectoryA(DatabasePath().c_str(), NULL);

        CreateDirectoryA((DatabasePath() + "Log").c_str(), NULL);
        CreateDirectoryA((DatabasePath() + "Settings").c_str(), NULL);

        CreateDirectoryA((DatabasePath() + "User").c_str(), NULL);
        fout.open(DatabasePath() + "User\\list");
        fout << tot << std::endl;
        for (auto UserItem : UserList)
        {
            fout << UserItem.GetName() << std::endl;

            GUID guid;
            CoCreateGuid(&guid);
            std::string GUID = GuidToString(guid);
            fout << GUID << std::endl;

            CreateDirectoryA((DatabasePath() + "User\\" + GUID).c_str(), NULL);
            CreateDirectoryA((DatabasePath() + "User\\" + GUID + "\\info").c_str(), NULL);
            CreateDirectoryA((DatabasePath() + "User\\" + GUID + "\\data").c_str(), NULL);

            fout2.open(UserFile(GUID, "info", "token"));
            fout2 << NewToken();
            fout2.close();

            fout2.open(UserFile(GUID, "info", "md5"));
            fout2 << UserItem.GetPasswordMD5();
            fout2.close();

            fout2.open(UserFile(GUID, "info", "type"));
            fout2 << UserItem.GetUserType();
            fout2.close();
        }
        fout.close();

        fout.open(DatabasePath() + "OOBEFlag");
        fout << "3.0";
        fout.close();
    }
    else
    {
        RemoveDir(DatabasePath().c_str());
    }

    AJAXOutput("true");

    HTMLFrame HTML;
    HTML.Log("成功从旧版Remote Controller上迁移了数据并完成初始化", "OOBE", LL_INFO);

    return 0;

}
