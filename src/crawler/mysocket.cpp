//
// Created by dingrui on 2022/6/22.
//

#include "mysocket.h"

void MySocket::set_non_block(int sock)
{
    int fl = fcntl(sock, F_GETFL);
    fcntl(sock, F_SETFL, fl | O_NONBLOCK);
}

int MySocket::create_socket()
{
    // TODO: 2022/6/22 源码里面是写的AF_INET 但是mac os文档只有PF_INET
    int fd = socket(PF_INET, SOCK_STREAM, 0);
    if (-1 == fd)
    {
        std::cerr << "create sock failed" << std::endl;
        return -1;
    }
    return fd;
}

bool MySocket::conn(std::string ip, uint16_t port, int sock)
{
    sockaddr_in server;
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(ip.c_str());
    if (connect(sock, (sockaddr *) &server, sizeof(server)) == 0) return true;
    std::cout << "socket connect failed" << std::endl;
    return false;
}