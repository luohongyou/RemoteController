// Encoding.h : Coral Remote Controller 编解码代码模板的定义。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define _CRT_SECURE_NO_DEPRECATE

#ifndef RC_ENCODING
#define RC_ENCODING

#include <string>
#include <locale.h>
#include <Windows.h>
#include <codecvt>
#include <wtypes.h>
#include <comutil.h>
#include <stdio.h>

// Hex / Base16
inline char ToHexUpper(unsigned int value);
inline char ToHexLower(unsigned int value);
inline int FromHex(unsigned int c);

std::string HexEncode(const std::string& d);
std::string HexDecode(const std::string& hex);

// URI
const char* encodeURI(const char* Str);
std::string decodeURI(std::string s);

// GUID
std::string GuidToString(const GUID& guid);

// string
std::string Num2String(int n);
int         String2Num(std::string n);
std::string WStr2Str(const std::wstring& wstr);
std::string PWSTR2string(PWSTR pwstr);
std::wstring S2WS(const std::string& s);

// UTF-8
std::string UTF8ToANSI(std::string s);
std::string UrlUTF8(char* str);
void GB2312ToUTF_8(std::string& pOut, char* pText, int pLen);
char* UTF_8ToGb2312(const char* utf8);
void Gb2312ToUnicode(WCHAR* pOut, char* gbBuffer);
void UTF_8ToUnicode(std::wstring& pOut, const std::string& pText);
void UnicodeToUTF_8(char* pOut, WCHAR* pText);
std::wstring ANSI2Unicode(const std::string& str);

// CRC32
void Init_CRC32_Tab();
unsigned int Calc_File_CRC32(const char* pFileName);

#endif // !RC_ENCODING
