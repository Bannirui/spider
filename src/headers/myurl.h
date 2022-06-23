//
// Created by dingrui on 2022/6/22.
//

# pragma once

#include "pch.h"

class MyUrl
{
public:
    explicit MyUrl(const std::string &domain);

    explicit MyUrl(const std::string &domain, std::string path);

    ~MyUrl() = default;

    const std::string &getDomain() const;

    const std::string &getIp() const;

    const std::string &getPath() const;

private:
    std::string _domain;
    std::string _ip;
    std::string _path;
    int _depth{};
};


