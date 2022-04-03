#include <XFileServerTask.h>
#include <XTools.h>
#include <iostream>
using namespace std;

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
    default:
        break;
    }
}