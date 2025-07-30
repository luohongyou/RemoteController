// rcauth.cpp : Coral Remote Controller 后台凭据管理服务

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#include <iostream>
#include <Windows.h>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
#include "AuthEncoding.h"
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

char buf[25600] = "";
std::string Action, Output;

#define TOKENEXPIREDTIME 10800
#define RSAEXPIREDTIME 10

class CUserWithToken : public CUser
{
public:
    std::string Token;
    time_t TokenExpiredTime;
};

std::string LocalToken, CloudToken, CloudPIN;
time_t CloudTokenExpiredTime;
std::vector<CUserWithToken> UserList;

std::ifstream fin;
RSAEncryptor* Encryptor;
time_t RSAKeyExpiredTime;

std::string CreateCloudPIN();

int main()
{
    HANDLE hMutex = NULL;
    hMutex = CreateMutexA(NULL, FALSE, "__CoralRemoteControllerAuthManager");
    if (hMutex != NULL)
        if (ERROR_ALREADY_EXISTS == GetLastError())
        {
            ReleaseMutex(hMutex);
            return 0;
        }

    SetAppDirectory("\\..\\..\\..\\Website\\htdocs\\");

    srand(time(NULL));

    // Create RSA Keys
    Encryptor = new RSAEncryptor;
    time(&RSAKeyExpiredTime);
    RSAKeyExpiredTime += RSAEXPIREDTIME;

    // Create Tokens
    LocalToken = NewHostToken();
    CloudToken = NewCloudToken();
    time(&CloudTokenExpiredTime);
    CloudTokenExpiredTime += TOKENEXPIREDTIME; // 1 Hour

    // Create CloudStorage PIN
    CloudPIN = CreateCloudPIN();

    int _Version = 1;
    fin.open(DatabasePath() + "OOBEFlag");
    if (!fin.fail())
        fin >> _Version;
    fin.close();
    if (_Version >= 3)
    {
        fin.open(DatabasePath() + "User\\list");
        if (!fin.fail())
        {
            int tot;
            CUserWithToken Temp;
            std::string _uname, _guid;
            fin >> tot;
            for (int i = 1; i <= tot; i++)
            {
                fin >> _uname >> _guid;
                Temp.Assign(_guid, _uname);
                Temp.Token = NewToken();
                time(&Temp.TokenExpiredTime);
                Temp.TokenExpiredTime += TOKENEXPIREDTIME; // 1 Hour
                UserList.push_back(Temp);
            }
        }
        fin.close();
    }

    DWORD rlen = 0;
    HANDLE hPipe = CreateNamedPipe(TEXT("\\\\.\\Pipe\\CoralRemoteControllerAuthManager"),
        PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES,
        0, 0, 5000, NULL);
    if (INVALID_HANDLE_VALUE == hPipe)
        return 0;

    while (true)
    {
        if (!ConnectNamedPipe(hPipe, NULL))
            ;// printf("Connection failed!\n");

        while (true)
        {
            memset(buf, 0, sizeof(buf));
            if (!ReadFile(hPipe, buf, sizeof(buf), &rlen, NULL))
                break;

            else
            {
                std::stringstream tempIO;
                tempIO << buf;
                tempIO >> Action;
                Output = "";

                // printf("%s\tAction:%s\n", StandardTime(false).c_str(), Action.c_str());

                if (Action == "RequestGUID2NamebyToken")
                {
                    std::string _Token;
                    tempIO >> _Token;

                    // printf("Token: %s\n", _Token.c_str());
                    time_t NowTime;
                    time(&NowTime);

                    Output = "CORAL_RC_AUTH_FAILED";
                    if (_Token == LocalToken)
                        Output = "00000000-0000-0000-0000-000000000000 ForegroundUser";
                    else if (_Token == CloudToken)
                    {
                        if (CloudTokenExpiredTime >= NowTime || !GetSettings("PasswordExpired", true))
                            Output = "FFFFFFFF-FFFF-FFFF-FFFF-FFFFFFFFFFFF CloudStorageUser";
                        else
                            CloudToken = NewCloudToken();

                        time(&CloudTokenExpiredTime);
                        CloudTokenExpiredTime += TOKENEXPIREDTIME; // 1 Hour
                    }
                    else
                    {
                        for (auto &UserItem : UserList)
                            if (UserItem.Token == _Token)
                            {
                                if (UserItem.TokenExpiredTime >= NowTime || !GetSettings("PasswordExpired", true))
                                    Output = UserItem.GetGUID() + " " + UserItem.GetName();

                                else
                                    UserItem.Token = NewToken();

                                time(&UserItem.TokenExpiredTime);
                                UserItem.TokenExpiredTime += TOKENEXPIREDTIME; // 1 Hour

                                break;
                            }
                    }
                }
                else if (Action == "RequestForegroundToken")
                {
                    Output = LocalToken;
                }
                else if (Action == "RequestRSAPublicKey")
                {
                    time_t NowTime;
                    time(&NowTime);
                    if (NowTime >= RSAKeyExpiredTime && GetSettings("PasswordExpired", true))
                    {
                        delete Encryptor;
                        Encryptor = new RSAEncryptor;
                        time(&RSAKeyExpiredTime);
                        RSAKeyExpiredTime += RSAEXPIREDTIME;
                    }

                    Output = Encryptor->getPublicKey();
                }
                else if (Action == "RequestLogin")
                {
                    time_t NowTime;
                    time(&NowTime);

                    if (NowTime >= RSAKeyExpiredTime && GetSettings("PasswordExpired", true))
                    {
                        delete Encryptor;
                        Encryptor = new RSAEncryptor;
                        time(&RSAKeyExpiredTime);
                        RSAKeyExpiredTime += RSAEXPIREDTIME;
                    }
                    
                    else
                    {
                        std::string _User, RSA_encrypted, md5_encrypted;
                        tempIO >> _User >> RSA_encrypted;

                        std::vector<unsigned char> encrypted_message;
                        for (int i = 0; i < RSA_encrypted.size(); i += 2)
                            encrypted_message.push_back(std::stoi(RSA_encrypted.substr(i, 2), 0, 16));
                        md5_encrypted = getMD5str(Encryptor->decrypt(encrypted_message));

                        for (auto& UserItem : UserList)
                            if (UserItem.GetName() == _User)
                            {
                                if (UserItem.GetPasswordMD5() == md5_encrypted)
                                {
                                    UserItem.Token = NewToken();
                                    time(&UserItem.TokenExpiredTime);
                                    UserItem.TokenExpiredTime += TOKENEXPIREDTIME; // 1 Hour
                                    Output = UserItem.GetGUID() + " " + UserItem.Token;
                                }
                                break;
                            }
                    }
                }
                else if (Action == "RequestCloudLogin")
                {
                    std::string readPIN;
                    tempIO >> readPIN;
                    if (readPIN == CloudPIN)
                    {
                        time_t NowTime;
                        time(&NowTime);

                        if (CloudTokenExpiredTime <= NowTime && GetSettings("PasswordExpired", true))
                            CloudToken = NewCloudToken();

                        time(&CloudTokenExpiredTime);
                        CloudTokenExpiredTime += TOKENEXPIREDTIME; // 1 Hour

                        Output = CloudToken;
                    }
                }
                else if (Action == "RequestCloudPIN")
                {
                    Output = CloudPIN;
                }
                else if (Action == "RefreshCloudToken")
                {
                    CloudToken = NewCloudToken();
                    Output = "Yes";
                }
                else if (Action == "RefreshCloudPIN")
                {
                    CloudPIN = CreateCloudPIN();
                    Output = "Yes";
                }
                else if (Action == "ChangePassword")
                {
                    Output = "No";

                    time_t NowTime;
                    time(&NowTime);

                    if (NowTime >= RSAKeyExpiredTime && GetSettings("PasswordExpired", true))
                    {
                        delete Encryptor;
                        Encryptor = new RSAEncryptor;
                        time(&RSAKeyExpiredTime);
                        RSAKeyExpiredTime += RSAEXPIREDTIME;
                    }

                    else
                    {
                        std::string UserGUID,
                            Old_RSA_encrypted, New_RSA_encrypted,
                            old_md5_encrypted, new_md5_encrypted;
                        tempIO >> UserGUID >> Old_RSA_encrypted >> New_RSA_encrypted;

                        {
                            std::vector<unsigned char> encrypted_message;
                            for (int i = 0; i < Old_RSA_encrypted.size(); i += 2)
                                encrypted_message.push_back(std::stoi(Old_RSA_encrypted.substr(i, 2), 0, 16));
                            old_md5_encrypted = getMD5str(Encryptor->decrypt(encrypted_message));
                        }
                        {
                            std::vector<unsigned char> encrypted_message;
                            for (int i = 0; i < New_RSA_encrypted.size(); i += 2)
                                encrypted_message.push_back(std::stoi(New_RSA_encrypted.substr(i, 2), 0, 16));
                            new_md5_encrypted = getMD5str(Encryptor->decrypt(encrypted_message));
                        }

                        for (auto& UserItem : UserList)
                            if (UserItem.GetGUID() == UserGUID)
                            {
                                if (UserItem.GetPasswordMD5() == old_md5_encrypted)
                                    Output = new_md5_encrypted;
                                break;
                            }
                    }
                }
                else if (Action == "RefreshUser")
                {
                    std::string UserGUID;
                    tempIO >> UserGUID;

                    for (auto& UserItem : UserList)
                        if (UserItem.GetGUID() == UserGUID)
                        {
                            UserItem.Assign(UserGUID);
                            break;
                        }
                    Output = "Yes";
                }
                else if (Action == "RefreshUserToken")
                {
                    std::string UserGUID;
                    tempIO >> UserGUID;

                    for (auto& UserItem : UserList)
                        if (UserItem.GetGUID() == UserGUID)
                        {
                            UserItem.Token = NewToken();
                            time(&UserItem.TokenExpiredTime);
                            UserItem.TokenExpiredTime += TOKENEXPIREDTIME; // 1 Hour
                            break;
                        }
                    Output = "Yes";
                }
                else if (Action == "AddUser")
                {
                    std::string UserGUID;
                    tempIO >> UserGUID;

                    CUserWithToken New;
                    New.Assign(UserGUID);
                    if (New.GetGUID() != "")
                    {
                        New.Token = NewToken();
                        time(&New.TokenExpiredTime);
                        New.TokenExpiredTime += TOKENEXPIREDTIME; // 1 Hour
                        UserList.push_back(New);
                    }
                    Output = "Yes";
                }
                else if (Action == "DeleteUser")
                {
                    std::string UserGUID;
                    tempIO >> UserGUID;

                    for (auto i = UserList.begin(); i != UserList.end(); i++)
                        if (i->GetGUID() == UserGUID)
                        {
                            UserList.erase(i);
                            break;
                        }
                    Output = "Yes";
                }
                else if (Action == "Reset")
                {
                    UserList.clear();
                    LocalToken = NewHostToken();

                    delete Encryptor;
                    Encryptor = new RSAEncryptor;
                    time(&RSAKeyExpiredTime);
                    RSAKeyExpiredTime += RSAEXPIREDTIME;

                    Output = "Yes";
                }

                // printf("Output: %s\n", Output.c_str());

                DWORD wlen = 0;
                WriteFile(hPipe, Output.c_str(), Output.size(), &wlen, 0);
            }
        }
        FlushFileBuffers(hPipe);
        DisconnectNamedPipe(hPipe);
    }

    return 0;
}

std::string CreateCloudPIN()
{
    std::string PIN;
    srand(time(NULL));
    int digit = 0;
    for (int i = 1; i <= 6; i++)
    {
        digit = rand() % 10;
        PIN += (digit + '0');
    }
    return PIN;
}