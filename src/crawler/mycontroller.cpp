//
// Created by dingrui on 2022/6/22.
//

#include "mycontroller.h"

#include "mydownloader.h"
#include "mymemorizer.h"
#include "myparser.h"

MyController::MyController(MyUrl seed)
        : _seed(seed)
{
    _url_map.insert({seed.getDomain() + seed.getPath(), seed});
    _url_wait_queue.push(seed);
}

void MyController::start()
{
    while (true)
    {
        if (!_url_wait_queue.empty())
        {
            MyUrl url = _url_wait_queue.front();
            _url_wait_queue.pop();

            MyDownloader downloader(url, &_mul_io);
            downloader.pre_download();
        } else
        {
            int n = _mul_io.dispatch(10, 100, _page_storage);
            if (-1 == n)
                continue;
            else if (n == 0)
            {
                std::cout << "all events are done" << std::endl;
                break;
            } else
            {
                MyMemorizer::save(_page_storage);
                MyParser::get_a_label(_page_storage, _url_map, _url_wait_queue);
                std::cout << "url_map" << std::endl;
                for (auto u: _url_map) std::cout << u.first << std::endl;
                std::cout << "url_queue=" << _url_wait_queue.empty() << ", n=" << n << std::endl;
                sleep(5);
            }
        }
    }
}