#ifndef XDIRTASK_H
#define XDIRTASK_H
#include <XComTask.h>
typedef void(*DirCBFunc)(std::string dirs);
class XDirTask : public XComTask
{
public:
    virtual void ReadCB(const XMsg *msg);

    // 连接成功的消息回调，由业务类重载
    virtual void ConnectedCB();

    // 获取目录后调用的回调
    DirCBFunc DirCB = 0;

    // 服务器路径
    void SetServerRoot(std::string root)
    {
        this->m_root2 = root;
    }
private:
    std::string m_root2;
};

#endif // XDIRTASK_H
