#include "xdiskgui.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    XDiskGUI w;
    w.show();
    // 处理信号槽
    return a.exec();
}
