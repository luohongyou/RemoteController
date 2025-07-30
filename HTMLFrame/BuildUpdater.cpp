// BuildUpdater.cpp : ����Build��Ϣ��

// ��Ȩ������
// Copyright (C) 2020-2024 Coral Studio. All Rights Reserved.

#include <iostream>
#include <fstream>
#include <Windows.h>
using namespace std;
int main()
{
	char strModule[256];
	GetModuleFileName(NULL, strModule, 256);
	std::string a;
	a.assign(strModule);
	a.append("\\..\\");
	SetCurrentDirectory(a.c_str());

	string temp;
	int build;
	ifstream fin("Build.h");
	fin >> temp >> temp >> build;
	fin.close();

	ofstream fout("Build.h");
	fout << "// Build " << build + 1;
	fout << R"(
// Build.h : �������Build��Ϣ

// ��Ȩ������
// Copyright (C) 2020-2024 Coral Studio. All Rights Reserved.

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
