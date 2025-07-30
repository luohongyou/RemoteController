// AuthEncoding.h : Coral Remote Controller OpenSSL编解码代码模板的定义。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#pragma warning(disable : 4996)

#ifndef RC_AENCODING
#define RC_AENCODING

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/md5.h>
#include <iostream>
#include <cstring>
#include <vector>
#include <memory>
#include <stdexcept>
#pragma comment(lib, "libssl.lib")
#pragma comment(lib, "libcrypto.lib")

// RSA
class RSAEncryptor
{
private:
    std::unique_ptr<RSA, decltype(&RSA_free)> rsa;
    std::unique_ptr<BIGNUM, decltype(&BN_free)> bn;

public:
    RSAEncryptor() : rsa(RSA_new(), RSA_free), bn(BN_new(), BN_free)
    {
        BN_set_word(bn.get(), RSA_F4);
        if (RSA_generate_key_ex(rsa.get(), 2048, bn.get(), nullptr) != 1)
        {
            throw std::runtime_error("RSA key generation failed");
        }
    }

    std::string getPublicKey()
    {
        BIO* bio = BIO_new(BIO_s_mem());
        if (bio == nullptr)
        {
            BIO_free_all(bio);
            throw std::runtime_error("Error creating BIO");

            return "";
        }

        if (PEM_write_bio_RSAPublicKey(bio, rsa.get()) != 1)
        {
            BIO_free_all(bio);
            throw std::runtime_error("Error writing public key to BIO");

            return "";
        }

        char* pub_key_pem;
        long pub_key_len = BIO_get_mem_data(bio, &pub_key_pem);
        if (pub_key_len <= 0)
        {
            BIO_free_all(bio);
            throw std::runtime_error("Error reading public key from BIO");

            return "";
        }

        std::stringstream tempIO;
        tempIO.write(pub_key_pem, pub_key_len);
        BIO_free_all(bio);

        return tempIO.str();
    }
    std::string getPrivateKey()
    {
        BIO* bio = BIO_new(BIO_s_mem());
        if (bio == nullptr)
        {
            BIO_free_all(bio);
            throw std::runtime_error("Error creating BIO");

            return "";
        }

        if (PEM_write_bio_RSAPrivateKey(bio, rsa.get(), nullptr, nullptr, 0, nullptr, nullptr) != 1)
        {
            BIO_free_all(bio);
            throw std::runtime_error("Error writing private key to BIO");

            return "";
        }

        char* pub_key_pem;
        long pub_key_len = BIO_get_mem_data(bio, &pub_key_pem);
        if (pub_key_len <= 0)
        {
            BIO_free_all(bio);
            throw std::runtime_error("Error reading private key from BIO");

            return "";
        }

        std::stringstream tempIO;
        tempIO.write(pub_key_pem, pub_key_len);
        BIO_free_all(bio);

        return tempIO.str();
    }

    std::vector<unsigned char> encrypt(const std::string& plaintext)
    {
        int rsa_size = RSA_size(rsa.get());
        std::vector<unsigned char> encrypted(rsa_size);

        int result = RSA_public_encrypt(plaintext.size(),
            reinterpret_cast<const unsigned char*>(plaintext.data()),
            encrypted.data(),
            rsa.get(),
            RSA_PKCS1_PADDING);
        if (result == -1)
        {
            throw std::runtime_error("RSA encryption failed");
        }

        encrypted.resize(result);
        return encrypted;
    }
    std::string decrypt(const std::vector<unsigned char>& ciphertext)
    {
        int rsa_size = RSA_size(rsa.get());
        std::vector<unsigned char> decrypted(rsa_size);

        int result = RSA_private_decrypt(ciphertext.size(),
            ciphertext.data(),
            decrypted.data(),
            rsa.get(),
            RSA_PKCS1_PADDING);
        if (result == -1)
        {
            throw std::runtime_error("RSA decryption failed");
        }

        return std::string(decrypted.begin(), decrypted.begin() + result);
    }
};

// MD5
std::string getMD5str(std::string _data)
{
    const char* data = _data.c_str();

    MD5_CTX c;
    MD5_Init(&c);
    MD5_Update(&c, data, strlen(data));

    char result[33];
    unsigned char digest[16] = { 0 };
    char hexBuffer[3];
    MD5_Final(digest, &c);
    result[0] = '\0';
    for (size_t i = 0; i != 16; ++i)
    {
        if (digest[i] < 16)     sprintf(hexBuffer, "0%x", digest[i]);
        else                    sprintf(hexBuffer, "%x", digest[i]);
        strcat(result, hexBuffer);
    }
    result[32] = '\0';

    return result;
}

// Base64
std::string base64_decode(const std::string& input)
{
    BIO* b64 = BIO_new(BIO_f_base64());
    BIO* bio = BIO_new_mem_buf(input.data(), input.size());
    bio = BIO_push(b64, bio);
    std::string output;
    char inbuf[512];
    int inlen;
    while ((inlen = BIO_read(bio, inbuf, 512)) > 0)
        output.append(inbuf, inlen);

    BIO_free_all(bio);
    return output;
}

std::string base64_encode(const std::string& input)
{
    std::string output;
    BIO* b64 = BIO_new(BIO_f_base64());
    BIO* bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);

    BIO_write(bio, input.c_str(), input.size());

    BIO_flush(bio);

    BUF_MEM* bptr = nullptr;
    BIO_get_mem_ptr(bio, &bptr);
    char* outbuf = (char*)malloc(bptr->length);
    memcpy(outbuf, bptr->data, bptr->length);
    output = outbuf;
    free(outbuf);
    BIO_free_all(bio);
    return output;
}

#endif