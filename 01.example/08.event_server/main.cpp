#include <event2/bufferevent.h>
#include <event2/event.h>
#include <iostream>
#include <signal.h>
#include <sys/socket.h>

using namespace std;

void ListenCB(evutil_socket_t sock, short what, void *arg)
{
    cout << "ListenCB" << endl;
    if (!(what & EV_READ))
    {
        cout << "not read" << endl;
    }
    sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    socklen_t size = sizeof(sin);
    evutil_socket_t client_socket = accept(sock, (sockaddr *)&sin, &size);
    if (client_socket <= 0)
    {
        cerr << "accept error" << endl;
    }
    char ip[16] = {0};
    evutil_inet_ntop(AF_INET, &sin.sin_addr, ip, sizeof(ip));
    cout << "client ip is = " << ip << endl;
}

int main()
{
    // 使用断开连接的socket,会发出此信号，造成程序退出
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
    {
        return 1;
    }

    // 创建libevent 上下文，默认是创建base锁
    event_base *base = event_base_new();

    // 创建socket绑定端口
    evutil_socket_t sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock <= 0)
    {
        cout << "socket error" << strerror(errno) << endl;
    }

    // 设置地址服用和非阻塞
    evutil_make_socket_nonblocking(sock);
    evutil_make_listen_socket_reuseable(sock);

    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(20010);
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    if (::bind(sock, (sockaddr *)&sin, sizeof(sin)) != 0)
    {
        cerr << "bind error" << endl;
    }
    cout << "bind port success" << endl;

    listen(sock, 10);

    // 默认水平触发(只要有数据没有处理，就一直触发)        EV_ET(边缘触发)
    // event_self_cbarg就是传递当前创建的event对象
    event *ev = event_new(base, sock, EV_READ | EV_PERSIST, ListenCB, event_self_cbarg());

    // 开始监听事件 第二个参数为超时时间
    event_add(ev, NULL);

    // 事件主循环，监控事件是否发送，分发事件到回调函数
    // 如果没有事件注册，则退出
    event_base_dispatch(base);

    evutil_closesocket(sock);
    event_del(ev);
    event_free(ev);
    event_base_free(base);

    return 0;
}
