//
// Created by dingrui on 2022/6/22.
//

#include "myutil.h"

std::string MyUtil::dns_parse(std::string domain)
{
    struct hostent *host = gethostbyname(domain.c_str());
    if (nullptr == host) exit(1);
    std::string ip = inet_ntoa(*((struct in_addr *) host->h_addr_list[0]));
    return std::move(ip);
}