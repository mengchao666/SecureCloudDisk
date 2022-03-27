#include <XTask.h>
#include <XThread.h>
#include <XThreadPool.h>
#include <iostream>
#include <thread>

using namespace std;

void XThreadPool::Init(int threadCount)
{
    this->threadCount = threadCount;
    this->lastThread = -1;
    for (int i = 0; i < threadCount; i++)
    {
        XThread *t = new XThread();
        t->id = i + 1; // 传递线程编号
        cout << "create thread " << i << endl;
        t->Start(); // 启动线程
        threads.push_back(t);
        this_thread::sleep_for(chrono::microseconds(10)); // 10ms
    }
}

// 分发线程
void XThreadPool::Dispatch(XTask *task)
{
    // 轮询
    if (!task)
    {
        return;
    }
    int tid = (lastThread + 1) % threadCount;
    lastThread = tid;
    XThread *t = threads[tid];

    t->AddTask(task);
    // 激活线程
    t->Activate();
}