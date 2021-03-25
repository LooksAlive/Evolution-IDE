#include <QDebug>

#include "ProcessDataPlot.h"


ProcessDataPlot::ProcessDataPlot(QWidget *parent) {
    setRenderHint(QPainter::Antialiasing);
    setVisible(true);
    setMaximumWidth(300);
    createWindow();

    setChart(MainChart);
}

void ProcessDataPlot::getMemoryData() {
    int who = RUSAGE_SELF;
    struct rusage usage;
    const int ret = getrusage(who, &usage);
    if (ret == -1) {
        qDebug() << "rusage error";
        timer->stop();
    }
}

void ProcessDataPlot::createWindow() {
    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, &QTimer::timeout, this, &ProcessDataPlot::getMemoryData);
    MainChart = new QChart();

    QLineSeries *series0 = new QLineSeries();
    QLineSeries *series1 = new QLineSeries();

    *series0 << QPointF(1, 5) << QPointF(3, 7) << QPointF(7, 6) << QPointF(9, 7) << QPointF(12, 6)
             << QPointF(16, 7) << QPointF(18, 5);
    *series1 << QPointF(1, 3) << QPointF(3, 4) << QPointF(7, 3) << QPointF(8, 2) << QPointF(12, 3)
             << QPointF(16, 4) << QPointF(18, 3);

    QAreaSeries *series = new QAreaSeries(series0, series1);
    // series->setName("Batman");
    QPen pen(0x059605);
    pen.setWidth(3);
    series->setPen(pen);

    QLinearGradient gradient(QPointF(0, 0), QPointF(0, 1));
    gradient.setColorAt(0.0, 0x3cc63c);
    gradient.setColorAt(1.0, 0x26f626);
    gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
    series->setBrush(gradient);


    MainChart->addSeries(series);
    MainChart->setTitle("Memory Usage");
    MainChart->createDefaultAxes();
    // MainChart->setAxisY();
    MainChart->axes(Qt::Horizontal).first()->setRange(0, 20);
    MainChart->axes(Qt::Vertical).first()->setRange(0, 10);
}
