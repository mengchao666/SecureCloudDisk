#include <XComTask.h>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <iostream>
using namespace std;

static void SReadCB(struct bufferevent *bev, void *ctx)
{
    cout << "mengchao test 3333333333333333333333333 SReadCB" << endl;
    auto task = (XComTask *)ctx;
    task->ReadCB();
}

static void SWriteCB(struct bufferevent *bev, void *ctx)
{
    auto task = (XComTask *)ctx;
    task->WriteCB();
}

bool XComTask::Write(const void *data, int size)
{
    if (!m_bev || !data || size <= 0)
    {
        return false;
    }
    int re = bufferevent_write(m_bev, data, size);
    if (re != 0)
    {
        return false;
    }
    return true;
}

bool XComTask::Write(const XMsg *msg)
{
    if (!m_bev || !msg || !msg->data || msg->size <= 0)
    {
        return false;
    }

    // 写入消息头
    int re = bufferevent_write(m_bev, msg, sizeof(XMsgHead));
    if (re != 0)
    {
        return false;
    }
    // 写入消息内容
    re = bufferevent_write(m_bev, msg->data, msg->size);
    if (re != 0)
    {
        return false;
    }
    return true;
}

// 激活写入回调
void XComTask::BeginWrite()
{
    if (!m_bev)
        return;
    bufferevent_trigger(m_bev, EV_WRITE, 0);
}

void XComTask::ReadCB(const XMsg *msg)
{
    cout << "mengchao test 2222222222222222222222222 ReadCB" << endl;
    cout << "recv Msg " << msg->type << "size = " << msg->size << endl;
}

void XComTask::ReadCB()
{
    cout << "mengchao test 111111111111111111111111 ReadCB" << endl;
    for (;;) // 确保边缘触发时能读到所有数据
    {
        if (!m_isRecvMsg)
        {
            int len = bufferevent_read(m_bev, readbuffer, sizeof(readbuffer));
            if (len <= 0)
            {
                return;
            }
            ReadCB(readbuffer, len);
            continue;
        }
        // 接收消息XMsgHead
        // 接收头部信息
        if (!m_msg.data)
        {
            int headsize = sizeof(XMsgHead);
            int len = bufferevent_read(m_bev, &m_msg, headsize);
            if (len <= 0)
            {
                return;
            }
            if (len != headsize)
            {
                cerr << "msg head recv error" << endl;
                return;
            }

            // 验证消息的有效性
            if (m_msg.type >= MSG_MAX_TYPE || m_msg.size < 0 || m_msg.size > MSG_MAX_SIZE)
            {
                cout << "m_msg.type = " << m_msg.type << endl;
                cout << "mm_msg.size = " << m_msg.size << endl;
                cerr << "msg head is error" << endl;
                return;
            }
            m_msg.data = new char[m_msg.size];
        }
        int readsize = m_msg.size - m_msg.recved;
        if (readsize <= 0)
        {
            delete m_msg.data;
            memset(&m_msg, 0, sizeof(m_msg));
            return;
        }
        int length = bufferevent_read(m_bev, m_msg.data + m_msg.recved, readsize);
        if (length <= 0)
        {
            return;
        }
        m_msg.recved += length;
        if (m_msg.recved == m_msg.size)
        {
            // 处理消息
            cout << "recved msg size = " << m_msg.size << endl;
            ReadCB(&m_msg);
            delete m_msg.data;
            memset(&m_msg, 0, sizeof(m_msg));
        }
    }
}

void XComTask::EventCB(short what)
{
    cout << "SEventCB what is " << what << endl;
    if (what & BEV_EVENT_CONNECTED)
    {
        cout << "connect is success" << endl;
        // bufferevent_write(m_bev, "OK", 3);
        // 通知连接成功
        ConnectedCB();
    }

    if (what & BEV_EVENT_ERROR || what & BEV_EVENT_TIMEOUT)
    {
        cout << "BEV_EVENT_ERROR or BEV_EVENT_TIMEOUT" << endl;
        bufferevent_free(m_bev);
    }

    if (what & BEV_EVENT_EOF)
    {
        cout << "BEV_EVENT_EOF" << endl;
        bufferevent_free(m_bev);
    }
}

static void SEventCB(struct bufferevent *bev, short what, void *ctx)
{
    auto task = (XComTask *)ctx;
    task->EventCB(what);
}

//初始化任务
bool XComTask::Init()
{
    int comsock = this->sockfunc();
    if (comsock <= 0)
    {
        comsock = -1;
    }
    // 建立连接bufferevent
    m_bev = bufferevent_socket_new(basefunc(), comsock, BEV_OPT_CLOSE_ON_FREE); // -1自动创建socket
    if (!m_bev)
    {
        cerr << "bufferevent_socket_new error " << endl;
    }

    cout << "mengchaotest 66666666666666666666666" << endl;

    // 设置回调
    bufferevent_setcb(m_bev, SReadCB, SWriteCB, SEventCB, this);

    bufferevent_enable(m_bev, EV_READ | EV_WRITE);

    timeval tv = {10, 0};
    bufferevent_set_timeouts(m_bev, &tv, &tv);

    // 连接服务器
    if (m_serverIP.empty())
    {
        return true;
    }
    sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(m_serverPort);
    evutil_inet_pton(AF_INET, m_serverIP.c_str(), &sin.sin_addr.s_addr);
    int re = bufferevent_socket_connect(m_bev, (sockaddr *)&sin, sizeof(sin));
    if (re != 0)
    {
        return false;
    }
    return true;
}