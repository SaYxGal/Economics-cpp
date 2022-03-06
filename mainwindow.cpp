#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , chartView(new QChartView(this))
{
    ui->setupUi(this);
    ui->mainLayout->addWidget(chartView);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    if(isValid()){
        int qd_1 = ui->Qd_1->value();
        int qd_2 = ui->Qd_2->value();
        QValueAxis* xAxis = new QValueAxis;
        xAxis->setTitleText("Спрос и предложение");
        xAxis->setRange(0, (qd_1 - qd_2 * ui->horizontalSlider->value())+ 50);
        QValueAxis* yAxis = new QValueAxis;
        yAxis->setTitleText("Цена");
        yAxis->setRange(0, ui->horizontalSlider->value() + 50);
        QLineSeries *series = new QLineSeries();
        series->append(qd_1,0);
        series->append(qd_1 - qd_2 * ui->horizontalSlider->value(), ui->horizontalSlider->value());
//        series->append(1, 1);
//        series->append(2, 4);
//        series->append(10, 2);

        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->addAxis(xAxis, Qt::AlignBottom);
        series->attachAxis(xAxis);
        chart->addAxis(yAxis, Qt::AlignLeft);
        series->attachAxis(yAxis);
        chart->legend()->hide();
        chart->setTitle("График");
        chartView->setChart(chart);
    }
    else{

    }
}
bool MainWindow::isValid()
{
    return true;
}

