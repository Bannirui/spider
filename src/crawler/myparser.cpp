//
// Created by dingrui on 2022/6/22.
//

#include "myparser.h"

bool MyParser::get_title(const std::string &page, std::string &file_name)
{
    auto begin = page.find("<title>");
    if (page.npos == begin)
        return false;
    begin = begin + strlen("<title>");
    auto end = page.find("</title>");
    if (page.npos == end) return false;
    file_name = "pages/";
    file_name += page.substr(begin, end - begin);
    return true;
}

bool MyParser::get_a_label(std::vector<std::string> &pages, std::unordered_map<std::string, MyUrl> &url_map,
                           std::queue<MyUrl> &url_wait_queue)
{
    for (auto &page: pages)
    {
        size_t begin = 0;
        while (true)
        {
            std::cout << "get a <a> label" << std::endl;
            sleep(1);
            begin = page.find("<a href=\"", begin);
            if (page.npos == begin)break;
            begin += strlen("<a href=\"");
            size_t end = page.find("\"", begin);
            if (page.npos == end) break;
            size_t len = end - begin;
            std::string str = page.substr(begin, len);
            std::cout << "begin=" << begin << ", end=" << end << " ,len=" << len << ", str=" << str << std::endl;
            begin = end;
            std::string domain;
            std::string path;
            parse_domain_and_path(str, &domain, &path);
            MyUrl url(domain, path);
            std::cout << "domain=" << url.getDomain() << std::endl;
            if (url_map.find(domain + path) != url_map.end()) continue;
            url_map.insert({domain + path, url});
            url_wait_queue.push(url);
        }
    }
    return true;
}

bool MyParser::start_from_html(std::string &page)
{
    auto pos = page.find("<!DOCTYPE html>");
    if (page.npos == pos) return false;
    page.erase(0, pos);
    return true;
}

bool MyParser::parse_domain_and_path(const std::string &url, std::string *domain, std::string *path)
{
    auto begin = url.find("//");
    if (url.npos == begin)
        return false;
    begin += strlen("//");
    auto end = url.find("/", begin);
    if (url.npos != end)
    {
        size_t len = end - begin;
        (*domain) = url.substr(begin, len);
        (*path) = url.substr(end);
    } else
    {
        // 根目录
        (*domain) = url.substr(begin);
        (*path) = "/";
    }
    std::cout << "domain=" << domain << std::endl;
    return true;
}