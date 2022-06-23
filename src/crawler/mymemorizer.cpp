//
// Created by dingrui on 2022/6/22.
//

#include "mymemorizer.h"
#include "myparser.h"

void MyMemorizer::save(std::vector<std::string> &pages)
{
    for (auto &page: pages)
    {
        std::string file_name;
        bool is_find_title = MyParser::get_title(page, file_name);
        if (!is_find_title) return;
        file_name += ".html";
        int fd = open(file_name.c_str(), O_WRONLY | O_CREAT, 0644);
        if (fd <= 0)
        {
            // TODO: 2022/6/22
            continue;
        }
        MyParser::start_from_html(page);
        int cnt = write(fd, page.c_str(), page.size());
        if (cnt > 0) std::cout << "write successfully" << std::endl;
        close(fd);
    }
}