#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "helpwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , chartView(new QChartView(this))
{
    ui->setupUi(this);
    ui->pushButton_2->setVisible(false);
    chartView->setMouseTracking(true);
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
    if(needForGenerate == false) {
        series->setColor(Qt::blue);
        series->setName("Функция спроса");
        needForGenerate = true;
    }
    else{
        series->setColor(Qt::red);
        series->setName("Функция предложения");
        needForGenerate = false;
    }
    chart->addSeries(series);
    series->attachAxis(xAxis);
    series->attachAxis(yAxis);
}
//void QChartView::mouseMoveEvent(QMouseEvent *event){
//    const QPoint curPos = event->pos();
//    QPointF curVal = chartView->chart()->mapToValue(QPointF(curPos));
//    QString coordStr = QString("X = %1, Y = %2").arg(curVal.x()).arg(curVal.y());
//    ui->coords->setText(coordStr);
//}
void MainWindow::createAreasOfSurplus(QChart* chart, double mid_price, double mid_volume, QValueAxis* xAxis, QValueAxis* yAxis){
    QLineSeries *series_of_mid_price = new QLineSeries();
    series_of_mid_price->append(ui->Qd_1->value() + ui->Qd_2->value() * ui->horizontalSlider->value(), mid_price);
    series_of_mid_price->append(mid_volume, mid_price);

    QLineSeries *series_for_spros = new QLineSeries();
    series_for_spros->append(ui->Qd_1->value() + ui->Qd_2->value() * ui->horizontalSlider->value(), ui->horizontalSlider->value());
    series_for_spros->append(mid_volume, mid_price);

    QLineSeries *series_for_predlog = new QLineSeries();
    series_for_predlog->append(ui->Qs_1->value(), 0);
    series_for_predlog->append(mid_volume, mid_price);

    QLineSeries *series_of_mid_price_2 = new QLineSeries();
    series_of_mid_price_2->append(ui->Qs_1->value(), mid_price);
    series_of_mid_price_2->append(mid_volume, mid_price);

    QAreaSeries *area1 = new QAreaSeries(series_for_spros, series_of_mid_price);
    QAreaSeries *area2 = new QAreaSeries(series_of_mid_price_2, series_for_predlog);

    area1->setBrush(QColor(127, 255, 212, 100));
    area2->setBrush(QColor(255, 160 ,122, 100));
    area1->setName("Зона излишков потребителя");
    area2->setName("Зона излишков продавца");
    chart->addSeries(area1);
    chart->addSeries(area2);
    area1->attachAxis(xAxis);
    area2->attachAxis(xAxis);
    area1->attachAxis(yAxis);
    area2->attachAxis(yAxis);
}
bool MainWindow::addMarkerOfEqual(QChart*chart, QValueAxis* xAxis, QValueAxis* yAxis){
    QPointF answer;
    if(ui->Qd_2->value() - ui->Qs_2->value() != 0){
        double temp = (double)(ui->Qs_1->value() - ui->Qd_1->value()) / (double)(ui->Qd_2->value() - ui->Qs_2->value());
        double temp2 = ui->Qd_1->value() + ui->Qd_2->value() * temp;
        QScatterSeries *marker = new QScatterSeries();
        marker->setMarkerSize(10);
        answer = QPointF(temp2, temp);
        if(temp2 > 0 && temp > 0 && ((ui->Qs_1->value() + ui->Qs_2->value() * ui->horizontalSlider->value()) >= temp2) && ui->horizontalSlider->value() >= temp){
            isDefined = true;
            marker->setBrush(Qt::green);
            std::pair<double, double> elastic = elasticity(temp, temp2);
            ui->status->setText("<html>Равновесная цена = " + QString::number(temp) + "<br>Равновесный объём = " + QString::number(temp2) + "<br>Эластичность спроса по цене = " + QString::number(elastic.first) + "<br>Эластичность предложения по цене = " + QString::number(elastic.second) + "</html>");
        }
        else{
            marker->setBrush(Qt::red);
            ui->status->setText("Не определено");
        }
        marker->append(answer);
        chart->addSeries(marker);
        marker->attachAxis(xAxis);
        marker->attachAxis(yAxis);
        return true;
    }
    else{
         ui->status->setText("Равновесной цены не обнаружено");
         return false;
    }
}

std::pair<double, double> MainWindow::elasticity(double mid_price, double mid_volume){
    return std::make_pair(abs((ui->Qd_2->value() * mid_price) / mid_volume), abs((ui->Qs_2->value() * mid_price) / mid_volume));
}

void MainWindow::on_pushButton_clicked()
{
    if(isValid()){
        ui->pushButton_2->setVisible(true);
        ui->status->setText("");
        int qd_1 = ui->Qd_1->value();
        int qd_2 = ui->Qd_2->value();
        int qs_1 = ui->Qs_1->value();
        int qs_2 = ui->Qs_2->value();
        QValueAxis* xAxis = new QValueAxis;
        xAxis->setTitleText("Объём товара (Q, ед)");
        xAxis->setTickCount(10);
        xAxis->setRange(0, int(qs_1 + qs_2 * ui->horizontalSlider->value())+ 10);
        xAxis->setLabelFormat("% .1f");
        QValueAxis* yAxis = new QValueAxis;
        yAxis->setTitleText("Цена (P, руб)");
        yAxis->setTickCount(10);
        yAxis->setRange(0, ui->horizontalSlider->value() + 10);
        yAxis->setLabelFormat("% .1f");
        QChart *chart = new QChart();
        chart->addAxis(xAxis, Qt::AlignBottom);
        chart->addAxis(yAxis, Qt::AlignLeft);
        if(ui->Qd_2->value() - ui->Qs_2->value() != 0){
            double temp = (double)(ui->Qs_1->value() - ui->Qd_1->value()) / (double)(ui->Qd_2->value() - ui->Qs_2->value());
            double temp2 = ui->Qd_1->value() + ui->Qd_2->value() * temp;
             if(temp2 > 0 && temp > 0 && ((ui->Qs_1->value() + ui->Qs_2->value() * ui->horizontalSlider->value()) > temp2) && ui->Qd_2->value() < 0 && ui->Qs_2->value() > 0){
                createAreasOfSurplus(chart,temp, temp2, xAxis, yAxis);
             }
        }
        generateLine(chart, qd_1, qd_2, xAxis, yAxis);
        generateLine(chart, qs_1, qs_2, xAxis, yAxis);
        addMarkerOfEqual(chart, xAxis, yAxis);
        //chart->legend()->hide();
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


void MainWindow::on_pushButton_2_clicked()
{
    Document xlsx;
    Format format;
    format.setBorderStyle(Format::BorderMediumDashDot);
    format.setFontColor(Qt::red);
    format.setFontBold(true);
    xlsx.write(1, 2, 0);
    xlsx.write(2, 2, ui->Qd_1->value()); //начальная точка графика спроса
    xlsx.write(1, 3, ui->horizontalSlider->value());
    xlsx.write(2, 3, ui->Qd_1->value() + ui->Qd_2->value() * ui->horizontalSlider->value()); //конеч. точка графика спроса

    xlsx.write(3, 2, 0);
    xlsx.write(4, 2, ui->Qs_1->value());
    xlsx.write(3, 3, ui->horizontalSlider->value());
    xlsx.write(4, 3, ui->Qs_1->value() + ui->Qs_2->value() * ui->horizontalSlider->value());

    if(isDefined == true){
        double temp = (double)(ui->Qs_1->value() - ui->Qd_1->value()) / (double)(ui->Qd_2->value() - ui->Qs_2->value());
        xlsx.write("D21", "Равновесный объём");
        xlsx.write("D22", ui->Qd_1->value() + ui->Qd_2->value() * temp, format);
        xlsx.write("G21", "Равновесная цена");
        xlsx.write("G22", temp, format);
        std::pair<double, double> numbers = elasticity(temp, ui->Qd_1->value() + ui->Qd_2->value() * temp);
        xlsx.write("J21", "Эластичность спроса по цене");
        xlsx.write("J22", numbers.first, format);
        xlsx.write("J23", "Эластичность предложения по цене");
        xlsx.write("J24", numbers.second, format);
    }

    Chart *lineChart = xlsx.insertChart(0, 3, QSize(600, 400));
    lineChart->setChartType(Chart::CT_LineChart);
    lineChart->setChartTitle("Результат");
    lineChart->setAxisTitle(Chart::ChartAxisPos::Bottom, "Цена");
    lineChart->setAxisTitle(Chart::ChartAxisPos::Left, "Объём товара");
    lineChart->addSeries(CellRange(1,1,2,3), NULL, true, true, false);
    lineChart->addSeries(CellRange(3, 1, 4, 3), NULL, true, true, false);
    xlsx.saveAs("chart1.xlsx");
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->horizontalSlider->setToolTip(QString::fromStdString(std::to_string(value)));
}


void MainWindow::on_helpButton_clicked()
{
    helpWindow *help = new helpWindow();
    help->setModal(true);
    help->show();
}

