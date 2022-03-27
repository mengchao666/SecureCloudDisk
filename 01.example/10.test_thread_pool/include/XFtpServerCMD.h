#include <XTask.h>
class XFtpServerCMD : public XTask // 接收FTP命令类
{
public:
    //初始化任务
    virtual bool Init();
};