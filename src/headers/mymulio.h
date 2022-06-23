//
// Created by dingrui on 2022/6/22.
//

# pragma once

#include "pch.h"
#include "myhttp.h"

class MyMulIO
{
public:
    MyMulIO();

    ~MyMulIO() = default;

    // 向kq添加要关注的事件
    void add(const MyHttp &http, int16_t ev_filter);

    void modify(const MyHttp &http, uint32_t ev_filter);

    void remove(const MyHttp &http);

    int dispatch(int num, int timeout, std::vector<std::string> &storage);

private:
    int _kqfd;
    std::unordered_map<int, MyHttp> _fd_http_map;
};


