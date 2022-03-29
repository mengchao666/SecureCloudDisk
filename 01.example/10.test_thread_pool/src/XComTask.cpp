#include <XComTask.h>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <iostream>
using namespace std;

static void SReadCB(struct bufferevent *bev, void *ctx)
{
    cout << "SReadCB" << endl;
}

static void SWriteCB(struct bufferevent *bev, void *ctx)
{
    cout << "SWriteCB" << endl;
}

static void SEventCB(struct bufferevent *bev, short what, void *ctx)
{
    cout << "SEventCB what is " << what << endl;
    if (what & BEV_EVENT_CONNECTED)
    {
        cout << "connect is success" << endl;
    }
}

//初始化任务
bool XComTask::Init()
{
    // 建立连接bufferevent
    m_bev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE); // -1自动创建socket
    if (!m_bev)
    {
        cerr << "bufferevent_socket_new error " << endl;
    }

    // 设置回调
    bufferevent_setcb(m_bev, SReadCB, SWriteCB, SEventCB, this);

    bufferevent_enable(m_bev, EV_READ | EV_WRITE);

    timeval tv = {10, 0};
    bufferevent_set_timeouts(m_bev, &tv, &tv);

    // 连接服务器
    if (m_serverIP.empty())
    {
        return true;
    }
    sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(m_serverPort);
    evutil_inet_pton(AF_INET, m_serverIP.c_str(), &sin.sin_addr.s_addr);
    int re = bufferevent_socket_connect(m_bev, (sockaddr *)&sin, sizeof(sin));
    if (re != 0)
    {
        return false;
    }
    return true;
}