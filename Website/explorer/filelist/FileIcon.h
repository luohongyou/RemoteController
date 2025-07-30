// FileIcon.h : Coral Remote Controller 文件管理图标载入模块

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#ifndef RC_EXPLORERICON
#define RC_EXPLORERICON

#include <string>
#include <fstream>
using namespace std;
string Name[60], FriendlyName[60];
string Extension[60][55];
int NameTot, ExtTot[60];

void LoadFileIconInfo()
{
	ifstream fin("explorer\\Resources\\IconList.ini");
	while (!fin.eof())
	{
		fin >> Name[++NameTot] >> ExtTot[NameTot];
		fin.get();
		getline(fin, FriendlyName[NameTot]);
		for (int i = 1; i <= ExtTot[NameTot]; i++)
			fin >> Extension[NameTot][i];
	}
	fin.close();
}

void GetFileInfo(string FileName, string& _FriendlyName, string& _Icon)
{
	string Ext;
	bool flag = 0;
	for (int i = FileName.size() - 1; i >= 0; i--)
		if (FileName[i] == '.')
		{
			Ext = FileName.substr(i + 1);
			flag = 1;
			break;
		}
	if (!flag)
	{
		_FriendlyName = "文件";
		_Icon = "Unknown.png";
		return;
	}
	string EXT, ext;
	EXT = ext = Ext;
	for (int i = 0; i < Ext.size(); i++)
		if ('a' <= Ext[i] && Ext[i] <= 'z')
			EXT[i] -= ('a' - 'A');
		else if ('A' <= Ext[i] && Ext[i] <= 'Z')
			ext[i] += ('a' - 'A');

	for (int i = 1; i <= NameTot; i++)
	{
		flag = 0;
		for (int j = 1; j <= ExtTot[i]; j++)
			if (ext == Extension[i][j])
				flag = 1;
		if (flag)
		{
			_FriendlyName = FriendlyName[i];
			_Icon = Name[i] + ".png";
			return;
		}
	}
	_FriendlyName = EXT + " 文件";
	_Icon = "Unknown.png";
	return;
}

#endif