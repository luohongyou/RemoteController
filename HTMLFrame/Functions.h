// Functions.h : Coral Remote Controller 常用函数的定义。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#ifndef RC_FUNCTIONS
#define RC_FUNCTIONS

#include <string>
#include <Windows.h>
#include <TlHelp32.h>
#include <shellapi.h>

void        str_replace_all(std::string& str, const std::string& from, const std::string& to);

void        SetAppDirectory(std::string s);
std::string DatabasePath();
int         GetSettings(std::string ID, int Default);
void        SetSettings(std::string ID, int Data);

std::string GetCompileInfo();

bool        RemoveDir(const char* szFileDir);

std::string RCAgent(std::string Action, std::string User, std::string Data);
std::string RCAuth(std::string Action, std::string Data);

std::string StandardTime(bool IsFileName);
std::string StandardTimeWithoutSec(bool IsFileName);
std::string StandardTime(bool IsFileName, time_t time);
std::string StandardTimeWithoutSec(bool IsFileName, time_t time);

void        AJAXOutput(std::string Code);

int         CheckProcess(std::string s);
int         Fast_CheckProcess(std::string s);
void        StopProcess(std::string s);
DWORD		WinExecAndWait32(const char* lpszAppPath, const char* lpParameters, const char* lpszDirectory, DWORD dwMilliseconds);
void        ForceSetForegroundWindow(HWND hwnd);

bool        ExpCheckAddress(std::string _Address);
#endif