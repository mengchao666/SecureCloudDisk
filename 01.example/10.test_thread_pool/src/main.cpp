#include <XThread.h>
#include <XThreadPool.h>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <event2/listener.h>
#include <iostream>
#include <signal.h>
#include <sys/socket.h>

using namespace std;

// telnet 127.0.0.1 20010

void ReadCB(struct bufferevent *bev, void *ctx)
{
    char buf[1024] = {0};
    int len = bufferevent_read(bev, buf, sizeof(buf) - 1);
    cout << "read len = " << len << endl;
    cout << buf << endl;

    // 插入buffer链表，监听到sock可写，才去写
    bufferevent_write(bev, "OK", 3);
}

void WriteCB(struct bufferevent *bev, void *ctx)
{
    cout << "write CB" << endl;
}

void EventCB(struct bufferevent *bev, short what, void *ctx)
{
    cout << "eventCB" << endl;
    if (what & BEV_EVENT_TIMEOUT && what & BEV_EVENT_READING)
    {
        cout << "Read Timeout" << endl;
        //读取缓冲区的内容

        //然后关掉
        bufferevent_free(bev);
    }
    else if (what & BEV_EVENT_TIMEOUT && what & BEV_EVENT_WRITING)
    {
        cout << "Write Timeout" << endl;
        //缓冲回滚

        //然后关掉
        bufferevent_free(bev);
    }
    else if (what & BEV_EVENT_ERROR)
    {
        cout << "BEV_EVENT_ERROR" << endl;

        //关掉
        bufferevent_free(bev);
    }
    // 关闭连接的正常错误
    else if (what & BEV_EVENT_EOF)
    {
        cout << "BEV_EVENT_EOF" << endl;
        // 考虑缓冲的处理

        //关掉
        bufferevent_free(bev);
    }
}

void ListenCB(struct evconnlistener *evc, evutil_socket_t client_socket,
              struct sockaddr *client_addr,
              int socklen, void *arg)
{
    cout << "ListenCB" << endl;
    char ip[16] = {0};
    sockaddr_in *addr = (sockaddr_in *)client_addr;
    evutil_inet_ntop(AF_INET, &addr->sin_addr, ip, sizeof(ip));
    cout << "client ip is = " << ip << endl;

    // 创建bufferevent上下文  内部创建了event对象read 和 write
    // BEV_OPT_CLOSE_ON_FREE 关闭bev时关闭socket
    event_base *base = (event_base *)arg;
    bufferevent *bev = bufferevent_socket_new(base, client_socket, BEV_OPT_CLOSE_ON_FREE);
    if (bev == NULL)
    {
        cerr << "bufferevent new failed" << endl;
    }

    // 添加监控事件
    bufferevent_enable(bev, EV_READ | EV_WRITE);

    // 超时时间设定，秒，微秒  读超时 和 写超时
    timeval t1 = {10, 0};
    bufferevent_set_timeouts(bev, &t1, 0);

    // 设置回调函数
    bufferevent_setcb(bev, ReadCB, WriteCB, EventCB, base);
}

int main()
{
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
    {
        return 1;
    }

    // 1、初始化线程池
    XThreadPool::Get()->Init(10); // 创建十个线程
    cout << "test thread pool create success" << endl;

    // 创建libevent 上下文，默认是创建base锁
    event_base *base = event_base_new();

    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(20010);
    sin.sin_addr.s_addr = htonl(INADDR_ANY);

    auto evc = evconnlistener_new_bind(base, ListenCB, base, LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,
                                       10, (sockaddr *)&sin, sizeof(sin));

    // 事件主循环，监控事件是否发送，分发事件到回调函数
    // 如果没有事件注册，则退出
    event_base_dispatch(base);

    evconnlistener_free(evc);
    event_base_free(base);

    return 0;
}
