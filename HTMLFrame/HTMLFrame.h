// HTMLFrame.h : Coral Remote Controller HTML�����ͺ����Ķ���

// ��Ȩ������
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#ifndef RC_HTMLFRAME
#define RC_HTMLFRAME

#define BETA

#define VERSION "4.0 Beta 7"
#define BRANCH "rc_vnext_beta"
#define OOBEVERSION 12

#pragma warning(disable: 4251)
#include "cgicc/Cgicc.h"
#pragma warning(default: 4251)

#include <Windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <comutil.h>

#include "Build.h"
#include "CUser.h"
#include "Functions.h"
#include "Encoding.h"

// Ԥ����Ĳ���
// ҳ������
#define PT_INDEX     0
#define PT_NOCRUMB   1
#define PT_FRAME     2
#define PT_EMPTY     3
// ��־����
#define LL_INFO      0
#define LL_WARNING   1
#define LL_ERROR     2
#define LL_DEBUG     5

// HTML���������
class HTMLFrame
{
private:

	// HTML��ҳ��Ϣ
	int PageType;                        // ҳ������

	std::string Link;                    // ��ҳ����
	std::string BrowserTitle;            // HTML <title> ��ǩ����
	std::string BreadCrumb;              // BreadCrumbҳ��λ��
	std::string PageTitle;               // ҳ�渱����
	std::string CustomCode;              // �Զ������

	std::string Token;                   // Token��Ϣ
	int IsCheckUser;                     // �Ƿ��¼����

	// HTMLFrame������
	std::string FeatureList;             // ���ܲ˵�
	int IsLog;                           // ��־�Ƿ��

public:
	CUser User;                          // �û���Ϣ
	cgicc::Cgicc cgi;                    // CGICC��

	// ���캯��
	HTMLFrame();

	// HTML����
	void Register                        // ע��HTML��Ϣ
	(int _PageType,
		std::string _Link, std::string _PageTitle, std::string _BrowserTitle, std::string _BreadCrumb,
		int _IsCheckUser);
	void ManageInit();                   // ��̨����ҳ���ʼ��
	void Custom(std::string _Code);      // �����Զ������
	void CustomFeatureList(
		std::string _Code);              // �����Զ��幦�ܲ˵�
	void Log(std::string Event, 
		std::string Module, int Level);  // ���������־
	void Output();                       // �������

private:
	std::string OutputFrameCode(
		int _PageType, 
		std::string _CustomCode);        // ���HTML����

public:
	class HTMLToolCards
	{
	public:
		int size;
		std::string Title[30];
		std::string Description[30];
		std::string Link[30];
		std::string Icon[30];

		HTMLToolCards();
		void Add(std::string _Title, std::string _Description, std::string _Link, std::string _Icon);
		std::string Output();
	};
};

#endif // !RC_HTMLFRAME