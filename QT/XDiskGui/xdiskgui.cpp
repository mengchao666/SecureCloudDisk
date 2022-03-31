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

    // 注册信号支持的类型
    qRegisterMetaType<std::string>("std::string");
    // 绑定目录获取的信号
    QObject::connect(XDiskClient::Get(), SIGNAL(SDir(std::string)), this, SLOT(UpdateDir(std::string)));
}

XDiskGUI::~XDiskGUI()
{
    delete ui;
}

void XDiskGUI::UpdateDir(std::string dirs)
{
//    QMessageBox::information(this, "", dirs.c_str());
    // "file1,1024;file2,4096;file3.zip,10240";
    QString str = dirs.c_str();
    str = str.trimmed(); // ?
    if (str.isEmpty())
    {
        return;
    }
    QStringList filestr = str.split(';');
    ui->filelistWidget->setRowCount(filestr.size());
    for (int i = 0; i < filestr.size(); i++)
    {
        QStringList fileinfo = filestr[i].split(',');
        if (fileinfo.size() != 2)
            continue;
        ui->filelistWidget->setItem(i, 0, new QTableWidgetItem(fileinfo[0]));
        ui->filelistWidget->setItem(i, 1, new QTableWidgetItem(tr("%1Byte").arg(fileinfo[1])));
    }
}

void XDiskGUI::Refresh()
{
    // 服务器路径 服务器IP 服务器端口
    string ip = ui->ipEdit->text().toStdString();
    string root = ui->pathEdit->text().toStdString();
    int port = ui->portBox->value();
//    QMessageBox::information(this, "", "Refresh");

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
