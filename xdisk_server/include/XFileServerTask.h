#ifndef XFILESERVERTASK_H
#define XFILESERVERTASK_H
#include <XComTask.h>

class XFileServerTask : public XComTask
{
public:
    virtual void ReadCB(const XMsg *msg);

private:
    // 处理目录获取的消息，返回目录列表
    void GetDir(const XMsg *msg);
};
#endif