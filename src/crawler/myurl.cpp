//
// Created by dingrui on 2022/6/22.
//

#include "myurl.h"

#include "myutil.h"

MyUrl::MyUrl(const std::string &domain)
        : _domain(domain), _path("/"), _depth(1)
{
    _ip = MyUtil::dns_parse(domain);
}

MyUrl::MyUrl(const std::string &domain, std::string path)
        : _domain(domain), _path(std::move(path)), _depth(1)
{
    _ip = MyUtil::dns_parse(domain);
}

const std::string &MyUrl::getDomain() const
{
    return _domain;
}

const std::string &MyUrl::getIp() const
{
    return _ip;
}

const std::string &MyUrl::getPath() const
{
    return _path;
}