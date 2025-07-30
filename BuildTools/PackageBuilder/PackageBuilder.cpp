// PackageBuilder.cpp : Coral Remote Controller 自动打包发布程序

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#include <print>
#include <format>
#include <fstream>
#include <Windows.h>
#include "../../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../x64/Release/HTMLFrame.lib")

std::string PackageFolder = "D:\\Websites\\Coral Remote Controller\\";
std::string ReleaseFolder = "E:\\项目\\Packages\\";
bool RCError = 0;

void RCCopyFileA(LPCSTR lpExistingFileName, LPCSTR lpNewFileName, BOOL bFailIfExists);

int main()
{
    SetAppDirectory("\\..\\");

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
    std::print("Coral Remote Controller {} Release Package Builder\n", VERSION);
    std::print("Package to be built: {}", GetCompileInfo());
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);


    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
    std::print("\n\n------------------------------------------------\nSTEP 1: UNINSTALLING CORAL REMOTE CONTROLLER...\n------------------------------------------------\n\n");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    system(("\"" + PackageFolder + "uninstall.bat\"").c_str());

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
    std::print("\n\n------------------------------------------\nSTEP 2: REMOVING EXISTING FILES...\n------------------------------------------\n\n");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    RemoveDir((PackageFolder + "Website\\htdocs").c_str());
    std::print("Removed: {}\n", PackageFolder + "Website\\htdocs");
    remove((PackageFolder + "Website\\logs\\access.log").c_str());
    remove((PackageFolder + "Website\\logs\\error.log").c_str());
    remove((PackageFolder + "Website\\logs\\ssl_request.log").c_str());
    std::print("Removed: {}\n", PackageFolder + "Website\\logs");
    RemoveDir((PackageFolder + "ManageUI").c_str());
    std::print("Removed: {}\n", PackageFolder + "ManageUI");

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
    std::print("\n\n----------------------------------------\nSTEP 3: COPYING COMPILED FILES...\n----------------------------------------\n\n");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    if (system("WebsiteBuilder.exe"))
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
        std::print("\nERROR: Copy File Failed!\n");
        return 0;
    }

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
    std::print("\n\n------------------------------------------------\nSTEP 4: COPYING INSTALL SCRIPTS & RESOURCES...\n------------------------------------------------\n\n");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    RCCopyFileA("..\\..\\Resources\\install.bat", (PackageFolder + "install.bat").c_str(), false);
    RCCopyFileA("..\\..\\Resources\\uninstall.bat", (PackageFolder + "uninstall.bat").c_str(), false);
    RCCopyFileA("..\\..\\Resources\\VC_redist.x64.exe", (PackageFolder + "VC_redist.x64.exe").c_str(), false);
    RCCopyFileA("..\\..\\Resources\\MicrosoftEdgeWebview2Setup.exe", (PackageFolder + "MicrosoftEdgeWebview2Setup.exe").c_str(), false);

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
    std::print("\n\n-------------------------------------------\nSTEP 5: WRITING BUILD INFORMATION...\n-------------------------------------------\n\n");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    std::print("Build: {}\nVersion: {}, {}\n", PROGRAM_BUILD, VERSION, BRANCH);
    std::ofstream fout(PackageFolder + "BuildInfo.ini");
    fout << PROGRAM_BUILD << std::endl << VERSION << std::endl << BRANCH;
    fout.close();

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
    std::print("\n\n-------------------------------------------\nSTEP 6: MAKING COMPRESSED PACKAGE...\n-------------------------------------------\n");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    system(((std::string)"..\\..\\Resources\\7zip\\7z.exe a \"" + ReleaseFolder + "Coral Remote Controller " + VERSION + " build " + PROGRAM_BUILD_S + ".zip\" \"" + PackageFolder + "\"").c_str());

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    std::print("\n\nRun Coral Remote Controller now? (y/n)");
    if (getchar() == 'y')
        system(("\"" + PackageFolder + "install.bat\"").c_str());

    return 0;
}
void RCCopyFileA(LPCSTR lpExistingFileName, LPCSTR lpNewFileName, BOOL bFailIfExists)
{
    if (CopyFileA(lpExistingFileName, lpNewFileName, bFailIfExists))
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
        std::print("Success: ");
    }
    else
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
        std::print("Failed:  ");
        RCError = 1;
    }

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    std::print("{}\n", lpNewFileName);
}
