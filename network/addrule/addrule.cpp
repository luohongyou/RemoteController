// addrule.cpp : Coral Remote Controller 网络设置 添加规则模块

// 版权声明：
// Copyright (C) 2020-2021 Coral Studio. All Rights Reserved.

#include <iostream>
#include <Windows.h>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;
string List[10001];
int main(int argc, char* argv[])
{
    puts("Content-type: text/html");
    string Code1 = R"(
<!DOCTYPE html>
<html lang="zh">
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=gb2312">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>addrule - Coral Remote Controller</title>
    <!-- Bootstrap -->
    <link href="/css/bootstrap-4.4.1.css" rel="stylesheet">
  </head>
  <body>
    <script>
)";
    string Code2 = R"(
window.location.href = "/network";
    </script>
    <!-- jQuery (necessary for Bootstrap's JavaScript plugins) --> 
    <script src="/js/jquery-3.4.1.min.js"></script> 
    <!-- Include all compiled plugins (below), or include individual files as needed --> 
    <script src="/js/popper.min.js"></script> 
    <script src="/js/bootstrap-4.4.1.js"></script>
  </body>
</html>
)";
    // 检查参数
    if (argc != 3)
    {
        Code1 += "alert(\'Web Application 调用格式错误！\');\n";
        Code1 += Code2;
        puts(Code1.c_str());
        return 0;
    }

    string NewDomain = argv[1], NewIP = argv[2];

    char szPath[MAX_PATH];
    GetSystemDirectoryA(szPath, MAX_PATH);
    string Path = szPath;
    Path += "\\drivers\\etc\\hosts";
    ifstream fin(Path);
    int cnt = 0;
    for (int i = 1; !fin.eof(); i++, cnt++)
    {
        getline(fin, List[i]);
        bool flag = false;
        for (int j = 0; j < List[i].size(); j++)
            if (List[i][j] != ' ' && List[i][j] != '\n' && List[i][j] != '\0' && List[i][j] != '\t')
            {
                if (List[i][j] != '#')
                    flag = true;
                break;
            }
        if (flag)
        {
            string Domain, IP;
            stringstream tempIO;
            tempIO << List[i];
            tempIO >> IP >> Domain;
            if (Domain == NewDomain)
            {
                Code1 += "alert(\'已有冲突的规则！\');\n";
                Code1 += Code2;
                puts(Code1.c_str());
                return 0;
            }
        }
    }
    fin.close();
    
    ofstream fout(Path, ios::app);
    fout << '\n' << NewIP << ' ' << NewDomain;
    fout.close();

    Code1 += Code2;
    puts(Code1.c_str());
    return 0;
}
