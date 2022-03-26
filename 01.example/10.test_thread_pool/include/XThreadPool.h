#include <vector>

class XThread;
class XThreadPool
{
private:
    int threadCount = 0; // 线程数量
    std::vector<XThread *> threads;
    XThreadPool(){};

public:
    // 单例模式
    static XThreadPool *Get()
    {
        static XThreadPool p;
        return &p;
    }

    // 初始化所有线程并启动线程
    void Init(int threadCount);
};