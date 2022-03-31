#ifndef XMSG_H
#define XMSG_H
#define MSG_MAX_SIZE 1000000 // 消息最大字节数， 1M
enum MsgType
{
    MSG_NONE = 0,
    MSG_GETDIR,   // 请求目录
    MSG_DIRLIST,  // 返回目录列表
    MSG_MAX_TYPE, //用来验证类型正确
};

// 约定每个消息必须包含内容，没有内容使用OK
struct XMsgHead
{
    MsgType type;
    int size = 0;
};

struct XMsg : public XMsgHead
{
    char *data = 0; // 存储消息内容
    int recved = 0; //已经接收到的消息大小字节数
};
#endif