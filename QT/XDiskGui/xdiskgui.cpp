#include "xdiskgui.h"
#include "ui_xdiskgui.h"
#include <QMessageBox>
#include <QFileDialog>

XDiskGUI::XDiskGUI(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::XDiskGUI)
{
    ui->setupUi(this);
}

XDiskGUI::~XDiskGUI()
{
    delete ui;
}

void XDiskGUI::Refresh()
{
    QMessageBox::information(this, "", "Refresh");
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
