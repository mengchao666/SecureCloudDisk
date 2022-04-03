#ifndef XUPLOADTASK_H
#define XUPLOADTASK_H
#include <XComTask.h>
#include <fstream>
typedef void(*UploadCBFunc)();

class XUploadTask:public XComTask
{
public:
    // 接收到消息的回调
    virtual void ReadCB(const XMsg *msg);

    // 连接成功的消息回调，由业务类重载
    virtual void ConnectedCB();

    virtual void WriteCB();

    void SetFilePath(std::string filePath)
    {
        this->m_filePath = filePath;
    }

    // 上传成功后的回调函数
    UploadCBFunc UploadCB;
private:
    std::string m_filePath = ""; // 需要上传的文件路径
    std::ifstream m_ifs; // 读取文件
    int m_fileSize = 0;//暂时不支持大于2G的文件
};

#endif // XUPLOADTASK_H
