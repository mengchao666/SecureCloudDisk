#ifndef XCOMTASK_H
#define XCOMTASK_H
#include <XMsg.h>
#include <XTask.h>
#include <event2/bufferevent.h>
#include <string>
class XComTask : public XTask
{
public:
    //初始化任务
    virtual bool Init();
    // 服务器IP
    void SetServerIP(std::string ip)
    {
        this->m_serverIP = ip;
    }

    // 服务器端口
    void SetServerPort(int port)
    {
        this->m_serverPort = port;
    }

    void SetIsRecvMsg(bool isRecvMsg)
    {
        this->m_isRecvMsg = isRecvMsg;
    }

    // 接收到消息的回调，由业务类重载
    virtual bool Write(const XMsg *msg);

    // 当关闭消息接收时，数据将发送到此函数，由业务模块重载
    virtual void ReadCB(void *data, int size) {}

    // 激活写入回调
    virtual void BeginWrite();

    // 连接成功的消息回调，由业务类重载
    virtual void ConnectedCB() {}

    virtual void EventCB(short what);

    virtual void ReadCB(const XMsg *msg);

    virtual void ReadCB();

    virtual void WriteCB(){};

    virtual bool Write(const void *data, int size); //发送消息

protected:
    char readbuffer[4096] = {0};

private:
    bufferevent *m_bev;
    std::string m_serverIP;
    int m_serverPort = 0;
    XMsg m_msg;              // 数据包缓存
    bool m_isRecvMsg = true; // 是否接收消息，接收消息会调用virtual void ReadCB(const XMsg *msg);
    // 不接受调用void ReadCB(void *data, int size) {}
};
#endif