#ifndef PROCESSDATAPLOT_H
#define PROCESSDATAPLOT_H

#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QLineSeries>
#include <QAreaSeries>

#include <QWidget>
#include <QTimer>

#include <sys/resource.h>

// sudo apt-get install libqt5chars5-dev

using namespace QtCharts;

class ProcessDataPlot : public QChartView {
Q_OBJECT
public:
    explicit ProcessDataPlot(QWidget *parent = nullptr);

    ~ProcessDataPlot() = default;

    // stops, when process no longer exists, rusage returns -1
    // updates process data every 1000ms
    // calling only .start() when process was run.
    QTimer *timer;

private:
    QChart *MainChart;


    void createWindow();

private slots:

    void getMemoryData();


};

#endif // PROCESSDATAPLOT_H
