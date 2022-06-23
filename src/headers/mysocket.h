//
// Created by dingrui on 2022/6/22.
//

# pragma once

#include "pch.h"

class MySocket
{
public:
    MySocket() = default;

    ~MySocket() = default;

    static void set_non_block(int sock);

    static int create_socket();

    static bool conn(std::string ip, uint16_t port, int sock);
};


