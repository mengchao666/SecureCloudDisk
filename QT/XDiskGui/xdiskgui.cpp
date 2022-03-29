#include "xdiskgui.h"
#include "ui_xdiskgui.h"
#include <QMessageBox>
#include <QFileDialog>
#include <string>
#include <xdiskclient.h>

using namespace std;
XDiskGUI::XDiskGUI(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::XDiskGUI)
{
    ui->setupUi(this);
    XDiskClient::Get()->Init();
}

XDiskGUI::~XDiskGUI()
{
    delete ui;
}

void XDiskGUI::Refresh()
{
    // 服务器路径 服务器IP 服务器端口
    string ip = ui->ipEdit->text().toStdString();
    string root = ui->pathEdit->text().toStdString();
    int port = ui->portBox->value();
    QMessageBox::information(this, "", "Refresh");

    // 1连接服务器
    XDiskClient::Get()->SetServerIP(ip);
    XDiskClient::Get()->SetServerPort(port);
    XDiskClient::Get()->SetServerRoot(root);
    XDiskClient::Get()->GetDir();
    // 2等待回调

}

void XDiskGUI::Upload()
{
    //用户选择一个文件
    QString filename = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("请选择上传文件"));
    if (filename.isEmpty())
    {
        return;
    }
    //插入到列表
    ui->filelistWidget->insertRow(0);// 插入在开头位置
    ui->filelistWidget->setItem(0, 0, new QTableWidgetItem(filename));
    ui->filelistWidget->setItem(0, 1, new QTableWidgetItem(tr("%1Byte").arg(1900)));
}
