#include <event2/bufferevent.h>
#include <event2/event.h>
#include <list>
#include <mutex>
class XTask;
class XThread
{
public:
    // 启动线程
    void Start();

    // 线程入口函数
    void Main();

    //安装线程，初始化event_base和管道监听事件，用于激活线程
    bool Setup();

    // 收到主线程发出的激活消息，（线程池的分发调用）
    void Notify(evutil_socket_t fd, short which);

    // 线程激活
    void Activate();

    // 添加处理的任务，一个线程可以同时处理多个任务，共用一个event_base
    void AddTask(XTask *t);

    int id = 0; // 线程编号
private:
    int notify_send_fd = 0;
    event_base *base = 0;

    std::list<XTask *> tasks; // 任务列表
    std::mutex tasks_mutex;   // 线程安全互斥
};