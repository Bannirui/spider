//
// Created by dingrui on 2022/6/22.
//

# pragma once

#include "pch.h"
#include "myurl.h"
#include "myhttp.h"
#include "mymulio.h"

class MyDownloader
{
public:
    explicit MyDownloader(const MyUrl &url, MyMulIO *mulIo);

    ~MyDownloader() = default;

    void pre_download();

public:
    MyMulIO *_r;
    MyHttp _http;
private:
    MyUrl _url;
};


