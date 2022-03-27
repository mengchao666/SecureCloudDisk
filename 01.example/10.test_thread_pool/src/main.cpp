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
