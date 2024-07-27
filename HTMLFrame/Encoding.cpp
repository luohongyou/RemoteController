// Encoding.cpp : Coral Remote Controller ��������ģ���ʵ�֡�

// ��Ȩ������
// Copyright (C) 2020-2024 Coral Studio. All Rights Reserved.

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
	int s;
	std::stringstream tempIO;
	tempIO << n;
	tempIO >> s;
	return s;
	return 0;
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
			// 1�ֽ�UTF-8����
			unicodeChar = pText[i];
		}
		else if ((pText[i] & 0xE0) == 0xC0 && i + 1 < pText.length() && (pText[i + 1] & 0xC0) == 0x80) {
			// 2�ֽ�UTF-8����
			unicodeChar = ((pText[i] & 0x1F) << 6) | (pText[i + 1] & 0x3F);
			++i;
		}
		else if ((pText[i] & 0xF0) == 0xE0 && i + 2 < pText.length() && (pText[i + 1] & 0xC0) == 0x80 && (pText[i + 2] & 0xC0) == 0x80) {
			// 3�ֽ�UTF-8����
			unicodeChar = ((pText[i] & 0x0F) << 12) | ((pText[i + 1] & 0x3F) << 6) | (pText[i + 2] & 0x3F);
			i += 2;
		}
		else {
			// �����UTF-8����
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