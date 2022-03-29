#include "xdiskgui.h"
#include <signal.h>
#include <QApplication>

int main(int argc, char *argv[])
{
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
    {
        return 1;
    }
    QApplication a(argc, argv);
    XDiskGUI w;
    w.show();
    // 处理信号槽
    return a.exec();
}
