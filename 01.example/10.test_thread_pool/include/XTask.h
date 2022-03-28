#ifndef XTASK_H
#define XTASK_H
#include <event2/event.h>
class XTask
{
public:
    event_base *base = 0;
    int sock = 0;
    int thread_id = 0;

    //初始化任务
    virtual bool Init() = 0;
};
#endif