#ifndef XFILESERVERTASK_H
#define XFILESERVERTASK_H
#include <XComTask.h>
#include <fstream>
#include <mutex>

class XFileServerTask : public XComTask
{
public:
    virtual void ReadCB(const XMsg *msg);

    virtual void ReadCB(void *data, int size);

    static void SetCurDir(std::string curDir)
    {
        cur_dir_mux.lock();
        m_curDir = curDir;
        cur_dir_mux.unlock();
    }

    static std::string GetCurDir()
    {
        cur_dir_mux.lock();
        std::string dir = m_curDir;
        cur_dir_mux.unlock();
        return dir;
    }

private:
    // 处理目录获取的消息，返回目录列表
    void GetDir(const XMsg *msg);

    // 处理客户端的上传请求
    void Upload(const XMsg *msg);

    // 当关闭消息接收时，数据将发送到此函数，由业务模块重载

    int m_fileSize = 0;          // 文件大小
    static std::string m_curDir; // 当前路径
    static std::mutex cur_dir_mux;

    //写入上传的文件
    std::ofstream m_ofs;

    int m_uploadSize = 0; // 已经上传的文件大小
};
#endif