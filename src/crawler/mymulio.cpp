//
// Created by dingrui on 2022/6/22.
//

#include "mymulio.h"

MyMulIO::MyMulIO()
{
    // 创建内核事件队列
    // 返回文件描述符
    // 有错误 返回-1
    _kqfd = kqueue();
    if (-1 == _kqfd)
    {
        std::cerr << "kqueue create error" << std::endl;
        exit(1);
    }
}

void MyMulIO::add(const MyHttp &http, int16_t ev_filter)
{
    int fd = http.sock();
    struct kevent change;

    EV_SET(&change, fd, ev_filter, EV_ADD, 0, 0, nullptr);
    // system call 不阻塞
    int ret = kevent(_kqfd, &change, 1, nullptr, 0, nullptr);
    if (-1 == ret)
    {
        std::cout << "kq add error=" << errno << std::endl;
        exit(1);
    } else
    {
        _fd_http_map.insert({fd, http});
    }
}

void MyMulIO::modify(const MyHttp &http, uint32_t ev_filter)
{
    int fd = http.sock();
    struct kevent change;
    EV_SET(&change, fd, ev_filter, EV_ADD, 0, 0, nullptr);
    // system call
    int ret = kevent(_kqfd, &change, 1, nullptr, 0, nullptr);
    if (-1 == ret)
    {
        std::cout << "kq modify error" << std::endl;
        exit(1);
    }
}

void MyMulIO::remove(const MyHttp &http)
{
    int fd = http.sock();
    struct kevent change;
    EV_SET(&change, fd, EVFILT_READ, EV_DELETE, 0, 0, nullptr);
    // system call
    int ret = kevent(_kqfd, &change, 1, nullptr, 0, nullptr);
    if (-1 == ret)
    {
        std::cout << "kq remove error" << std::endl;
        exit(1);
    } else
    {
        auto pos = _fd_http_map.find(fd);
        if (pos != _fd_http_map.end())
        {
            _fd_http_map.erase(pos);
        }
    }
}

int MyMulIO::dispatch(int num, int timeout, std::vector<std::string> &storage)
{
    struct kevent events[1024];
    struct timespec ts;
    ts.tv_sec = timeout / 1000;
    // TODO: 2022/6/22 超时怎么正确使用
    // system call 已经就绪的fd数量
    int n = kevent(_kqfd, nullptr, 0, events, 1024, nullptr);
    if (-1 == n)
    {
        std::cerr << "kevent() system call err, errno=" << errno << std::endl;
        return 0;
    }
    int read_ready_cnt = 0;
    for (int i = 0; i < n; i++)
    {
        struct kevent event = events[i];
        int ev_fd = (int) (event.ident); // 绑定的fd
        int ev_filter = (int) (event.filter); // 被关注的事件

        if (ev_filter == EV_ERROR)
        {
            std::cout << "kq event error" << std::endl;
            continue;
        }
        if (_fd_http_map.find(ev_fd) == _fd_http_map.end()) return n;
        if (ev_filter == EVFILT_READ)
        {
            // fd就绪 可读 读完改关注写事件
            std::string out = _fd_http_map[ev_fd]._response;
            _fd_http_map[ev_fd].recv_http_request(&out);
            _fd_http_map[ev_fd]._r->remove(_fd_http_map[ev_fd]);
            storage.push_back(std::move(out));
            read_ready_cnt++;
        }
        if (ev_filter == EVFILT_WRITE)
        {
            // fd就绪 可写 写完改关注读事件
            _fd_http_map[ev_fd].build_htt_request("/");
            _fd_http_map[ev_fd].send_http_request();
            _fd_http_map[ev_fd]._r->modify(_fd_http_map[ev_fd], EVFILT_READ);
        }
    }
    return read_ready_cnt;
}