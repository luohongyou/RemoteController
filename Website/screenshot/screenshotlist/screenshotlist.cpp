// screenshotlist.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/screenshot/screenshotlist.exe"

#include <iostream>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\");

    HTMLFrame HTML;
    HTML.Register(PT_FRAME, ADDRESS, "screenshotlist", "screenshotlist", "", true);

	std::string Code = R"(
<center>
<table class="table table-hover">
<tr>
<th>缩略图</th>
<th>时间</th>
<th>操作</th>
</tr>
)";
	int cnt = 0;
	std::ifstream fin;
	std::string FileName;
	fin.open(HTML.User.Data("screenshotlist"));
	if (fin.fail())
		fin.close();
	else
	{
		fin >> cnt;
		for (int i = 1; i <= cnt; ++i)
		{
			fin >> FileName;

			std::string data1, data2, Time;
			int year, month, day, hour, min, sec;

			std::string tempFName = FileName.substr(FileName.find('-', 11) + 1);
			sscanf_s(tempFName.c_str(), "%d-%d-%d-%d-%d-%d", &year, &month, &day, &hour, &min, &sec);
			std::stringstream tempIO;
			tempIO << year
				<< '/' << (month < 10 ? "0" : "") << month
				<< '/' << (day < 10 ? "0" : "") << day
				<< ' ' << (hour < 10 ? "0" : "") << hour
				<< ':' << (min < 10 ? "0" : "") << min
				<< ':' << (sec < 10 ? "0" : "") << sec;

			tempIO >> data1 >> data2;
			Time = data1 + ' ' + data2;

			Code += "<tr>\n";
			Code += "<td> <img src=\"\\storage\\screenshot\\" + FileName + ".jpg\" width=\"80 % \"/> </td>\n";
			Code += "<td>" + Time + "</td>\n";
			Code +=
				R"(<td><div class="btn-group">
	<a href="/storage/screenshot/)" + FileName + R"(.jpg" class="portfolio-lightbox"><i class="bi bi-eye"></i></a>&nbsp;&nbsp;
	<!-- <a href="/storage/screenshot/)" + FileName + R"(.jpg" target="_blank"><i class="bi bi-eye"></i></a>&nbsp;&nbsp; -->
	<a href="/storage/screenshot/)" + FileName + R"(.jpg" download=")" + FileName + R"("><i class="bi bi-download"></i></a>&nbsp;&nbsp;
	<a href="javascript:void(0);" onclick="Delete(')" + FileName + R"(.jpg') "><i class="bi bi-trash3"></i></a>
</div></td>
)";
			Code += "</tr>\n";
		}
		fin.close();
	}
	Code += R"(
</table>
</center>
)";

    AJAXOutput(Code);
    return 0;
}

