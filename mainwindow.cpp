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

void MainWindow::addMarkerOfEqual(QChart*chart, QValueAxis* xAxis, QValueAxis* yAxis){
    QPointF answer;
    if(ui->Qd_2->value() - ui->Qs_2->value() != 0){
        QScatterSeries *marker = new QScatterSeries();
        marker->setMarkerSize(10);
        double temp = (double)(ui->Qs_1->value() - ui->Qd_1->value()) / (double)(ui->Qd_2->value() - ui->Qs_2->value());
        double temp2 = ui->Qd_1->value() + ui->Qd_2->value() * temp;
        answer = QPointF(temp2, temp);
        if(temp2 > 0){
            marker->setBrush(Qt::green);
        }
        else{
            marker->setBrush(Qt::red);
        }
        marker->append(answer);
        chart->addSeries(marker);
        marker->attachAxis(xAxis);
        marker->attachAxis(yAxis);
    }
    else{
         ui->status->setText("Равновесной цены не обнаружено");
         return;
    }
}

void MainWindow::on_pushButton_clicked()
{
    if(isValid()){
        ui->status->setText("");
        int qd_1 = ui->Qd_1->value();
        int qd_2 = ui->Qd_2->value();
        int qs_1 = ui->Qs_1->value();
        int qs_2 = ui->Qs_2->value();
        QValueAxis* xAxis = new QValueAxis;
        xAxis->setTitleText("Объём товара");
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
        addMarkerOfEqual(chart, xAxis, yAxis);
        chart->legend()->hide();
        chart->setTitle("График");
        chartView->setChart(chart);
    }
    else{
        ui->status->setText("Введите корректные данные");
    }
}
bool MainWindow::isValid()
{
    if((!ui->Qd_1->value() && !ui->Qd_2->value()) || (!ui->Qs_1->value() && !ui->Qs_2->value()) || !ui->horizontalSlider->value()){
        return false;
    }
    else{
        return true;
    }
}

