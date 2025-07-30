// updateformat.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/OOBE/update/updateformat.exe"

#include <iostream>
#include <vector>
#include <format>
#include <filesystem>
#include "../../../../HTMLFrame/HTMLFrame.h"
#include "../index_OOBE_update/LegacyFormat.h"
#pragma comment(lib, "../../../../x64/Release/HTMLFrame.lib")

namespace fs = std::filesystem;
std::vector<LegacyUser> UserList;
void CopyDirectory(const fs::path& src_dir, const fs::path& dst_dir);

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\..\\");

    if (argc != 3)
        return 0;

    bool IsUpdate = String2Num(argv[1]);

    if (strcmp(argv[2], "NEWVERSION")) // 1.0 - 2.0
    {
        LegacyUser User;
        User.LegacyLoadUser(argv[2]);

        if (User.GetUserType() != UT_ROOT)
        {
            AJAXOutput("false");
            return 0;
        }

        RemoveDir("screenshoot");
        RemoveDir("temp");

        CreateDirectoryA("storage\\screenshot", NULL);
        CreateDirectoryA("temp", NULL);
        CreateDirectoryA("temp\\data", NULL);
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

                fout2.open(UserFile(GUID, "info", "md5"));
                fout2 << UserItem.GetPasswordMD5();
                fout2.close();

                fout2.open(UserFile(GUID, "info", "type"));
                fout2 << UserItem.GetUserType();
                fout2.close();

                RCAuth("AddUser", GUID);
            }
            fout.close();

            CreateDirectoryA((DatabasePath() + "User\\00000000-0000-0000-0000-000000000000").c_str(), NULL);
            CreateDirectoryA((DatabasePath() + "User\\00000000-0000-0000-0000-000000000000\\info").c_str(), NULL);
            CreateDirectoryA((DatabasePath() + "User\\00000000-0000-0000-0000-000000000000\\data").c_str(), NULL);

            fout.open(DatabasePath() + "OOBEFlag");
            fout << "4.0";
            fout.close();

            HTMLFrame HTML;
            HTML.Log("成功从旧版Remote Controller上迁移了数据并完成初始化", "OOBE", LL_INFO);
        }
        else
        {
            RemoveDir(DatabasePath().c_str());
        }
    }
    else // 3.0
    {
        HTMLFrame HTML;
        HTML.Register(PT_INDEX, ADDRESS, "updateformat", "updateformat", "", true);

        if (HTML.User.GetUserType() > UT_ADMIN)
            return 0;

        std::ifstream fin, fin2;
        std::ofstream fout;

        // ForegroundUser
        CreateDirectoryA((DatabasePath() + "User\\00000000-0000-0000-0000-000000000000").c_str(), NULL);
        CreateDirectoryA((DatabasePath() + "User\\00000000-0000-0000-0000-000000000000\\info").c_str(), NULL);
        CreateDirectoryA((DatabasePath() + "User\\00000000-0000-0000-0000-000000000000\\data").c_str(), NULL);

        // Screenshot
        CreateDirectoryA("storage\\screenshot", NULL);
        CopyDirectory("screenshoot\\cache", "storage\\screenshot");
        RemoveDir("screenshoot");

        // User: screenshotlist & token
        fin.open(DatabasePath() + "User\\list");
        if (!fin.fail())
        {
            int tot, cnt;
            CUser Temp;
            std::string _uname, _guid, FileName, NewFileName;
            std::vector<std::string> ScreenshotList;

            fin >> tot;
            for (int i = 1; i <= tot; i++)
            {
                fin >> _uname >> _guid;
                Temp.Assign(_guid, _uname);

                rename(Temp.Data("screenshootlist").c_str(), Temp.Data("screenshotlist").c_str());
                remove(UserFile(Temp.GetGUID(), "info", "token").c_str());

                fin2.open(Temp.Data("screenshotlist"));
                
                ScreenshotList.clear();
                if (!fin2.fail())
                {
                    fin2 >> cnt;
                    for (int i = 1; i <= cnt; ++i)
                    {
                        fin2 >> FileName;
                        NewFileName = FileName;
                        NewFileName.erase(9, 1);

                        rename(("storage\\screenshot\\" + FileName + ".jpg").c_str(), ("storage\\screenshot\\" + NewFileName + ".jpg").c_str());
                        ScreenshotList.push_back(NewFileName);
                    }
                    
                    fout.open(Temp.Data("screenshotlist"));
                    fout << cnt << std::endl;
                    for (auto Item : ScreenshotList)
                        fout << Item << std::endl;
                    fout.close();
                }

                fin2.close();

            }
        }
        fin.close();

        fout.open(DatabasePath() + "OOBEFlag");
        fout << "4.0";
        fout.close();

        HTML.Log("成功升级了Remote Controller数据格式，版本4.0", "OOBE", LL_INFO);
    }

    AJAXOutput("true");

    return 0;

}

void CopyDirectory(const fs::path& src_dir, const fs::path& dst_dir)
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
            copy_file(path, dest_path, fs::copy_options::update_existing);
        }
        else {
            std::cerr << "Unhandled path: " << path << std::endl;
        }
    }
}