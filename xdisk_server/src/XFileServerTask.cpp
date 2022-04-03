#include <XFileServerTask.h>
#include <XTools.h>
#include <iostream>
using namespace std;

string XFileServerTask::m_curDir = "./";
std::mutex XFileServerTask::cur_dir_mux;

// 当关闭消息接收时，数据将发送到此函数，由业务模块重载
void XFileServerTask::ReadCB(void *data, int size)
{
    if (!data || size <= 0 || !m_ofs.is_open())
        return;
    m_ofs.write((char *)data, size);
    m_uploadSize += size;
    if (m_uploadSize == m_fileSize)
    {
        cout << "file wirte end" << endl;
        m_ofs.close();
        XMsg remsg;
        remsg.type = MSG_UPLOAD_COMPLETE;
        remsg.size = 3; // 加1 是发送\0
        remsg.data = (char *)"OK";
        Write(&remsg);
    }
}

// 处理客户端的上传请求
void XFileServerTask::Upload(const XMsg *msg)
{
    cout << "Upload is here" << endl;
    // 1、获取文件名和大小
    if (!msg->data || !msg || msg->size <= 0)
    {
        cout << "msg->data || !msg || msg->size <= 0" << endl;
        return;
    }

    string str = msg->data;
    if (str.empty())
    {
        cout << "str is empty" << endl;
        return;
    }
    int pos = str.find_last_of(',');
    if (pos <= 0)
    {
        cout << "pos <= 0" << endl;
        return;
    }
    string filename = str.substr(0, pos);
    int sizepos = pos + 1;
    if (sizepos >= str.size())
    {
        cout << "sizepos >= str.size()" << endl;
        return;
    }
    string tmp = str.substr(sizepos, str.size() - sizepos);
    cout << filename << ":" << tmp << endl;
    m_fileSize = atoi(tmp.c_str());
    if (m_fileSize <= 0)
    {
        return;
    }
    // 2、打开本地文件
    string filepath = GetCurDir() + filename;
    cout << "filepath = " << filepath << ":" << tmp << endl;
    m_ofs.open(filepath, ios::out | ios::binary);
    if (!m_ofs.is_open())
    {
        cerr << "open filepath error" << endl;
        return;
    }
    cout << "open filepath success" << endl;
    // 3、回复accept
    XMsg remsg;
    remsg.type = MSG_UPLOAD_ACCEPT;
    remsg.size = 3; // 加1 是发送\0
    remsg.data = (char *)"OK";
    Write(&remsg);
    // 不接受消息，开始接收文件
    SetIsRecvMsg(false);
    m_uploadSize = 0;
}

void XFileServerTask::GetDir(const XMsg *msg)
{
    cout << " mengchao  XMsg msg is  = " << (char *)msg->data << endl;
    if (!msg->data)
    {
        cout << "the code is go here" << endl;
        return;
    }
    int len = strlen(msg->data);
    cout << "mengchao len = " << len << endl;
    string root = msg->data;
    if (root.empty())
    {
        cout << "mengchao mengchao the code is go here" << endl;
        root = "../";
    }
    SetCurDir(root);
    // string dir = "file1,1024;file2,4096;file3.zip,10240";
    string dir = GetDirData(root);
    XMsg remsg;
    remsg.type = MSG_DIRLIST;
    remsg.size = dir.size() + 1; // 加1 是发送\0
    remsg.data = (char *)dir.c_str();
    Write(&remsg);
}

void XFileServerTask::ReadCB(const XMsg *msg)
{
    cout << "mengchao test 555555555555555555555555 ReadCB" << endl;
    switch (msg->type)
    {
    case MSG_GETDIR:
        cout << "MSG_GETDIR" << endl;
        GetDir(msg);
        break;
    case MSG_UPLOAD_INFO:
        cout << "MSG_UPLOAD_INFO" << endl;
        Upload(msg);
        break;
    default:
        break;
    }
}