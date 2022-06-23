//
// Created by dingrui on 2022/6/22.
//

# pragma once

#include "pch.h"

#include "myurl.h"

class MyParser
{
public:
    MyParser() = default;

    ~MyParser() = default;

    static bool get_title(const std::string &page, std::string &file_name);

    static bool get_a_label(std::vector<std::string> &pages, std::unordered_map<std::string, MyUrl> &url_map,
                            std::queue<MyUrl> &url_wait_queue);

    static bool start_from_html(std::string &page);

    static bool parse_domain_and_path(const std::string &url, std::string *domain, std::string *path);
};