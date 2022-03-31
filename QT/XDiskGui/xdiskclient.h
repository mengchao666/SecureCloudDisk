#ifndef XDISKCLIENT_H
#define XDISKCLIENT_H
#include <string>
#include <QObject>

class XDiskClient : public QObject
{
    Q_OBJECT

public:
    // 单例模式
    static XDiskClient *Get()
    {
        static XDiskClient xc;
        return &xc;
    }

    // 初始化，包括线程池
    bool Init();
    // 获取目录的文件列表，只是请求消息到服务端
    // @para path 目录路径
    void GetDir();

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

    // 服务器路径
    void SetServerRoot(std::string root)
    {
        this->m_root = root;
    }

signals:
    void SDir(std::string dirs);

private:
    XDiskClient() = default;//为啥写个=default就能编过？下面两行就编译不过 MacOS M1Pro
    // XDiskClient();
    // XDiskClient(){}
    std::string m_serverIP;
    int m_serverPort = 0;
    std::string m_root;

};

#endif // XDISKCLIENT_H
