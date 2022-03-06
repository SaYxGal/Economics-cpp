#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , chartView(new QChartView(this))
{
    ui->setupUi(this);
    ui->mainLayout->addWidget(chartView);
    chartView->setRenderHint(QPainter::Antialiasing);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::generateLine(QChart *chart, int first, int second, QValueAxis* xAxis, QValueAxis* yAxis){
    QLineSeries *series = new QLineSeries();
    series->append(first,0);
    series->append(first + second * ui->horizontalSlider->value(), ui->horizontalSlider->value());
    chart->addSeries(series);
    series->attachAxis(xAxis);
    series->attachAxis(yAxis);
}

void MainWindow::on_pushButton_clicked()
{
    if(isValid()){
        int qd_1 = ui->Qd_1->value();
        int qd_2 = ui->Qd_2->value();
        int qs_1 = ui->Qs_1->value();
        int qs_2 = ui->Qs_2->value();
        QValueAxis* xAxis = new QValueAxis;
        xAxis->setTitleText("Спрос и предложение");
        xAxis->setTickCount(10);
        xAxis->setRange(0, (qd_1 + qd_2 * ui->horizontalSlider->value())+ 50);
        QValueAxis* yAxis = new QValueAxis;
        yAxis->setTitleText("Цена");
        yAxis->setTickCount(10);
        yAxis->setRange(0, ui->horizontalSlider->value() + 50);
        QChart *chart = new QChart();
        chart->addAxis(xAxis, Qt::AlignBottom);
        chart->addAxis(yAxis, Qt::AlignLeft);
        generateLine(chart, qd_1, qd_2, xAxis, yAxis);
        generateLine(chart, qs_1, qs_2, xAxis, yAxis);
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

