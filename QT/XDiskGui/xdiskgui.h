#ifndef XDISKGUI_H
#define XDISKGUI_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class XDiskGUI; }
QT_END_NAMESPACE

class XDiskGUI : public QWidget
{
    Q_OBJECT

public:
    XDiskGUI(QWidget *parent = nullptr);
    ~XDiskGUI();
public slots:
    void Refresh();
    void UpdateDir(std::string dirs);
    void Upload();

private:
    Ui::XDiskGUI *ui;
};
#endif // XDISKGUI_H
