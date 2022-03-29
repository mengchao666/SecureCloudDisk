#ifndef XCOMTASK_H
#define XCOMTASK_H
#include <XTask.h>
#include <event2/bufferevent.h>
#include <string>
class XComTask : public XTask
{
public:
    //初始化任务
    bool Init();
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

private:
    bufferevent *m_bev;
    std::string m_serverIP;
    int m_serverPort = 0;
};
#endif