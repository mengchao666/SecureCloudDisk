#include "xdiskclient.h"
#include <iostream>
#include "XComTask.h"
#include "XThreadPool.h"
#include "XDirTask.h"

using namespace std;

// 初始化，包括线程池
bool XDiskClient::Init()
{
    XThreadPool::Get()->Init(10);
    return true;
}

void DirCB(string dirs)
{
    cout << dirs << endl;
    XDiskClient::Get()->SDir(dirs);
}


// 获取目录的文件列表，只是请求消息到服务端
// @para path 目录路径
void XDiskClient::GetDir()
{
    cout << "GetDir is here" << endl;
    cout << "server ip is " << m_serverIP << endl;
    cout << "server port is " << m_serverPort << endl;
    cout << "server root is " << m_root << endl;
    auto task = new XDirTask();
    task->SetServerIP(m_serverIP);
    task->SetServerPort(m_serverPort);
    task->SetServerIP(m_root);
    task->DirCB = DirCB;
    XThreadPool::Get()->Dispatch(task);

    //这时现在不能操作，task未初始化 task没有event_base
}
