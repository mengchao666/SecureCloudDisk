#include <XDirTask.h>
#include <iostream>
using namespace std;

void XDirTask::ReadCB(const XMsg *msg)
{
    // 接收到服务端发送的目录
    switch(msg->type)
    {
        case MSG_DIRLIST:
            cout << "MSC_DIRLIST" << endl;
            DirCB(msg->data);
            break;
        default:
            break;
    }
}

// 连接成功的消息回调，由业务类重载
void XDirTask::ConnectedCB()
{
    XMsg msg;
    msg.type = MSG_GETDIR;
    msg.size = m_root.size() + 1;
    msg.data = (char *)m_root.c_str();
    Write(&msg);
}
