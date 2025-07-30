/********************************************************************************
*                                                                               *
* NTVersion.h : Get Windows version and information. (Version 1.3)              *
*                                                                               *
* Copyright (C) 2020 - 2024 Coral Studio. All Rights Reserved.                  *
*                                                                               *
********************************************************************************/

#ifndef NTVERSION
#define NTVERSION

#pragma warning(push)
#pragma warning(disable: 4996)

#include <Windows.h>
#include <string>

// structure
struct NTVERSIONINFO                   // for GetWindowsNTVersionEx()
{
	DWORD MajorVersion;                // Major NT version
	DWORD MinorVersion;                // Minor NT version
	DWORD BuildNumber;                 // NT build number
	BOOL Is64BitSystem;                // 64-bit(true) 32-bit(false)
	BOOL IsARMSystem;                  // ARM(true) x86(false)
	BOOL IsWindowsServer;              // Server(true) Workstation(false)
	char OSName[256];                  // OS short name
	char OSFullName[256];              // OS full name
};

// function
inline bool IsWindowsNT()
{
	OSVERSIONINFOEX os;
	os.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	GetVersionEx((OSVERSIONINFO*)&os);
	return os.dwPlatformId == VER_PLATFORM_WIN32_NT;
}
inline bool Is64bitSystem()            // Only supports Windows XP / Server 2003 and higher
{
	SYSTEM_INFO si;
	GetNativeSystemInfo(&si);
	return si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
		si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64 ||
		si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_ARM64;
}
inline bool IsARMSystem()              // Only supports Windows XP / Server 2003 and higher
{
	SYSTEM_INFO si;
	GetNativeSystemInfo(&si);
	return si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_ARM ||
		si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_ARM64;
}
inline bool IsWindowsServer()          // Only supports Windows 2000 and higher
{
	OSVERSIONINFOEXW osvi
		= { sizeof(osvi), 0, 0, 0, 0, {0}, 0, 0, 0, VER_NT_WORKSTATION };
	const DWORDLONG dwlConditionMask = VerSetConditionMask(0, VER_PRODUCT_TYPE, VER_EQUAL);
	return !VerifyVersionInfoW(&osvi, VER_PRODUCT_TYPE, dwlConditionMask);
}
const char* GetWindowsNTVersion()      // Only supports Windows 2000 and higher
{
	int MajorVersion, Build;
	bool isServer;

	typedef LONG(__stdcall* fnRtlGetVersion)(PRTL_OSVERSIONINFOW lpVersionInformation);
	fnRtlGetVersion pRtlGetVersion;
	HMODULE hNtdll;
	LONG ntStatus;

	RTL_OSVERSIONINFOW VersionInformation = { 0 };

#ifdef UNICODE
	hNtdll = GetModuleHandle(L"ntdll.dll");
#else
	hNtdll = GetModuleHandle("ntdll.dll");
#endif

	if (hNtdll == NULL)
		return "Unknown Version";

	pRtlGetVersion = (fnRtlGetVersion)GetProcAddress(hNtdll, "RtlGetVersion");
	if (pRtlGetVersion == NULL)
		return "Unknown Version";

	VersionInformation.dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOW);
	ntStatus = pRtlGetVersion(&VersionInformation);

	if (ntStatus != 0)
		return "Unknown Version";

	MajorVersion = VersionInformation.dwMajorVersion;
	Build = VersionInformation.dwBuildNumber;
	isServer = IsWindowsServer();

	// NT 3.1
	if (Build < 528)
		return "Windows NT 3.1 Beta";
	else if (Build == 528)
		return "Windows NT 3.1";

	// NT 3.51
	else if (Build < 807)
		return "Windows NT 3.51 Beta";
	else if (Build == 807)
		return "Windows NT 3.51";

	// NT 4.0
	else if (Build < 1381)
		return "Windows NT 4.0 Beta";
	else if (Build == 1381)
		return "Windows NT 4.0";

	// NT 5.0
	else if (Build < 1946)
		return "Windows NT 5.0 Beta";
	else if (Build < 2195)
		return "Windows 2000 Beta";
	else if (Build == 2195)
		return "Windows 2000";

	// NT 5.1
	else if (Build < 2464)
		if (!isServer)
			return "Windows Codename Whistler Beta";
		else
			return "Windows Codename Whistler Server Beta";
	else if (Build < 2600)
		if (!isServer)
			return "Windows XP Beta";
		else
			return "Windows Server 2003 Beta";
	else if (Build == 2600)
		return "Windows XP";

	// NT 5.2 - 6.0 (Pre-Reset)
	else if (Build < 3790)
		if (!isServer)
		{
			if (MajorVersion == 5)
				return "Windows XP Beta";
			else
				return "Windows Codename Longhorn Beta";
		}
		else
		{
			if (MajorVersion == 5)
				return "Windows Server 2003 Beta";
			else
				return "Windows Codename Longhorn Server Beta";
		}
	else if (Build == 3790)
		if (!isServer)
			return "Windows XP";
		else
		{
			if (GetSystemMetrics(SM_SERVERR2) == 0)
				return "Windows Server 2003";
			else
				return "Windows Server 2003 R2";
		}
	else if (Build < 5000)
		if (!isServer)
			return "Windows Codename Longhorn Beta";
		else
			return "Windows Codename Longhorn Server Beta";

	// NT 6.0 (Post-Reset)
	else if (Build < 6000)
		if (!isServer)
			return "Windows Vista Beta";
		else
			return "Windows Server 2008 Beta";
	else if (Build == 6000)
		if (!isServer)
			return "Windows Vista";
		else
			return "Windows Server 2008";
	else if (Build == 6001)
		if (!isServer)
			return "Windows Vista SP1";
		else
			return "Windows Server 2008 SP1";
	else if (Build == 6002)
		if (!isServer)
			return "Windows Vista SP2";
		else
			return "Windows Server 2008 SP2";

	// NT 6.1
	else if (Build < 7600)
		if (!isServer)
			return "Windows 7 Beta";
		else
			return "Windows Server 2008 R2 Beta";
	else if (Build == 7600)
		if (!isServer)
			return "Windows 7";
		else
			return "Windows Server 2008 R2";
	else if (Build == 7601)
		if (!isServer)
			return "Windows 7 SP1";
		else
			return "Windows Server 2008 R2 SP1";

	// NT 6.2
	else if (Build < 9200)
		if (!isServer)
			return "Windows 8 Beta";
		else
			return "Windows Server 2012 Beta";
	else if (Build == 9200)
		if (!isServer)
			return "Windows 8";
		else
			return "Windows Server 2012";

	// NT 6.3
	else if (Build < 9600)
		if (!isServer)
			return "Windows 8.1 Preview";
		else
			return "Windows Server 2012 R2 Preview";
	else if (Build == 9600)
		if (!isServer)
			return "Windows 8.1";
		else
			return "Windows Server 2012 R2";

	// NT 6.4 - 10.0.10240
	else if (Build < 10074)
		if (!isServer)
			return "Windows 10 Technical Preview";
		else
			return "Windows Server 2016 Technical Preview";
	else if (Build < 10240)
		if (!isServer)
			return "Windows 10 Insider Preview (TH1)";
		else
			return "Windows Server 2016 Technical Preview";
	else if (Build == 10240)
		if (!isServer)
			return "Windows 10 (Version 1507)";
		else
			return "Windows Server 2016 Technical Preview";

	// NT 10.0.10586
	else if (Build < 10586)
		if (!isServer)
			return "Windows 10 Insider Preview (TH2)";
		else
			return "Windows Server 2016 Technical Preview";
	else if (Build == 10586)
		if (!isServer)
			return "Windows 10 (Version 1511)";
		else
			return "Windows Server 2016 Technical Preview";

	// NT 10.0.14393
	else if (Build < 14393)
		if (!isServer)
			return "Windows 10 Insider Preview (RS1)";
		else
			return "Windows Server 2016 Technical Preview";
	else if (Build == 14393)
		if (!isServer)
			return "Windows 10 (Version 1607)";
		else
			return "Windows Server 2016";

	// NT 10.0.15063
	else if (Build < 15063)
		if (!isServer)
			return "Windows 10 Insider Preview (RS2)";
		else
			return "Windows Server 1703 Insider Preview";
	else if (Build == 15063)
		if (!isServer)
			return "Windows 10 (Version 1703)";
		else
			return "Windows Server 1703";

	// NT 10.0.16299
	else if (Build < 16299)
		if (!isServer)
			return "Windows 10 Insider Preview (RS3)";
		else
			return "Windows Server 1709 Insider Preview";
	else if (Build == 16299)
		if (!isServer)
			return "Windows 10 (Version 1709)";
		else
			return "Windows Server 1709";

	// NT 10.0.17134
	else if (Build < 17134)
		if (!isServer)
			return "Windows 10 Insider Preview (RS4)";
		else
			return "Windows Server 1803 Insider Preview";
	else if (Build == 17134)
		if (!isServer)
			return "Windows 10 (Version 1803)";
		else
			return "Windows Server 1803";

	// NT 10.0.17763
	else if (Build < 17763)
		if (!isServer)
			return "Windows 10 Insider Preview (RS5)";
		else
			return "Windows Server 2019 Insider Preview";
	else if (Build == 17763)
		if (!isServer)
			return "Windows 10 (Version 1809)";
		else
			return "Windows Server 2019";

	// NT 10.0.18362
	else if (Build < 18362)
		if (!isServer)
			return "Windows 10 Insider Preview (19H1)";
		else
			return "Windows Server 1903 Insider Preview";
	else if (Build == 18362)
		if (!isServer)
			return "Windows 10 (Version 1903)";
		else
			return "Windows Server 1903";

	// NT 10.0.18363
	else if (Build == 18363)
		if (!isServer)
			return "Windows 10 (Version 1909)";
		else
			return "Windows Server 1909";

	// NT 10.0.19041
	else if (Build < 19041)
		if (!isServer)
			return "Windows 10 Insider Preview (20H1)";
		else
			return "Windows Server 2004 Insider Preview";
	else if (Build == 19041)
		if (!isServer)
			return "Windows 10 (Version 2004)";
		else
			return "Windows Server 2004";
			
	// NT 10.0.19042
	else if (Build == 19042)
		if (!isServer)
			return "Windows 10 (Version 20H2)";
		else
			return "Windows Server 20H2";

	// NT 10.0.19043
	else if (Build == 19043)
	if (!isServer)
		return "Windows 10 (Version 21H1)";
	else
		return "Windows Server 21H1";

	// NT 10.0.19044
	else if (Build == 19044)
	if (!isServer)
		return "Windows 10 (Version 21H2)";
	else
		return "Windows Server 21H2";
		
	// NT 10.0.19045
	else if (Build == 19045)
	if (!isServer)
		return "Windows 10 (Version 22H2)";
	else
		return "Windows Server 22H2";
		
	// NT 10.0.20348
	else if (Build < 20348)
		if (!isServer)
			return "Windows 11 Insider Preview";
		else
			return "Windows Server 2022 Insider Preview";
	else if (Build == 20348)
		if (!isServer)
			return "Windows 10 (Version 21H2)";
		else
			return "Windows Server 2022";

	// NT 10.0.----- (Windows 10)
	else if (Build <= 21390)
		if (!isServer)
			return "Windows 10 Insider Preview";
		else
			return "Windows Server Insider Preview";
		
	// NT 10.0.22000
	else if (Build < 22000)
		if (!isServer)
			return "Windows 11 Insider Preview (21H2)";
		else
			return "Windows Server 21H2 Insider Preview";
	else if (Build == 22000)
		if (!isServer)
			return "Windows 11 (Version 21H2)";
		else
			return "Windows Server 21H2";
			
	// NT 10.0.22621
	else if (Build < 22621)
		if (!isServer)
			return "Windows 11 Insider Preview (22H2)";
		else
			return "Windows Server 22H2 Insider Preview";
	else if (Build == 22621)
		if (!isServer)
			return "Windows 11 (Version 22H2)";
		else
			return "Windows Server 22H2";
			
	// NT 10.0.22631
	else if (Build < 22631)
		if (!isServer)
			return "Windows 11 Insider Preview (23H2)";
		else
			return "Windows Server 23H2 Insider Preview";
	else if (Build == 22631)
		if (!isServer)
			return "Windows 11 (Version 23H2)";
		else
			return "Windows Server 23H2";
			
	// NT 10.0.25398
	else if (Build < 25398)
		if (!isServer)
			return "Windows 11 Insider Preview (23H2)";
		else
			return "Windows Server 23H2 Insider Preview";
	else if (Build == 25398)
		if (!isServer)
			return "Windows 11 Insider Preview (23H2)";
		else
			return "Windows Server 23H2";

	// NT 10.0.26100
	else if (Build < 26100)
		if (!isServer)
			return "Windows 11 Insider Preview (24H2)";
		else
			return "Windows Server 2025 Insider Preview";
	else if (Build == 26100)
		if (!isServer)
			return "Windows 11 (Version 24H2)";
		else
			return "Windows Server 2025";
			
	// NT 10.0.-----
	else if (Build > 26100)
		if (!isServer)
			return "Windows 11 Insider Preview (24H2)";
		else
			return "Windows Server vNext Insider Preview";

	else
		return "Unknown Version";
}
BOOL GetWindowsNTVersionEx(NTVERSIONINFO& Info, bool IsCPUArchitecture)
// Only supports Windows XP / Server 2003 and higher
{
	typedef LONG(__stdcall* fnRtlGetVersion)(PRTL_OSVERSIONINFOW lpVersionInformation);
	fnRtlGetVersion pRtlGetVersion;
	HMODULE hNtdll;
	LONG ntStatus;

	RTL_OSVERSIONINFOW VersionInformation = { 0 };

#ifdef UNICODE
	hNtdll = GetModuleHandle(L"ntdll.dll");
#else
	hNtdll = GetModuleHandle("ntdll.dll");
#endif // UNICODE

	if (hNtdll == NULL)
		return FALSE;

	pRtlGetVersion = (fnRtlGetVersion)GetProcAddress(hNtdll, "RtlGetVersion");
	if (pRtlGetVersion == NULL)
		return FALSE;

	VersionInformation.dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOW);
	ntStatus = pRtlGetVersion(&VersionInformation);

	if (ntStatus != 0)
		return FALSE;

	Info.MajorVersion = VersionInformation.dwMajorVersion;
	Info.MinorVersion = VersionInformation.dwMinorVersion;
	Info.BuildNumber = VersionInformation.dwBuildNumber;
	Info.IsWindowsServer = IsWindowsServer();
	Info.Is64BitSystem = Is64bitSystem();
	Info.IsARMSystem = IsARMSystem();

	std::string OSName;
	OSName = "";

	if (Info.BuildNumber <= 528)
		OSName = "Windows NT 3.1";
	else if (Info.BuildNumber <= 807)
		OSName = "Windows NT 3.51";
	else if (Info.BuildNumber <= 1381)
		OSName = "Windows NT 4.0";
	else if (Info.BuildNumber <= 2195)
		OSName = "Windows 2000";
	else if (Info.BuildNumber <= 6002)
		if (Info.MajorVersion == 5)
		{
			if (!Info.IsWindowsServer)
				OSName = "Windows XP";
			else
				if (GetSystemMetrics(SM_SERVERR2) == 0)
					OSName = "Windows Server 2003";
				else
					OSName = "Windows Server 2003 R2";
		}
		else
		{
			if (!Info.IsWindowsServer)
				OSName = "Windows Vista";
			else
				OSName = "Windows Server 2008";
		}
	else if (Info.BuildNumber <= 7601)
		if (!Info.IsWindowsServer)
			OSName = "Windows 7";
		else
			OSName = "Windows Server 2008 R2";
	else if (Info.BuildNumber <= 9200)
		if (!Info.IsWindowsServer)
			OSName = "Windows 8";
		else
			OSName = "Windows Server 2012";
	else if (Info.BuildNumber <= 9600)
		if (!Info.IsWindowsServer)
			OSName = "Windows 8.1";
		else
			OSName = "Windows Server 2012 R2";
	else if (Info.BuildNumber <= 21390)
		if (!Info.IsWindowsServer)
			OSName = "Windows 10";
		else
		{
			if (Info.BuildNumber <= 14393)
				OSName = "Windows Server 2016";
			else if (Info.BuildNumber <= 17763)
				OSName = "Windows Server 2019";
			else
				OSName = "Windows Server 2022";
		}
	else
		if (!Info.IsWindowsServer)
			OSName = "Windows 11";
		else
			OSName = "Windows Server 2025";
	if (IsCPUArchitecture)
		if (Info.Is64BitSystem && !Info.IsARMSystem)
			OSName += " x64";
		else if (Info.Is64BitSystem && Info.IsARMSystem)
			OSName += " ARM64";
		else if (!Info.Is64BitSystem && !Info.IsARMSystem)
			OSName += " x86";
		else
			OSName += " ARM";
	strcpy(Info.OSName, OSName.c_str());

	OSName = GetWindowsNTVersion();
	if (IsCPUArchitecture)
		if (Info.Is64BitSystem && !Info.IsARMSystem)
			OSName += " x64";
		else if (Info.Is64BitSystem && Info.IsARMSystem)
			OSName += " ARM64";
		else if (!Info.Is64BitSystem && !Info.IsARMSystem)
			OSName += " x86";
		else
			OSName += " ARM";
	strcpy(Info.OSFullName, OSName.c_str());

	return TRUE;
}

#pragma warning(pop)

#endif // !NTVERSION
