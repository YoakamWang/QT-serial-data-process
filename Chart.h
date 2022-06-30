#ifndef CHART_H
#define CHART_H
#include <QWidget>
#include <QTimer>
#include <QChartView>
#include <QValueAxis>
#include <QLineSeries>

#include "qcustomplot.h"
#include "mainwindow.h"

QT_CHARTS_USE_NAMESPACE
//class MainWindow;

namespace Ui {
class Chart;
}

class Chart : public QWidget
{
    Q_OBJECT

public:
    explicit Chart(QWidget *parent = nullptr);
    ~Chart();
    void setupDemo(int demoIndex);

    void setupRealtimeDataDemo(QCustomPlot *customPlot);

private:
    Ui::Chart *ui;
    QString demoName;
    QTimer dataTimer;
    QCPItemTracer *itemDemoPhaseTracer;
    int currentDemoIndex;
    //MainWindow& mwindow;
    //int eround=MainWindow::encoderround1;
    //int mround=MainWindow::motoround1;
    //int bround=MainWindow::biground1;

private slots:
    void slotBtnClear();
    void realtimeDataSlot(/*int eround,int mround,int bround*/);
    //void receiveData(int a,int b,int c);
};

#endif // CHART_H
