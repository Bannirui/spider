//
// Created by dingrui on 2022/6/22.
//

# pragma once

#include "myurl.h"
#include "mymulio.h"

class MyController
{
public:
    explicit MyController(MyUrl seed);

    ~MyController() = default;

    void start();

private:
    MyMulIO _mul_io; // 构造方法的时候触发构造
    MyUrl _seed;
    std::unordered_map<std::string, MyUrl> _url_map; // 已经抓取和待抓取的所有url集合
    std::queue<MyUrl> _url_wait_queue; // 未抓取的url等待队列
    std::vector<std::string> _page_storage; // 已经获取响应的页面
};


