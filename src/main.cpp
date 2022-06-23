#include <iostream>

#include "myurl.h"
#include "mycontroller.h"

int main()
{
    MyUrl seed("www.baidu.com");
    MyController con(seed);
    con.start();
    return 0;
}