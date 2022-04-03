#include "xdiskclient.h"
#include <iostream>
#include "XComTask.h"
#include "XThreadPool.h"
#include "XDirTask.h"
#include "xuploadtask.h"

using namespace std;

// 初始化，包括线程池
bool XDiskClient::Init()
{
    XThreadPool::Get()->Init(10);
    return true;
}

static void DirCB(string dirs)
{
    cout << dirs << endl;
    XDiskClient::Get()->SDir(dirs);
}


static void UploadCB()
{
    cout << "UploadCB" << endl;
    XDiskClient::Get()->SUploadComplete();
}


// 获取目录的文件列表，只是请求消息到服务端
// @para path 目录路径
void XDiskClient::GetDir()
{
    auto task = new XDirTask();
    task->SetServerIP(m_serverIP);
    task->SetServerPort(m_serverPort);
    task->SetServerRoot(m_root);
    task->DirCB = DirCB;
    XThreadPool::Get()->Dispatch(task);

    //这时现在不能操作，task未初始化 task没有event_base
}

// 上传文件请求
// @para filepath 文件路径
void XDiskClient::Upload(std::string filepath)
{
    auto task = new XUploadTask();
    task->SetServerIP(m_serverIP);
    task->SetServerPort(m_serverPort);
    task->SetFilePath(filepath);
    task->UploadCB = UploadCB;
    XThreadPool::Get()->Dispatch(task);
}
