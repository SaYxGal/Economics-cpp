#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>
#include <QChartView>
#include <QLineSeries>
#include <QMouseEvent>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    bool isValid();
    void generateLine(QChart* chart, int first, int second, QValueAxis *xAxis,  QValueAxis *yAxis);
    void addMarkerOfEqual(QChart*chart, QValueAxis* xAxis, QValueAxis* yAxis);
private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QChartView *chartView;
};
#endif // MAINWINDOW_H
