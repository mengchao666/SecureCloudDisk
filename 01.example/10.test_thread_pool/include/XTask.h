#ifndef XTASK_H
#define XTASK_H
#include <event2/event.h>
class XTask
{
public:
//初始化任务
    virtual bool Init() = 0;
    
    void set_sock(int sock)
    {
        this->sock = sock;
    }

    void set_threadid(int thread_id)
    {
        this->thread_id = thread_id;
    }

    int thread_idfunc()
    {
        return thread_id;
    }

    int sockfunc()
    {
        return sock;
    }

    event_base *basefunc()
    {
        return base;
    }

    void set_base(event_base *base)
    {
        this->base = base;
    }

private:
    event_base *base = 0;
    int sock = 0;
    int thread_id = 0;

    
};
#endif