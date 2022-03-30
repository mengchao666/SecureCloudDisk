#include <XServerTask.h>
#include <XTask.h>
#include <event2/event.h>
#include <event2/listener.h>
#include <iostream>
#include <string.h>
using namespace std;

void SListenCB(struct evconnlistener *evc, evutil_socket_t client_socket,
               struct sockaddr *client_addr, int socklen, void *arg)
{
    cout << "SListenCB" << endl;
    auto task = (XServerTask *)arg;
    if (task->ListenCB)
    {
        task->ListenCB(client_socket, client_addr, socklen, arg);
    }
    else
    {
        cerr << "please set callback function ListenCB" << endl;
    }
}

bool XServerTask::Init()
{
    if (m_serverPort <= 0)
    {
        cerr << "XServerTask::Init failed m_serverPort is invaild" << endl;
        return false;
    }
    // 监听端口
    sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_port = htons(m_serverPort);
    sin.sin_family = AF_INET;
    // 设置回调函数
    auto evc = evconnlistener_new_bind(basefunc(), SListenCB, this, LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,
                                       10, (sockaddr *)&sin, sizeof(sin));
    if (!evc)
    {
        cout << "listen port " << m_serverPort << " failed" << endl;
    }
    return true;
}