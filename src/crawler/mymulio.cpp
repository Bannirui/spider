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

    // 初始化kevent结构体
    // change 要初始化的kevent结构体
    // fd 文件描述符 监听哪个fd
    // 关注的事件 EVFILT_VNODE EVFILT_WRITE
    // EV_ADD 添加到kqueue中
    // EV_CLEAR 每次事件被取走 状态重置
    // NOTE_WRITE 每当fd指向的文件描述符有写入时返回 目的就是为了监听文件变化的
    // 附加数据 后面取就绪的时候作为判定依据
    EV_SET(&change, fd, ev_filter, EV_ADD | EV_ENABLE, 0, 0, &ev_filter);

    // changelist 指向kevent结构的指针 要监听的事件
    // eventlist 指向kevent结构的指针 如果有有事件发生会放在eventlist中
    // 返回的是eventlist里面事件的数量 即有多少个事件放到了eventlist中
    // 设定 如果nevents是0 函数会立即返回 如果nevents不是0 且timeout指针为空 函数会永久阻塞 直到事件发生
    // 作用就是向kqueue中注册要监听的事件
    // system call
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

void MyMulIO::modify(const MyHttp &http, uint32_t events)
{
    int fd = http.sock();
    struct kevent ev;
    EV_SET(&ev, fd, EVFILT_WRITE, EV_ADD | EV_CLEAR, events, 0, nullptr);
    // system call
    int ret = kevent(_kqfd, &ev, 1, nullptr, 0, nullptr);
    if (-1 == ret)
    {
        std::cout << "kq modify error" << std::endl;
        exit(1);
    }
}

void MyMulIO::remove(const MyHttp &http)
{
    int fd = http.sock();
    struct kevent ev;
    EV_SET(&ev, fd, EVFILT_WRITE, EV_DELETE, 0, 0, nullptr);
    // system call
    int ret = kevent(_kqfd, &ev, 1, nullptr, 0, nullptr);
    if (-1 == ret)
    {
        std::cout << "kq del error" << std::endl;
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
        return -1;
    }
    for (int i = 0; i < n; i++)
    {
        struct kevent event = events[i];
        int ev_fd = event.ident; // 绑定的fd
        int ev_flags = event.flags; // 发生的事件
        int att = *((int16_t *)event.udata); // 附加数据 标识就绪的是读还是写

        if (ev_flags & EV_ERROR)
        {
            std::cout << "kq event error" << std::endl;
            continue;
        }
        if (_fd_http_map.find(ev_fd) == _fd_http_map.end()) return n;
        if (att == EVFILT_READ)
        {
            // fd就绪 可读 读完改关注写事件
            std::string out = _fd_http_map[ev_fd]._response;
            _fd_http_map[ev_fd].recv_http_request(&out);
            _fd_http_map[ev_fd]._r->remove(_fd_http_map[ev_fd]);
            storage.push_back(std::move(out));
        }
        if (att == EVFILT_WRITE)
        {
            // fd就绪 可写 写完改关注读事件
            _fd_http_map[ev_fd].build_htt_request("/");
            _fd_http_map[ev_fd].send_http_request();
            _fd_http_map[ev_fd]._r->modify(_fd_http_map[ev_fd], EVFILT_READ);
        }
    }
    return n;
}