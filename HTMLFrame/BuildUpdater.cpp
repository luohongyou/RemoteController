// BuildUpdater.cpp : 更新Build信息。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#include <iostream>
#include <fstream>
#include <Windows.h>
using namespace std;
int main()
{
	char strModule[256];
	GetModuleFileNameA(NULL, strModule, 256);
	std::string a;
	a.assign(strModule);
	a.append("\\..\\");
	SetCurrentDirectoryA(a.c_str());

	string temp;
	int build;
	ifstream fin("Build.h");
	fin >> temp >> temp >> build;
	fin.close();

	ofstream fout("Build.h");
	fout << "// Build " << build + 1;
	fout << R"(
// Build.h : 定义程序Build信息

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#ifndef _RCBUILD_
#define _RCBUILD_

)";
	fout << "#define PROGRAM_BUILD " << build + 1 << endl;
	fout << "#define PROGRAM_BUILD_S \"" << build + 1 << "\"" << endl;
	fout << endl;
	fout << "#endif" << endl;
	fout.close();
	return 0;
}
