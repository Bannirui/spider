//
// Created by dingrui on 2022/6/22.
//

# pragma once

#include "pch.h"

class MyMulIO;

static const uint16_t DEFAULT_PORT = 80;

class MyHttp
{
public:
    MyHttp() = default;

    MyHttp(std::string ip, std::string domain, MyMulIO *mulIo);

    ~MyHttp() = default;

    int sock() const;

    void setSock(int sock);

    std::string build_htt_request(std::string url);

    int send_http_request();

    void recv_http_request(std::string *out);

public:
    std::string _response;
    MyMulIO *_r;

private:
    std::string _ip;
    std::string _domain;
    uint16_t _port = DEFAULT_PORT;
    int _sock;
    sockaddr_in _peer;

    std::string _request;
    std::string _request_line;
    std::string _request_header;
    std::string _request_space;
};