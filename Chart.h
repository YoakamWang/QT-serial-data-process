#ifndef CHART_H
#define CHART_H
#include <QWidget>
#include <QTimer>
#include <QChartView>
#include <QValueAxis>
#include <QLineSeries>

#include "qcustomplot.h"

QT_CHARTS_USE_NAMESPACE

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

private slots:
    void slotBtnClear();
    void realtimeDataSlot();
};

#endif // CHART_H
