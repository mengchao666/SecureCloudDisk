
#ifndef XSERVERTASK_H
#define XSERVERTASK_H
#include <XTask.h>

typedef void (*ListenCBFunc)(int sock, struct sockaddr *addr, int socklen, void *arg);
class XServerTask : public XTask
{
public:
    virtual bool Init();
    ListenCBFunc ListenCB = 0;
    void SetServerPort(int port)
    {
        m_serverPort = port;
    }

private:
    int m_serverPort;
};
#endif