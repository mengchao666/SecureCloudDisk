#include <XServerTask.h>
#include <XThread.h>
#include <XThreadPool.h>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <event2/listener.h>
#include <iostream>
#include <signal.h>
#include <sys/socket.h>
#include <thread>
using namespace std;

static void ListenCB(int sock, struct sockaddr *addr, int socklen, void *arg)
{
    cout << "ListenCB is here" << endl;
}

// telnet 127.0.0.1 20010

int main()
{
    int serverPort = 20010;
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
    {
        return 1;
    }

    int threadCount = 10;
    // 1、初始化线程池
    XThreadPool::Get()->Init(threadCount); // 创建threadCount个线程

    XThreadPool serverPool;
    serverPool.Init(1);
    auto task = new XServerTask();
    task->SetServerPort(serverPort);
    task->ListenCB = ListenCB;
    serverPool.Dispatch(task);

    for (;;)
    {
        this_thread::sleep_for(chrono::seconds(1));
    }
    return 0;
}
