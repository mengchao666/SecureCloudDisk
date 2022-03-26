#include <XThread.h>
#include <XThreadPool.h>
#include <iostream>
#include <thread>

using namespace std;

void XThreadPool::Init(int threadCount)
{
    this->threadCount = threadCount;
    for (int i = 0; i < threadCount; i++)
    {
        XThread *t = new XThread();
        cout << "create thread " << i << endl;
        threads.push_back(t);
        this_thread::sleep_for(chrono::microseconds(10)); // 10ms
    }
}