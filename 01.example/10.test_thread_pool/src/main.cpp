#include <XFtpServerCMD.h>
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

void ListenCB(struct evconnlistener *e, evutil_socket_t s,
              struct sockaddr *client_addr,
              int socklen, void *arg)
{
    cout << "listen CB" << endl;
    XTask *task = new XFtpServerCMD();
    task->sock = s;
    XThreadPool::Get()->Dispatch(task);
}

int main()
{
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
    {
        return 1;
    }

    // 1、初始化线程池
    XThreadPool::Get()->Init(3); // 创建3个线程
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
