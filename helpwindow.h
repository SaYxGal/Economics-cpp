#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QWidget>
#include <QDialog>

namespace Ui {
class helpWindow;
}

class helpWindow : public QDialog
{
    Q_OBJECT

public:
    explicit helpWindow(QWidget *parent = nullptr);
    void insertImage();
    ~helpWindow();

private:
    Ui::helpWindow *ui;
};

#endif // HELPWINDOW_H
