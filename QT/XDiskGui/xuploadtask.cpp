#include "xuploadtask.h"
#include <iostream>
using namespace std;
// 接收到消息的回调
void XUploadTask::ReadCB(const XMsg *msg)
{
    switch (msg->type)
    {
    case MSG_UPLOAD_ACCEPT:
            BeginWrite();
        break;
    case MSG_UPLOAD_COMPLETE:
        //界面刷新
        if (UploadCB)
        {
            UploadCB();
        }
        break;
    default:
        break;
    }
}

void XUploadTask::WriteCB()
{
    if (!m_ifs.is_open())
    {
        return;
    }
    m_ifs.read(readbuffer, sizeof(readbuffer));
    int len = m_ifs.gcount();
    if (len <= 0)
    {
        m_ifs.close();
        return;
    }
    Write(readbuffer, len);
    if (m_ifs.eof())
    {
        m_ifs.close();
    }
}

// 连接成功的消息回调，由业务类重载
void XUploadTask::ConnectedCB()
{
    if (m_filePath.empty())
    {
        cout << "filepaht is empty" << endl;
        return;
    }

    m_ifs.open(m_filePath.c_str(), ios::in | ios::binary | ios::ate); // 二进制形式打开,打开文件跳转到结尾获取文件大小
    if (m_ifs.is_open())
    {
        cerr << "open file " << m_filePath << " failed " << endl;
    }

    // 1、打开本地文件，确保能打开
    // 2、获取文件名称,文件大小filename.zip,1024
    m_fileSize = m_ifs  .tellg();
    m_ifs.seekg(0, ios::beg);// 将文件移动到开头
    cerr << "open file " << m_filePath << " success " << endl;
    string filename;
    int pos = m_filePath.find_last_of('/');
    if (pos < 0)
    {
        pos = 0;
    }
    filename = m_filePath.substr(pos, m_filePath.size() - pos);
    // 3、发送上传文件请求
    char data[1024] = {0};
    sprintf(data, "%s, %d",filename.c_str(), m_fileSize);

    XMsg msg;
    msg.type = MSG_UPLOAD_INFO;
    cout << "mengchao test11111111111  " << endl;
    msg.size = strlen(data) + 1;
    msg.data = data;
    Write(&msg);
}
