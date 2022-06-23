//
// Created by dingrui on 2022/6/22.
//

#include "myhttp.h"
#include "mysocket.h"

static const std::string REQUEST_METHOD = "GET";
static const std::string HTTP_VERSION = "HTTP/1.1";
static const std::string LF = "\n";
static const std::string BLANK_SPACE = " ";


MyHttp::MyHttp(std::string ip, std::string domain, MyMulIO *mulIo)
        : _ip(ip), _domain(domain), _r(mulIo)
{
    _sock = MySocket::create_socket();
    MySocket::conn(_ip, _port, _sock);
    MySocket::set_non_block(_sock);
    memset(&_peer, 0, sizeof(_peer));
}

int MyHttp::sock() const
{
    return _sock;
}

std::string MyHttp::build_htt_request(std::string url)
{
    _request_line += REQUEST_METHOD;
    _request_line += BLANK_SPACE;
    _request_line += url;
    _request_line += BLANK_SPACE;
    _request_line += HTTP_VERSION;
    _request_line += LF;

    _request_space = LF;
    _request += _request_line;
    _request += _request_space;
    std::cout << "request=" << _request << std::endl;
    return _request;
}

int MyHttp::send_http_request()
{
    _peer.sin_family = AF_INET;
    _peer.sin_port = htons(_port);
    _peer.sin_addr.s_addr = inet_addr(_ip.c_str());

    // system call
    ssize_t fd = send(_sock, _request.c_str(), _request.size(), 0);
    if (-1 == fd)
    {
        // TODO: 2022/6/22
        std::cerr << "send request error" << std::endl;
        return -1;
    }
    return (int) fd;
}

void MyHttp::recv_http_request(std::string *out)
{
    for (;;)
    {
        usleep(10000);

        char buffer[2048] = {0};
        // system call
        ssize_t s = recv(_sock, buffer, sizeof(buffer) - 1, 0);
        if (-1 == s)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                std::cout << "EAGAIN" << std::endl;
                break;
            } else if (errno == EINTR) continue;
            else break;
        }
        usleep(10000);
        buffer[s] = '\0';
        (*out) += buffer;
    }
}

void MyHttp::setSock(int sock)
{
    _sock = sock;
}
