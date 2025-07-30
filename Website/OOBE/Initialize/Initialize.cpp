// Initialize.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/OOBE/Initialize.exe"

#include <iostream>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\");

    if (argc != 2)
        return 0;

    HTMLFrame HTML;
    HTML.Register(PT_INDEX, ADDRESS, "Initialize", "Initialize", "", false);
    HTML.Custom(R"(<script> window.location.href = "/"; </script>)");

    // 检查是否已经OOBE
    std::ifstream fin;
    std::ofstream fout;
    fin.open(DatabasePath() + "OOBEFlag");
    if (!fin.fail())
    {
        fin.close();
        HTML.Output();
        return 0;
    }

    CreateDirectoryA(DatabasePath().c_str(), NULL);

    GUID guid;
    CoCreateGuid(&guid);
    std::string GUID = GuidToString(guid);

    CreateDirectoryA((DatabasePath() + "Log").c_str(), NULL);
    CreateDirectoryA((DatabasePath() + "Settings").c_str(), NULL);

    CreateDirectoryA((DatabasePath() + "User").c_str(), NULL);

    CreateDirectoryA((DatabasePath() + "User\\00000000-0000-0000-0000-000000000000").c_str(), NULL);
    CreateDirectoryA((DatabasePath() + "User\\00000000-0000-0000-0000-000000000000\\info").c_str(), NULL);
    CreateDirectoryA((DatabasePath() + "User\\00000000-0000-0000-0000-000000000000\\data").c_str(), NULL);

    fout.open(DatabasePath() + "User\\list");
    fout << "1\n";
    fout << "root\n";
    fout << GUID << std::endl;
    fout.close();

    CreateDirectoryA((DatabasePath() + "User\\" + GUID).c_str(), NULL);
    CreateDirectoryA((DatabasePath() + "User\\" + GUID + "\\info").c_str(), NULL);
    CreateDirectoryA((DatabasePath() + "User\\" + GUID + "\\data").c_str(), NULL);

    fout.open(UserFile(GUID, "info", "md5"));
    fout << argv[1];
    fout.close();

    fout.open(UserFile(GUID, "info", "type"));
    fout << 0;
    fout.close();

    fout.open(DatabasePath() + "OOBEFlag");
    fout << "4.0";
    fout.close();

    RCAuth("AddUser", GUID);

    HTML.Output();
    HTML.Log("成功完成了系统初始化设置", "OOBE", LL_INFO);

    return 0;
}

