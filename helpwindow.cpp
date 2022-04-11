#include "helpwindow.h"
#include "ui_helpwindow.h"

helpWindow::helpWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::helpWindow)
{
    ui->setupUi(this);
}

helpWindow::~helpWindow()
{
    delete ui;
}
void helpWindow::insertImage()
{
    QTextCursor c = ui->textBrowser->textCursor();
    c.movePosition(QTextCursor::End);
    c.insertImage(":resource/images/Example.jpg");
}
