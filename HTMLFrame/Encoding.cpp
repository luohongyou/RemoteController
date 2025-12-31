// Encoding.cpp : Coral Remote Controller 编解码代码模板的实现。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#include "pch.h"
#include "Encoding.h"
#include "HTMLFrame.h"

#pragma comment(lib, "comsuppw.lib")

// Hex / Base16
inline char ToHexUpper(unsigned int value)
{
	return "0123456789ABCDEF"[value & 0xF];
}
inline char ToHexLower(unsigned int value)
{
	return "0123456789abcdef"[value & 0xF];
}
inline int FromHex(unsigned int c)
{
	return ((c >= '0') && (c <= '9')) ? int(c - '0') :
		((c >= 'A') && (c <= 'F')) ? int(c - 'A' + 10) :
		((c >= 'a') && (c <= 'f')) ? int(c - 'a' + 10) :
		/* otherwise */              -1;
}
std::string HexEncode(const std::string& d)
{
	std::string hex;
	hex.resize(d.size() * 2);
	char* pHexData = (char*)hex.data();
	const unsigned char* pSrcData = (const unsigned char*)d.data();
	for (int i = 0; i < d.size(); i++)
	{
		pHexData[i * 2] = ToHexLower(pSrcData[i] >> 4);
		pHexData[i * 2 + 1] = ToHexLower(pSrcData[i] & 0xf);
	}

	return hex;
}
std::string HexDecode(const std::string& hex)
{
	std::string res;
	res.resize(hex.size() + 1 / 2);
	unsigned char* pResult = (unsigned char*)res.data() + res.size();
	bool odd_digit = true;

	for (int i = hex.size() - 1; i >= 0; i--)
	{
		unsigned char ch = unsigned char(hex.at(i));
		int tmp = FromHex(ch);
		if (tmp == -1)
			continue;
		if (odd_digit) {
			--pResult;
			*pResult = tmp;
			odd_digit = false;
		}
		else {
			*pResult |= tmp << 4;
			odd_digit = true;
		}
	}

	res.erase(0, pResult - (unsigned char*)res.data());

	return res;
}

// URI
const char* encodeURI(const char* Str)
{
	wchar_t* Bufw = NULL;
	char* Bufc = NULL;
	static char RTV[512000] = { 0 };
	long needSize = MultiByteToWideChar(CP_ACP, NULL, Str, -1, NULL, 0);
	if (0 == needSize)
		return NULL;

	Bufw = new wchar_t[needSize];
	if (NULL == Bufw)
	{
		delete[] Bufw;
		return NULL;
	}

	memset(Bufw, 0x0, needSize * 2);
	MultiByteToWideChar(CP_ACP, NULL, Str, -1, Bufw, needSize);

	needSize = WideCharToMultiByte(CP_UTF8, NULL, Bufw, -1, NULL, 0, NULL, NULL);
	if (0 == needSize)
	{
		delete[] Bufw;
		return NULL;
	}

	Bufc = new char[needSize];
	if (NULL == Bufc)
	{
		delete[] Bufw;
		delete[] Bufc;
		return NULL;
	}

	memset(Bufc, 0x0, needSize);
	WideCharToMultiByte(CP_UTF8, NULL, Bufw, -1, Bufc, needSize, NULL, NULL);

	unsigned char* pWork = (unsigned char*)Bufc;
	memset(RTV, 0x0, sizeof(RTV));
	if (strlen(Bufc) > 512000)
	{
		delete[] Bufw;
		delete[] Bufc;
		return NULL;
	}
	while (*pWork != 0x0)
	{
		if (*pWork != '!' && *pWork != '@' && *pWork != '#' &&
			*pWork != '$' && *pWork != '&' && *pWork != '*' &&
			*pWork != '(' && *pWork != ')' && *pWork != '=' &&
			*pWork != ':' && *pWork != '/' && *pWork != ';' &&
			*pWork != '?' && *pWork != '+' && *pWork != '\'' &&
			*pWork != '.' && *pWork != ',')
		{
			sprintf(RTV + strlen(RTV), "%%%2X", *pWork);
		}
		else
		{
			sprintf(RTV + strlen(RTV), "%c", *pWork);
		}
		pWork++;
	}
	if (NULL != Bufw)
	{
		delete[] Bufw;
		Bufw = NULL;
	}
	if (NULL != Bufc)
	{
		delete[] Bufc;
		Bufc = NULL;
	}
	return RTV;
}
std::string decodeURI(std::string s)
{
	const int MAXL = 512000;
	char ss[MAXL];
	char* p;
	int i, v, r, n;
	
	p = (char*)s.c_str();
	i = 0;
	while (1) {
		r = sscanf_s(p, "%%%2X%n", &v, &n);
		if (1 == r) {
			ss[i] = (char)v;
			p += n;
		}
		else if (0 == r) {
			ss[i] = p[0];
			p++;
		}
		else break;
		i++;
		if (i >= MAXL - 1) break;
	}
	ss[i] = 0;
	return ss;
}

// GUID
std::string GuidToString(const GUID& guid)
{
	char buf[64] = { 0 };
	sprintf_s(buf, sizeof(buf),
		"%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
	return std::string(buf);
}

// string
std::string Num2String(int n)
{
	std::string s;
	std::stringstream tempIO;
	tempIO << n;
	tempIO >> s;
	return s;
}
int String2Num(std::string n)
{
	return std::stoi(n);
}
std::string UTF8ToANSI(std::string s)
{
	BSTR bstrWide;
	char* pszAnsi;
	int nLength;
	const char* pszCode = s.c_str();

	nLength = MultiByteToWideChar(CP_UTF8, 0, pszCode, strlen(pszCode) + 1, NULL, NULL);
	bstrWide = SysAllocStringLen(NULL, nLength);

	MultiByteToWideChar(CP_UTF8, 0, pszCode, strlen(pszCode) + 1, bstrWide, nLength);

	nLength = WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, NULL, 0, NULL, NULL);
	pszAnsi = new char[nLength];

	WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, pszAnsi, nLength, NULL, NULL);
	SysFreeString(bstrWide);

	std::string r(pszAnsi);
	delete[] pszAnsi;
	return r;
}
std::string WStr2Str(const std::wstring& wstr) 
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.to_bytes(wstr);
}
std::string PWSTR2string(PWSTR pwstr)
{
	if (pwstr == nullptr) {
		return std::string();
	}

	int size = WideCharToMultiByte(CP_ACP, 0, pwstr, -1, nullptr, 0, nullptr, nullptr);
	std::string str(size, 0);
	WideCharToMultiByte(CP_ACP, 0, pwstr, -1, &str[0], size, nullptr, nullptr);

	return str;
}
std::wstring S2WS(const std::string& s)
{
	std::string curLocale = setlocale(LC_ALL, "");
	const char* _Source = s.c_str();
	size_t _Dsize = mbstowcs(NULL, _Source, 0) + 1;
	wchar_t* _Dest = new wchar_t[_Dsize];
	wmemset(_Dest, 0, _Dsize);
	mbstowcs(_Dest, _Source, _Dsize);
	std::wstring result = _Dest;
	delete[]_Dest;
	setlocale(LC_ALL, curLocale.c_str());
	return result;
}

std::string UrlUTF8(char* str)
{
	std::string tt;
	std::string dd;
	GB2312ToUTF_8(tt, str, strlen(str));
	int len = tt.length();
	for (int i = 0; i < len; i++)
	{
		if (isalnum((BYTE)tt.at(i)))
		{
			char tempbuff[2] = { 0 };
			sprintf_s(tempbuff, "%c", (BYTE)tt.at(i));
			dd.append(tempbuff);
		}
		else if (isspace((BYTE)tt.at(i)))
		{
			dd.append("+");
		}
		else
		{
			char tempbuff[4];
			sprintf_s(tempbuff, "%%%X%X", ((BYTE)tt.at(i)) >> 4, ((BYTE)tt.at(i)) % 16);
			dd.append(tempbuff);
		}

	}
	return dd;
}
void GB2312ToUTF_8(std::string& pOut, char* pText, int pLen)
{
	char buf[4];
	memset(buf, 0, 4);

	pOut.clear();

	int i = 0;
	while (i < pLen)
	{
		if (pText[i] >= 0)
		{
			char asciistr[2] = { 0 };
			asciistr[0] = (pText[i++]);
			pOut.append(asciistr);
		}
		else
		{
			WCHAR pbuffer;
			Gb2312ToUnicode(&pbuffer, pText + i);

			UnicodeToUTF_8(buf, &pbuffer);

			pOut.append(buf);

			i += 2;
		}
	}

	return;
}
char* UTF_8ToGb2312(const char* utf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}
void Gb2312ToUnicode(WCHAR* pOut, char* gbBuffer)
{
	::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, gbBuffer, 2, pOut, 1);
	return;
}
void UTF_8ToUnicode(std::wstring& pOut, const std::string& pText) 
{
	pOut = L"";

	for (size_t i = 0; i < pText.length(); ++i) {
		wchar_t unicodeChar = 0;

		if ((pText[i] & 0x80) == 0) {
			// 1字节UTF-8编码
			unicodeChar = pText[i];
		}
		else if ((pText[i] & 0xE0) == 0xC0 && i + 1 < pText.length() && (pText[i + 1] & 0xC0) == 0x80) {
			// 2字节UTF-8编码
			unicodeChar = ((pText[i] & 0x1F) << 6) | (pText[i + 1] & 0x3F);
			++i;
		}
		else if ((pText[i] & 0xF0) == 0xE0 && i + 2 < pText.length() && (pText[i + 1] & 0xC0) == 0x80 && (pText[i + 2] & 0xC0) == 0x80) {
			// 3字节UTF-8编码
			unicodeChar = ((pText[i] & 0x0F) << 12) | ((pText[i + 1] & 0x3F) << 6) | (pText[i + 2] & 0x3F);
			i += 2;
		}
		else {
			// 错误的UTF-8编码
			unicodeChar = pText[i];
		}

		pOut += unicodeChar;
	}
}
void UnicodeToUTF_8(char* pOut, WCHAR* pText)
{
	char* pchar = (char*)pText;

	pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
	pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
	pOut[2] = (0x80 | (pchar[0] & 0x3F));

	return;
}
std::wstring ANSI2Unicode(const std::string& str) 
{
	int len = str.size();
	int unicode_len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	wchar_t* unicode_p = new wchar_t[unicode_len + 1];
	memset(unicode_p, 0, (unicode_len) * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, (LPWSTR)unicode_p, unicode_len);
	std::wstring str_w;
	str_w = (wchar_t*)unicode_p;
	delete unicode_p;
	return str_w;
}
bool IsUTF8(const void* pBuffer, int size)
{
	bool IsUTF8 = false;
	unsigned char* start = (unsigned char*)pBuffer;
	unsigned char* end = (unsigned char*)pBuffer + size;
	while (start < end)
	{
		if (*start < 0x80) // (10000000): 值小于0x80的为ASCII字符 
		{
			start++;
		}
		else if (*start < (0xC0)) // (11000000): 值介于0x80与0xC0之间的为无效UTF-8字符 
		{
			IsUTF8 = false;
			break;
		}
		else if (*start < (0xE0)) // (11100000): 此范围内为2字节UTF-8字符 
		{
			IsUTF8 = true;
			if (start >= end - 1)
				break;
			if ((start[1] & (0xC0)) != 0x80)
			{
				IsUTF8 = false;
				break;
			}
			start += 2;
		}
		else if (*start < (0xF0)) // (11110000): 此范围内为3字节UTF-8字符 
		{
			IsUTF8 = true;
			if (start >= end - 2) break;
			if ((start[1] & (0xC0)) != 0x80 || (start[2] & (0xC0)) != 0x80)
			{
				IsUTF8 = false; break;
			}
			start += 3;
		}
		else if (*start < (0xF8)) // (11111000): 此范围内为4字节UTF-8字符 
		{
			IsUTF8 = true;
			if (start >= end - 3) break;
			if ((start[1] & (0xC0)) != 0x80 || (start[2] & (0xC0)) != 0x80 || (start[3] & (0xC0)) != 0x80)
			{
				IsUTF8 = false; break;
			}
			start += 4;
		}
		else
		{
			IsUTF8 = false;
			break;
		}
	}
	return IsUTF8;
}

// CRC32
#define Poly 0xEDB88320L
static unsigned int crc_tab32[256];
void Init_CRC32_Tab()
{
	int i, j;
	unsigned int crc;

	for (i = 0; i < 256; i++)
	{
		crc = (unsigned long)i;
		for (j = 0; j < 8; j++)
		{
			if (crc & 0x00000001L)
				crc = (crc >> 1) ^ Poly;
			else
				crc = crc >> 1;
		}
		crc_tab32[i] = crc;
	}
}
unsigned int Get_CRC32(unsigned int crcinit, unsigned char* bs, unsigned int bssize)
{
	unsigned int crc = crcinit ^ 0xffffffff;

	while (bssize--)
		crc = (crc >> 8) ^ crc_tab32[(crc & 0xff) ^ *bs++];

	return crc ^ 0xffffffff;
}
const unsigned int size = 16 * 1024;
static unsigned char crcbuf[size];
unsigned int Calc_File_CRC32(const char* pFileName)
{
	HANDLE hFile = CreateFileA(pFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return 0;

	DWORD rdlen;
	unsigned int crc = 0;
	memset(crcbuf, 0, size);

	while (ReadFile(hFile, crcbuf, size, &rdlen, NULL), rdlen)
		crc = Get_CRC32(crc, crcbuf, rdlen);

	CloseHandle(hFile);

	return crc;
}
