//
// Created by dingrui on 2022/6/22.
//

# pragma once

#include "pch.h"

class MyUtil
{
public:
    /**
     * @brief 域名解析为ip地址
     * @param domain 域名
     * @return ip
     * @example www.baidu.com -> 180.101.49.11
     */
    static std::string dns_parse(std::string domain);
};


