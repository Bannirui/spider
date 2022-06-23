//
// Created by dingrui on 2022/6/22.
//

#include "mydownloader.h"

MyDownloader::MyDownloader(const MyUrl &url, MyMulIO *mulIo)
        : _url(url), _r(mulIo), _http(url.getIp(), url.getDomain(), mulIo)
{
}

void MyDownloader::pre_download()
{
    // 向kqueue注册一个ke 关注写事件
    _r->add(_http, EVFILT_WRITE);
}