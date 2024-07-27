// HTMLFrame.h : Coral Remote Controller HTML输出类和函数的定义

// 版权声明：
// Copyright (C) 2020-2024 Coral Studio. All Rights Reserved.

#ifndef RC_HTMLFRAME
#define RC_HTMLFRAME

// #define BETA

#define VERSION "3.0.0"
#define BRANCH "rc_release"
#define OOBEVERSION 5

#pragma warning(disable: 4251)
#include "cgicc/CgiDefs.h"
#include "cgicc/Cgicc.h"
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTMLClasses.h"
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

// 预定义的参数
// 页面类型
#define PT_INDEX     0
#define PT_NOCRUMB   1
#define PT_FRAME     2
#define PT_EMPTY     3
// 日志类型
#define LL_INFO      0
#define LL_WARNING   1
#define LL_ERROR     2

// HTML代码输出类
class HTMLFrame
{
private:

	// HTML网页信息
	int PageType;                        // 页面类型

	std::string Link;                    // 网页链接
	std::string BrowserTitle;            // HTML <title> 标签内容
	std::string BreadCrumb;              // BreadCrumb页面位置
	std::string PageTitle;               // 页面副标题
	std::string CustomCode;              // 自定义代码

	std::string Token;                   // Token信息
	int IsCheckUser;                     // 是否登录访问

	// HTMLFrame类数据
	std::string FeatureList;             // 功能菜单
	int IsLog;                           // 日志是否打开

public:
	CUser User;                          // 用户信息
	cgicc::Cgicc cgi;                    // CGICC库

	// 构造函数
	HTMLFrame();

	// HTML操作
	void Register                        // 注册HTML信息
	(int _PageType,
		std::string _Link, std::string _PageTitle, std::string _BrowserTitle, std::string _BreadCrumb,
		int _IsCheckUser);
	void ManageInit();                   // 后台管理页面初始化
	void Custom(std::string _Code);      // 设置自定义代码
	void CustomFeatureList(
		std::string _Code);              // 设置自定义功能菜单
	void Log(std::string Event, 
		std::string Module, int Level);  // 输出基本日志
	void Output();                       // 输出代码

private:
	std::string OutputFrameCode(
		int _PageType, 
		std::string _CustomCode);        // 输出HTML代码
};

class HTMLToolCards
{
public:
	int size;
	std::string Title[30];
	std::string Description[30];
	std::string Link[30];

	HTMLToolCards();
	void Add(std::string _Title, std::string _Description, std::string _Link);
	std::string Output();
};

#endif // !RC_HTMLFRAME