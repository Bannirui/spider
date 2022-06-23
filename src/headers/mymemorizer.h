//
// Created by dingrui on 2022/6/22.
//

# pragma once

#include "pch.h"

class MyMemorizer
{
public:
    MyMemorizer() = default;

    ~MyMemorizer() = default;

    static void save(std::vector<std::string> &pages);
};


