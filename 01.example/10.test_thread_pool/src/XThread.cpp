#include "XThread.h"
#include <XTask.h>
#include <event2/buffer.h>
#include <event2/event.h>
#include <iostream>
#include <thread>
#include <unistd.h>

using namespace std;

// 激活线程任务的事件回调函数
static void NotifyCB(evutil_socket_t fd, short which, void *arg)
{
    XThread *t = (XThread *)arg;
    t->Notify(fd, which);
}

void XThread::Notify(evutil_socket_t fd, short which)
{
    // 水平触发，只要没有接收完成，会再次进来
    char buf[2] = {0};
    int re = read(fd, buf, 1);
    if (re <= 0)
    {
        return;
    }
    cout << "id = " << id << buf << endl;
    XTask *task = NULL;

    // 获取任务，并初始化任务
    tasks_mutex.lock();
    if (tasks.empty())
    {
        tasks_mutex.unlock();
        return;
    }
    task = tasks.front(); // 先进先出
    tasks.pop_front();
    tasks_mutex.unlock();
    task->Init();
}

// 添加处理的任务，一个线程可以同时处理多个任务，共用一个event_base
void XThread::AddTask(XTask *t)
{
    if (!t)
        return;
    t->base = this->base;
    tasks_mutex.lock();
    tasks.push_back(t);
    tasks_mutex.unlock();
}

// 线程激活
void XThread::Activate()
{
    int re = write(this->notify_send_fd, "c", 1);
    if (re <= 0)
    {
        cerr << "Activate failed" << endl;
    }
}

// 启动线程
void XThread::Start()
{
    Setup();
    // 启动线程
    thread th(&XThread::Main, this);

    // 断开与主线程联系
    th.detach();
}

// 线程入口函数
void XThread::Main()
{
    cout << id << "void XThread::Main() begin" << endl;
    if (!base)
    {
        cerr << "Thread::Main() failed! base is null" << endl;
    }
    event_base_dispatch(base);
    event_base_free(base);
    cout << id << "void XThread::Main() end" << endl;
}

//安装线程，初始化event_base和管道监听事件，用于激活线程
bool XThread::Setup()
{
    // linux用管道 创建为管道，不能用send recv ，用read write
    // fds[0] 读 fds[1]写
    int fds[2];
    if (pipe(fds))
    {
        cerr << "pipe failed" << endl;
        return false;
    }

    // 读取绑定到event事件中，写入要保存
    notify_send_fd = fds[1];

    // 创建libevent上下文,无锁
    event_config *ev_config = event_config_new();
    event_config_set_flag(ev_config, EVENT_BASE_FLAG_NOLOCK);
    this->base = event_base_new_with_config(ev_config);
    event_config_free(ev_config);
    if (!base)
    {
        cerr << "event_base_new_with_config failed in thread" << endl;
    }

    // 添加管道监听事件，用于激活线程执行任务
    event *ev = event_new(base, fds[0], EV_READ | EV_PERSIST, NotifyCB, this);
    event_add(ev, NULL);

    return true;
}