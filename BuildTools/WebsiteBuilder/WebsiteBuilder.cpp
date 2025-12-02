// WebsiteBuilder.cpp : Coral Remote Controller 网站自动生成工具

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#include <print>
#include <filesystem>
#include <Windows.h>
#include "../../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../x64/Release/HTMLFrame.lib")

namespace fs = std::filesystem;
std::string ReleaseFolder = "D:\\Websites\\Coral Remote Controller\\Website\\htdocs\\";
bool RCError = 0;

void CopyDirectory(const fs::path& src_dir, const fs::path& dst_dir);
void RCCopyFileA(LPCSTR lpExistingFileName, LPCSTR lpNewFileName, BOOL bFailIfExists);

int main()
{
    SetAppDirectory("\\..\\");
    Init_CRC32_Tab();

    StopProcess("agentprotect.exe");
    StopProcess("agentupdater.exe");
    StopProcess("rcagent.exe");
    if (Calc_File_CRC32("rcauth.exe") != Calc_File_CRC32((ReleaseFolder + "..\\..\\ManageUI\\agent\\rcauth.exe").c_str()))
        StopProcess("rcauth.exe");
    StopProcess("jsmpeg_vnc_coral.exe");

    // '/'
    CreateDirectoryA(ReleaseFolder.c_str(), NULL);
    RCCopyFileA("index.exe", (ReleaseFolder + "index.exe").c_str(), false);
    RCCopyFileA("..\\..\\Resources\\favicon.ico", (ReleaseFolder + "favicon.ico").c_str(), false);

    // assets
    CopyDirectory("..\\..\\Resources\\assets", ReleaseFolder + "assets");

    // clipboard
    CreateDirectoryA((ReleaseFolder + "clipboard").c_str(), NULL);
    RCCopyFileA("index_clipboard.exe", (ReleaseFolder + "clipboard\\index.exe").c_str(), false);
    RCCopyFileA("getclipboardnow.exe", (ReleaseFolder + "clipboard\\getclipboardnow.exe").c_str(), false);
    RCCopyFileA("getclipboardhistory.exe", (ReleaseFolder + "clipboard\\getclipboardhistory.exe").c_str(), false);
    RCCopyFileA("modifyclipboard.exe", (ReleaseFolder + "clipboard\\modifyclipboard.exe").c_str(), false);
    RCCopyFileA("clearclipboardhistory.exe", (ReleaseFolder + "clipboard\\clearclipboardhistory.exe").c_str(), false);

    // command
    CreateDirectoryA((ReleaseFolder + "command").c_str(), NULL);
    RCCopyFileA("index_command.exe", (ReleaseFolder + "command\\index.exe").c_str(), false);
    RCCopyFileA("batchfilehandler.exe", (ReleaseFolder + "command\\batchfilehandler.exe").c_str(), false);

    // datapush
    CreateDirectoryA((ReleaseFolder + "datapush").c_str(), NULL);
    RCCopyFileA("index_datapush.exe", (ReleaseFolder + "datapush\\index.exe").c_str(), false);

    // login
    CreateDirectoryA((ReleaseFolder + "login").c_str(), NULL);
    RCCopyFileA("index_login.exe", (ReleaseFolder + "login\\index.exe").c_str(), false);
    RCCopyFileA("checkpassword.exe", (ReleaseFolder + "login\\checkpassword.exe").c_str(), false);
    RCCopyFileA("datatracker.exe", (ReleaseFolder + "login\\datatracker.exe").c_str(), false);
    RCCopyFileA("RSAPublicKey.exe", (ReleaseFolder + "login\\RSAPublicKey.exe").c_str(), false);
    // login/cloudstorage
    CreateDirectoryA((ReleaseFolder + "login\\cloudstorage").c_str(), NULL);
    RCCopyFileA("index_login_cloudstorage.exe", (ReleaseFolder + "login\\cloudstorage\\index.exe").c_str(), false);
    RCCopyFileA("checkpin.exe", (ReleaseFolder + "login\\cloudstorage\\checkpin.exe").c_str(), false);

    // errors
    CreateDirectoryA((ReleaseFolder + "errors").c_str(), NULL);
    // errors/403
    CreateDirectoryA((ReleaseFolder + "errors\\403").c_str(), NULL);
    RCCopyFileA("index_errors_403.exe", (ReleaseFolder + "errors\\403\\index.exe").c_str(), false);
    // errors/404
    CreateDirectoryA((ReleaseFolder + "errors\\404").c_str(), NULL);
    RCCopyFileA("index_errors_404.exe", (ReleaseFolder + "errors\\404\\index.exe").c_str(), false);
    // errors/500
    CreateDirectoryA((ReleaseFolder + "errors\\500").c_str(), NULL);
    RCCopyFileA("index_errors_500.exe", (ReleaseFolder + "errors\\500\\index.exe").c_str(), false);

    // explorer
    CreateDirectoryA((ReleaseFolder + "explorer").c_str(), NULL);
    CopyDirectory("..\\..\\Resources\\explorerResources", ReleaseFolder + "explorer\\Resources");
    RCCopyFileA("index_explorer.exe", (ReleaseFolder + "explorer\\index.exe").c_str(), false);
    RCCopyFileA("filelist.exe", (ReleaseFolder + "explorer\\filelist.exe").c_str(), false);
    RCCopyFileA("expuploadhandler.exe", (ReleaseFolder + "explorer\\expuploadhandler.exe").c_str(), false);
    RCCopyFileA("expactionhandler.exe", (ReleaseFolder + "explorer\\expactionhandler.exe").c_str(), false);
    RCCopyFileA("quicklook.exe", (ReleaseFolder + "explorer\\quicklook.exe").c_str(), false);
    RCCopyFileA("quickstartbar.exe", (ReleaseFolder + "explorer\\quickstartbar.exe").c_str(), false);
    RCCopyFileA("filedownload.exe", (ReleaseFolder + "explorer\\filedownload.exe").c_str(), false);

    // OOBE
    CreateDirectoryA((ReleaseFolder + "OOBE").c_str(), NULL);
    RCCopyFileA("index_OOBE.exe", (ReleaseFolder + "OOBE\\index.exe").c_str(), false);
    RCCopyFileA("Initialize.exe", (ReleaseFolder + "OOBE\\Initialize.exe").c_str(), false);
    // OOBE/update
    CreateDirectoryA((ReleaseFolder + "OOBE\\update").c_str(), NULL);
    RCCopyFileA("index_OOBE_update.exe", (ReleaseFolder + "OOBE\\update\\index.exe").c_str(), false);
    RCCopyFileA("legacycheckroot.exe", (ReleaseFolder + "OOBE\\update\\legacycheckroot.exe").c_str(), false);
    RCCopyFileA("updateformat.exe", (ReleaseFolder + "OOBE\\update\\updateformat.exe").c_str(), false);
    // OOBE/user
    CreateDirectoryA((ReleaseFolder + "OOBE\\user").c_str(), NULL);
    RCCopyFileA("index_OOBE_user.exe", (ReleaseFolder + "OOBE\\user\\index.exe").c_str(), false);
    RCCopyFileA("newpassword.exe", (ReleaseFolder + "OOBE\\user\\newpassword.exe").c_str(), false);
    RCCopyFileA("changelog.exe", (ReleaseFolder + "OOBE\\user\\changelog.exe").c_str(), false);
    RCCopyFileA("finishuserOOBE.exe", (ReleaseFolder + "OOBE\\user\\finishuserOOBE.exe").c_str(), false);
    // OOBE/user/changepassword
    CreateDirectoryA((ReleaseFolder + "OOBE\\user\\changepassword").c_str(), NULL);
    RCCopyFileA("index_OOBE_user_changepassword.exe", (ReleaseFolder + "OOBE\\user\\changepassword\\index.exe").c_str(), false);
    RCCopyFileA("forcechangepw.exe", (ReleaseFolder + "OOBE\\user\\changepassword\\forcechangepw.exe").c_str(), false);

    // manage
    CreateDirectoryA((ReleaseFolder + "manage").c_str(), NULL);
    // manage/main
    CreateDirectoryA((ReleaseFolder + "manage\\main").c_str(), NULL);
    RCCopyFileA("index_manage_main.exe", (ReleaseFolder + "manage\\main\\index.exe").c_str(), false);
    // manage/main/log
    CreateDirectoryA((ReleaseFolder + "manage\\main\\log").c_str(), NULL);
    RCCopyFileA("index_manage_main_log.exe", (ReleaseFolder + "manage\\main\\log\\index.exe").c_str(), false);
    RCCopyFileA("logdisplayer.exe", (ReleaseFolder + "manage\\main\\log\\logdisplayer.exe").c_str(), false);
    RCCopyFileA("clearlog.exe", (ReleaseFolder + "manage\\main\\log\\clearlog.exe").c_str(), false);
    // manage/main/settings
    CreateDirectoryA((ReleaseFolder + "manage\\main\\settings").c_str(), NULL);
    RCCopyFileA("index_manage_main_settings.exe", (ReleaseFolder + "manage\\main\\settings\\index.exe").c_str(), false);
    RCCopyFileA("settingswitcher.exe", (ReleaseFolder + "manage\\main\\settings\\settingswitcher.exe").c_str(), false);
    RCCopyFileA("reset.exe", (ReleaseFolder + "manage\\main\\settings\\reset.exe").c_str(), false);
    // manage/main/storage
    CreateDirectoryA((ReleaseFolder + "manage\\main\\storage").c_str(), NULL);
    RCCopyFileA("index_manage_main_storage.exe", (ReleaseFolder + "manage\\main\\storage\\index.exe").c_str(), false);
    RCCopyFileA("clearstorage.exe", (ReleaseFolder + "manage\\main\\storage\\clearstorage.exe").c_str(), false);
    // manage/main/user
    CreateDirectoryA((ReleaseFolder + "manage\\main\\user").c_str(), NULL);
    RCCopyFileA("index_manage_main_user.exe", (ReleaseFolder + "manage\\main\\user\\index.exe").c_str(), false);
    RCCopyFileA("listuser.exe", (ReleaseFolder + "manage\\main\\user\\listuser.exe").c_str(), false);
    RCCopyFileA("adduser.exe", (ReleaseFolder + "manage\\main\\user\\adduser.exe").c_str(), false);
    RCCopyFileA("deleteuser.exe", (ReleaseFolder + "manage\\main\\user\\deleteuser.exe").c_str(), false);
    // manage/main/user/cloudstorage
    CreateDirectoryA((ReleaseFolder + "manage\\main\\user\\cloudstorage").c_str(), NULL);
    RCCopyFileA("index_manage_main_user_cloudstorage.exe", (ReleaseFolder + "manage\\main\\user\\cloudstorage\\index.exe").c_str(), false);
    // manage/main/update
    CreateDirectoryA((ReleaseFolder + "manage\\main\\update").c_str(), NULL);
    RCCopyFileA("index_manage_main_update.exe", (ReleaseFolder + "manage\\main\\update\\index.exe").c_str(), false);
    RCCopyFileA("uploadpackage.exe", (ReleaseFolder + "manage\\main\\update\\uploadpackage.exe").c_str(), false);

    // manage/account
    CreateDirectoryA((ReleaseFolder + "manage\\account").c_str(), NULL);
    RCCopyFileA("index_manage_account.exe", (ReleaseFolder + "manage\\account\\index.exe").c_str(), false);
    RCCopyFileA("changepassword.exe", (ReleaseFolder + "manage\\account\\changepassword.exe").c_str(), false);

    // message
    CreateDirectoryA((ReleaseFolder + "message").c_str(), NULL);
    RCCopyFileA("index_message.exe", (ReleaseFolder + "message\\index.exe").c_str(), false);
    RCCopyFileA("onewaymessage.exe", (ReleaseFolder + "message\\onewaymessage.exe").c_str(), false);

    // network
    CreateDirectoryA((ReleaseFolder + "network").c_str(), NULL);
    RCCopyFileA("index_network.exe", (ReleaseFolder + "network\\index.exe").c_str(), false);
    RCCopyFileA("listrule.exe", (ReleaseFolder + "network\\listrule.exe").c_str(), false);
    RCCopyFileA("addrule.exe", (ReleaseFolder + "network\\addrule.exe").c_str(), false);
    RCCopyFileA("deleterule.exe", (ReleaseFolder + "network\\deleterule.exe").c_str(), false);

    // power
    CreateDirectoryA((ReleaseFolder + "power").c_str(), NULL);
    RCCopyFileA("index_power.exe", (ReleaseFolder + "power\\index.exe").c_str(), false);
    RCCopyFileA("powercfg.exe", (ReleaseFolder + "power\\powercfg.exe").c_str(), false);
    RCCopyFileA("volume.exe", (ReleaseFolder + "power\\volume.exe").c_str(), false);
    RCCopyFileA("lockpc.exe", (ReleaseFolder + "power\\lockpc.exe").c_str(), false);
    RCCopyFileA("changewallpaper.exe", (ReleaseFolder + "power\\changewallpaper.exe").c_str(), false);
    RCCopyFileA("getwallpaper.exe", (ReleaseFolder + "power\\getwallpaper.exe").c_str(), false);

    // screenshot
    CreateDirectoryA((ReleaseFolder + "screenshot").c_str(), NULL);
    RCCopyFileA("index_screenshot.exe", (ReleaseFolder + "screenshot\\index.exe").c_str(), false);
    RCCopyFileA("printscreen.exe", (ReleaseFolder + "screenshot\\printscreen.exe").c_str(), false);
    RCCopyFileA("screenshotlist.exe", (ReleaseFolder + "screenshot\\screenshotlist.exe").c_str(), false);
    RCCopyFileA("screenshotdelete.exe", (ReleaseFolder + "screenshot\\screenshotdelete.exe").c_str(), false);

    // storage
    CreateDirectoryA((ReleaseFolder + "storage").c_str(), NULL);
    CreateDirectoryA((ReleaseFolder + "storage\\screenshot").c_str(), NULL);
    CreateDirectoryA((ReleaseFolder + "storage\\sharedzone").c_str(), NULL);

    // sysinfo
    CreateDirectoryA((ReleaseFolder + "sysinfo").c_str(), NULL);
    RCCopyFileA("index_sysinfo.exe", (ReleaseFolder + "sysinfo\\index.exe").c_str(), false);
    RCCopyFileA("hardwareinfo.exe", (ReleaseFolder + "sysinfo\\hardwareinfo.exe").c_str(), false);
    RCCopyFileA("softwareinfo.exe", (ReleaseFolder + "sysinfo\\softwareinfo.exe").c_str(), false);

    // temp
    CreateDirectoryA((ReleaseFolder + "temp").c_str(), NULL);
    CreateDirectoryA((ReleaseFolder + "temp\\data").c_str(), NULL);
    CreateDirectoryA((ReleaseFolder + "temp\\download").c_str(), NULL);
    CreateDirectoryA((ReleaseFolder + "temp\\wallpaper").c_str(), NULL);

    // taskmgr
    CreateDirectoryA((ReleaseFolder + "taskmgr").c_str(), NULL);
    RCCopyFileA("index_taskmgr.exe", (ReleaseFolder + "taskmgr\\index.exe").c_str(), false);
    RCCopyFileA("tasklist.exe", (ReleaseFolder + "taskmgr\\tasklist.exe").c_str(), false);
    RCCopyFileA("taskkill.exe", (ReleaseFolder + "taskmgr\\taskkill.exe").c_str(), false);

    // windowmgr
    CreateDirectoryA((ReleaseFolder + "windowmgr").c_str(), NULL);
    RCCopyFileA("index_windowmgr.exe", (ReleaseFolder + "windowmgr\\index.exe").c_str(), false);
    RCCopyFileA("listwindows.exe", (ReleaseFolder + "windowmgr\\listwindows.exe").c_str(), false);
    RCCopyFileA("closewindow.exe", (ReleaseFolder + "windowmgr\\closewindow.exe").c_str(), false);

    // ManageUI
    CreateDirectoryA((ReleaseFolder + "..\\..\\ManageUI").c_str(), NULL);
    // assets
    CopyDirectory("..\\..\\Resources\\LocalUIAssets", ReleaseFolder + "..\\..\\ManageUI\\assets");
    // agent
    CreateDirectoryA((ReleaseFolder + "..\\..\\ManageUI\\agent").c_str(), NULL);
    RCCopyFileA("rcagent.exe", (ReleaseFolder + "..\\..\\ManageUI\\agent\\rcagent.exe").c_str(), false);
    RCCopyFileA("rcauth.exe", (ReleaseFolder + "..\\..\\ManageUI\\agent\\rcauth.exe").c_str(), false);
    RCCopyFileA("..\\..\\Resources\\libcrypto-3-x64.dll", (ReleaseFolder + "..\\..\\ManageUI\\agent\\libcrypto-3-x64.dll").c_str(), false);
    RCCopyFileA("agentprotect.exe", (ReleaseFolder + "..\\..\\ManageUI\\agent\\agentprotect.exe").c_str(), false);
    RCCopyFileA("agentupdater.exe", (ReleaseFolder + "..\\..\\ManageUI\\agent\\agentupdater.exe").c_str(), false);
    RCCopyFileA("ExitRemoteController.exe", (ReleaseFolder + "..\\..\\ManageUI\\agent\\ExitRemoteController.exe").c_str(), false);
    // messager
    CreateDirectoryA((ReleaseFolder + "..\\..\\ManageUI\\messager").c_str(), NULL);
    RCCopyFileA("onewaymessager.exe", (ReleaseFolder + "..\\..\\ManageUI\\messager\\onewaymessager.exe").c_str(), false);
    RCCopyFileA("chatapp.exe", (ReleaseFolder + "..\\..\\ManageUI\\messager\\chatapp.exe").c_str(), false);
    RCCopyFileA("WebView2Loader.dll", (ReleaseFolder + "..\\..\\ManageUI\\messager\\WebView2Loader.dll").c_str(), false);
    // ui
    CreateDirectoryA((ReleaseFolder + "..\\..\\ManageUI\\ui").c_str(), NULL);
    RCCopyFileA("BlackScreenUI.exe", (ReleaseFolder + "..\\..\\ManageUI\\ui\\BlackScreenUI.exe").c_str(), false);
    RCCopyFileA("LocalUI.exe", (ReleaseFolder + "..\\..\\ManageUI\\ui\\LocalUI.exe").c_str(), false);
    RCCopyFileA("WebView2Loader.dll", (ReleaseFolder + "..\\..\\ManageUI\\ui\\WebView2Loader.dll").c_str(), false);
    // update
    CreateDirectoryA((ReleaseFolder + "..\\..\\ManageUI\\update").c_str(), NULL);
    RCCopyFileA("rcupdater.exe", (ReleaseFolder + "..\\..\\ManageUI\\update\\rcupdater.exe").c_str(), false);
    CreateDirectoryA((ReleaseFolder + "..\\..\\ManageUI\\update\\packages").c_str(), NULL);
    // 7zip
    CopyDirectory("..\\..\\Resources\\7zip", ReleaseFolder + "..\\..\\ManageUI\\7zip");
    // jsmpeg-vnc
    CopyDirectory("..\\..\\Resources\\jsmpeg-vnc", ReleaseFolder + "..\\..\\ManageUI\\jsmpeg-vnc");

    ShellExecuteA(NULL, "open", (ReleaseFolder + "..\\..\\ManageUI\\agent\\agentprotect.exe").c_str(), "", NULL, SW_HIDE);

    return RCError;
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