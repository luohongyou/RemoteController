// Functions.cpp : Coral Remote Controller 常用函数的实现。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#include "pch.h"
#include "HTMLFrame.h"
#include "Functions.h"

void str_replace_all(std::string& str, const std::string& from, const std::string& to)
{
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos)
	{
		str.replace(start_pos, from.length(), to);
		start_pos += to.length();
	}
}

void SetAppDirectory(std::string s)
{
    char strModule[256];
    GetModuleFileNameA(NULL, strModule, 256);
    std::string a;
    a.assign(strModule);
    a.append(s);
    SetCurrentDirectoryA(a.c_str());
}

std::string DatabasePath()
{
	// return "..\\..\\Database\\";
	std::string ans = "C:\\ProgramData\\Coral Remote Controller\\";
	char _str[MAX_PATH], SystemC;
	GetSystemDirectoryA(_str, MAX_PATH);
	SystemC = _str[0];
	ans[0] = SystemC;
	return ans;
}

int GetSettings(std::string ID, int Default)
{
	int Ans = Default;
	std::ifstream fin(DatabasePath() + "Settings\\" + ID);
	if (!fin.fail())
		fin >> Ans;
	fin.close();

	return Ans;
}

void SetSettings(std::string ID, int Data)
{
	std::ofstream fout(DatabasePath() + "Settings\\" + ID);
	fout << Data;
	fout.close();
}

std::string GetCompileInfo()
{
	std::string BetaCompileInfo;
	const char* month_names[] = { "Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec" };
	std::string timetemp;
	timetemp = __DATE__;
	if (timetemp[4] == ' ')
		timetemp[4] = '0';
	std::string year = timetemp.substr(9, 2);
	std::string day = timetemp.substr(4, 2);
	char month[5];
	std::string s_month = timetemp.substr(0, 3);
	int tmonth;
	for (int i = 0; i <= 11; i++)
		if (s_month == month_names[i])
		{
			tmonth = i + 1;
			break;
		}
	if (tmonth < 10)
		month[0] = '0', month[1] = tmonth + '0', month[2] = '\0';
	else
		month[0] = '1', month[1] = tmonth % 10 + '0', month[2] = '\0';
	timetemp = __TIME__;
	std::string hour = timetemp.substr(0, 2);
	std::string min = timetemp.substr(3, 2);
	BetaCompileInfo = "HTMLFrame." + (std::string)PROGRAM_BUILD_S + "." + BRANCH + "." + year + month + day + "-" + hour + min;
	return BetaCompileInfo;
}

bool RemoveDir(const char* szFileDir)
{
	std::string strDir = szFileDir;
	if (strDir.at(strDir.length() - 1) != '\\')
		strDir += '\\';
	WIN32_FIND_DATAA wfd;
	HANDLE hFind = FindFirstFileA((strDir + "*.*").c_str(), &wfd);
	if (hFind == INVALID_HANDLE_VALUE)
		return false;
	do
	{
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (strcmp(wfd.cFileName, ".") != 0 &&
				strcmp(wfd.cFileName, "..") != 0)
				RemoveDir((strDir + wfd.cFileName).c_str());
		}
		else
		{
			remove((strDir + wfd.cFileName).c_str());
		}
	} while (FindNextFileA(hFind, &wfd));
	FindClose(hFind);
	RemoveDirectoryA(szFileDir);
	return true;
}

char rbuf[25600];
std::string RCAgent(std::string Action, std::string User, std::string Data)
{
	DWORD wlen = 0;
	BOOL bRet = WaitNamedPipe(TEXT("\\\\.\\Pipe\\CoralRemoteAgent"), 5000);

	if (!bRet)
		return "CORAL_RC_AGENT_FAILED";

	HANDLE hPipe = CreateFile(TEXT("\\\\.\\Pipe\\CoralRemoteAgent"),
		GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hPipe)
		return "CORAL_RC_AGENT_FAILED";

	std::string Send, Ans;
	Send = Action + " " + User + " " + Data;
	if (WriteFile(hPipe, Send.c_str(), Send.size(), &wlen, 0) == FALSE)
		return "CORAL_RC_AGENT_FAILED";
	else
	{
		memset(rbuf, 0, sizeof(rbuf));
		DWORD rlen = 0;
		ReadFile(hPipe, rbuf, sizeof(rbuf), &rlen, 0);
		Ans = rbuf;
	}

	FlushFileBuffers(hPipe);
	DisconnectNamedPipe(hPipe);
	CloseHandle(hPipe);

	return Ans;
}
std::string RCAuth(std::string Action, std::string Data)
{
	DWORD wlen = 0;
	BOOL bRet = WaitNamedPipe(TEXT("\\\\.\\Pipe\\CoralRemoteControllerAuthManager"), 5000);

	if (!bRet)
		return "CORAL_RC_AUTH_FAILED";

	HANDLE hPipe = CreateFile(TEXT("\\\\.\\Pipe\\CoralRemoteControllerAuthManager"),
		GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hPipe)
		return "CORAL_RC_AUTH_FAILED";

	std::string Send, Ans;
	Send = Action + " " + Data;
	if (WriteFile(hPipe, Send.c_str(), Send.size(), &wlen, 0) == FALSE)
		return "CORAL_RC_AUTH_FAILED";
	else
	{
		memset(rbuf, 0, sizeof(rbuf));
		DWORD rlen = 0;
		ReadFile(hPipe, rbuf, sizeof(rbuf), &rlen, 0);
		Ans = rbuf;
	}

	FlushFileBuffers(hPipe);
	DisconnectNamedPipe(hPipe);
	CloseHandle(hPipe);

	return Ans;
}

std::string StandardTime(bool IsFileName)
{
	time_t now = time(0);
	return StandardTime(IsFileName, now);
}
std::string StandardTimeWithoutSec(bool IsFileName)
{
	time_t now = time(0);
	return StandardTimeWithoutSec(IsFileName, now);
}

std::string StandardTime(bool IsFileName, time_t Time)
{
	struct tm ltm;
	localtime_s(&ltm, &Time);

	std::string data1, data2;
	std::stringstream tempIO;
	tempIO << 1900 + ltm.tm_year
		<< (IsFileName ? '-' : '/') << (1 + ltm.tm_mon < 10 ? "0" : "") << 1 + ltm.tm_mon
		<< (IsFileName ? '-' : '/') << (ltm.tm_mday < 10 ? "0" : "") << ltm.tm_mday
		<< (IsFileName ? '-' : ' ') << (ltm.tm_hour < 10 ? "0" : "") << ltm.tm_hour
		<< (IsFileName ? '-' : ':') << (ltm.tm_min < 10 ? "0" : "") << ltm.tm_min
		<< (IsFileName ? '-' : ':') << (ltm.tm_sec < 10 ? "0" : "") << ltm.tm_sec;

	if (!IsFileName)
	{
		tempIO >> data1 >> data2;
		return data1 + ' ' + data2;
	}

	tempIO >> data1;
	return data1;
}
std::string StandardTimeWithoutSec(bool IsFileName, time_t Time)
{
	struct tm ltm;
	localtime_s(&ltm, &Time);

	std::string data1, data2;
	std::stringstream tempIO;
	tempIO << 1900 + ltm.tm_year
		<< (IsFileName ? '-' : '/') << (1 + ltm.tm_mon < 10 ? "0" : "") << 1 + ltm.tm_mon
		<< (IsFileName ? '-' : '/') << (ltm.tm_mday < 10 ? "0" : "") << ltm.tm_mday
		<< (IsFileName ? '-' : ' ') << (ltm.tm_hour < 10 ? "0" : "") << ltm.tm_hour
		<< (IsFileName ? '-' : ':') << (ltm.tm_min < 10 ? "0" : "") << ltm.tm_min;

	if (!IsFileName)
	{
		tempIO >> data1 >> data2;
		return data1 + ' ' + data2;
	}

	tempIO >> data1;
	return data1;
}

void AJAXOutput(std::string Code)
{
	puts("Content-type: text/html\n");
	std::string UTF8Code, OriginCode = Code.c_str();
	GB2312ToUTF_8(UTF8Code, (char*)OriginCode.c_str(), OriginCode.size());
	puts(UTF8Code.c_str());
}

int CheckProcess(std::string s)
{
	HANDLE hToken;
	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken);
	TOKEN_PRIVILEGES tp;
	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
	PROCESSENTRY32 pd;
	pd.dwSize = sizeof(pd);
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	BOOL choose = ::Process32First(hProcessSnap, &pd);
	int stdexe = 0;
	while (choose)
	{
		char szEXE[100];
		_bstr_t b(pd.szExeFile);
		strcpy_s(szEXE, b);
		if (!strcmp(szEXE, s.c_str()))
			++stdexe;
		choose = ::Process32Next(hProcessSnap, &pd);
	}
	return stdexe;
}
int Fast_CheckProcess(std::string s)
{
	HANDLE hToken;
	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken);
	TOKEN_PRIVILEGES tp;
	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
	PROCESSENTRY32 pd;
	pd.dwSize = sizeof(pd);
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	BOOL choose = ::Process32First(hProcessSnap, &pd);
	while (choose)
	{
		char szEXE[100];
		_bstr_t b(pd.szExeFile);
		strcpy_s(szEXE, b);
		if (!strcmp(szEXE, s.c_str()))
			return true;
		choose = ::Process32Next(hProcessSnap, &pd);
	}
	return false;
}
void StopProcess(std::string s)
{
	HANDLE hToken;
	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken);
	TOKEN_PRIVILEGES tp;
	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
	PROCESSENTRY32 pd;
	pd.dwSize = sizeof(pd);
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	BOOL choose = ::Process32First(hProcessSnap, &pd);
	while (choose)
	{
		char szEXE[100];
		_bstr_t b(pd.szExeFile);
		strcpy_s(szEXE, b);
		if (!strcmp(szEXE, s.c_str()))
		{
			HANDLE std = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pd.th32ProcessID);
			::TerminateProcess(std, 0);
		}
		choose = ::Process32Next(hProcessSnap, &pd);
	}
}
DWORD WinExecAndWait32(const char* lpszAppPath, const char* lpParameters, const char* lpszDirectory, DWORD dwMilliseconds)
{
	SHELLEXECUTEINFOA ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFOA);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = lpszAppPath;
	ShExecInfo.lpParameters = lpParameters;
	ShExecInfo.lpDirectory = lpszDirectory;
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = NULL;
	ShellExecuteExA(&ShExecInfo);

	if (WaitForSingleObject(ShExecInfo.hProcess, dwMilliseconds) == WAIT_TIMEOUT)
	{
		TerminateProcess(ShExecInfo.hProcess, 0);
		return 0;
	}

	DWORD dwExitCode;
	GetExitCodeProcess(ShExecInfo.hProcess, &dwExitCode);

	return dwExitCode;
}
void ForceSetForegroundWindow(HWND hwnd)
{
	if (!hwnd)
		return;

	DWORD foregroundThreadID = GetWindowThreadProcessId(GetForegroundWindow(), NULL);
	DWORD currentThreadID = GetCurrentThreadId();

	if (foregroundThreadID != currentThreadID)
	{
		AttachThreadInput(foregroundThreadID, currentThreadID, TRUE);
		SetForegroundWindow(hwnd);
		AttachThreadInput(foregroundThreadID, currentThreadID, FALSE);
	}
	else
		SetForegroundWindow(hwnd);
}


bool ExpCheckAddress(std::string _Address)
{
	if (_Address.find("..") != std::string::npos)
		return false;
	return true;
}
