#ifndef XTHREADPOOL_H
#define XTHREADPOOL_H
#include <vector>

class XThread;
class XTask;
class XThreadPool
{
private:
    int threadCount = 0; // 线程数量
    int lastThread = -1;
    std::vector<XThread *> threads;

public:
    // 单例模式
    static XThreadPool *Get()
    {
        static XThreadPool p;
        return &p;
    }

    // 初始化所有线程并启动线程
    void Init(int threadCount);

    // 分发线程
    void Dispatch(XTask *task);
};
#endif