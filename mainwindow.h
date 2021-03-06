#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>
#include <string>
#include <QChartView>
#include <QLineSeries>
#include <QMouseEvent>
#include <utility>
#include <QFileDialog>
#include <QGraphicsSimpleTextItem>
#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"
using namespace QXlsx;
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
    void createAreasOfSurplus(QChart* chart, double mid_price, double mid_volume, QValueAxis* xAxis, QValueAxis* yAxis);
    std::pair<double, double> elasticity(double mid_price, double mid_volume);
    bool addMarkerOfEqual(QChart*chart, QValueAxis* xAxis, QValueAxis* yAxis);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_helpButton_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    QChartView *chartView;
    bool isDefined = false;
    bool needForGenerate = false;
};


#endif // MAINWINDOW_H
