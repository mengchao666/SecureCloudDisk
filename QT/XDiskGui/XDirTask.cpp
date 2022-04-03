#include <XDirTask.h>
#include <iostream>
using namespace std;

void XDirTask::ReadCB(const XMsg *msg)
{
    cout << "mengchao test 44444444444444444444444 SReadCB" << endl;
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
    cout << "mengchao test11111111111 m_root2 = " << m_root2 << endl;
    msg.size = m_root2.size() + 1;
    msg.data = (char *)m_root2.c_str();
    Write(&msg);
}
