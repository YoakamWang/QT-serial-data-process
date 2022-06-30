#include "Chart.h"
#include "ui_chart.h"
#include "mainwindow.h"

#include <QDebug>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#  include <QDesktopWidget>
#endif
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>

int MainWindow::encoderround1;    //catch the data from MainWindow.
int MainWindow::motoround1;
int MainWindow::biground1;
Chart::Chart(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Chart)
{

  ui->setupUi(this);
  setGeometry(400, 250, 542, 390);
  QObject::connect(ui->clearButton, SIGNAL(clicked(bool)), this, SLOT(slotBtnClear()));
  setupDemo(0);

  setupRealtimeDataDemo(ui->customPlot);

}

void Chart::setupDemo(int demoIndex)
{
  switch (demoIndex)
  {
    case 0:setupRealtimeDataDemo(ui->customPlot); break;

  }
  setWindowTitle("QCustomPlot: "+demoName);
  //statusBar()->clearMessage();
  currentDemoIndex = demoIndex;
  ui->customPlot->replot();
}


void Chart::setupRealtimeDataDemo(QCustomPlot *customPlot)
{
  demoName = "Real Time Data Demo";

  // include this section to fully disable antialiasing for higher performance:
  /*
  customPlot->setNotAntialiasedElements(QCP::aeAll);
  QFont font;
  font.setStyleStrategy(QFont::NoAntialias);
  customPlot->xAxis->setTickLabelFont(font);
  customPlot->yAxis->setTickLabelFont(font);
  customPlot->legend->setFont(font);
  */
  customPlot->addGraph(); // blue line
  customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));
  customPlot->addGraph(); // red line
  customPlot->graph(1)->setPen(QPen(QColor(255, 110, 40)));
  customPlot->addGraph(); // green line
  customPlot->graph(2)->setPen(QPen(QColor(10, 255, 40)));
  QSharedPointer<QCPAxisTickerDateTime> timeTicker(new QCPAxisTickerDateTime);
  //timeTicker->setTimeFormat("%h:%m:%s");
  timeTicker->setDateTimeFormat("dd.MM.yy hh:mm:ss");
  customPlot->xAxis->setTicker(timeTicker);
  customPlot->xAxis->setTickLabelRotation(30);

  customPlot->axisRect()->setupFullAxesBox();
  //customPlot->yAxis->setRange(-1.2, 1.2);
  customPlot->xAxis->setRange(0,1000);
  customPlot->yAxis->setRange(0, 1000);
  // make left and bottom axes transfer their ranges to right and top axes:
  connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

  // setup a timer that repeatedly calls Chart::realtimeDataSlot:
  connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot(/*int a2,int b2,int c2*/)));
  customPlot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom|QCP::iSelectPlottables);//can drag and zoom
    //customPlot->legend->setBrush(QColor(255,255,255,0));
  customPlot->graph(0)->setName("Encoder Rotation Rate"); //set the legend name
  customPlot->graph(1)->setName("Motor Rotation Rate");
  customPlot->graph(2)->setName("BigWheel Rotation Rate");

  //customPlot->graph(3)->removeFromLegend();
  customPlot->legend->setVisible(true); //display the legend with all graphs
  dataTimer.start(15); // Interval 0 means to refresh as fast as possible
}


void Chart::realtimeDataSlot()
{

  static QTime timeStart = QTime::currentTime();
  // calculate two new data points:
  //double key = timeStart.msecsTo(QTime::currentTime())/1000.0; // time elapsed since start of demo, in seconds
  double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
  static double lastPointKey = 0;
  //if (key-lastPointKey > 0.002) // at most add point every 2 ms
   if (key-lastPointKey > 0.2)
  {
    //connect(&mwindow,SIGNAL(sendDataChart(int a3,int b3,int c3)),this,SLOT(receiveData(int a1,int b1,int c1)));
    // add data to lines:
    //ui->customPlot->graph(0)->addData(key, qSin(key)+std::rand()/(double)RAND_MAX*1*qSin(key/0.3843));
    //ui->customPlot->graph(1)->addData(key, qCos(key)+std::rand()/(double)RAND_MAX*0.5*qSin(key/0.4364));
//       int eround=MainWindow::encoderround1;
//       int mround=MainWindow::motoround1;
//       int bround=MainWindow::biground1;
       ui->customPlot->graph(0)->addData(key, MainWindow::encoderround1);
       ui->customPlot->graph(1)->addData(key, MainWindow::motoround1);
       ui->customPlot->graph(2)->addData(key, MainWindow::biground1);
        //   customPlot->graph(3)->addData(key, rand()%400);
          // customPlot->graph(4)->addData(key, rand()%250);
         //  customPlot->graph(5)->addData(key, rand()%100);

    //ui->customPlot->graph(0)->rescaleValueAxis();
    //ui->customPlot->graph(1)->rescaleValueAxis(true);
    lastPointKey = key;
  }
  // make key axis range scroll with the data (at a constant range size of 8):
  //ui->customPlot->xAxis->setRange(key, 8, Qt::AlignRight);
   ui->customPlot->xAxis->setRange(key, 10, Qt::AlignRight);
   ui->customPlot->replot();

  // calculate frames per second:
  static double lastFpsKey;
  static int frameCount;
  ++frameCount;
  if (key-lastFpsKey > 2) // average fps over 2 seconds
  {
   // ui->statusBar->showMessage(
        //  QString("%1 FPS, Total Data points: %2")
        //  .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
        //  .arg(ui->customPlot->graph(0)->data()->size()+ui->customPlot->graph(1)->data()->size())
       //   , 0);
    lastFpsKey = key;
    frameCount = 0;
  }
}


Chart::~Chart()
{
  delete ui;
}

void Chart::slotBtnClear(){
    int count=ui->customPlot->graphCount();    //get the graph count in the realtime setup function.
      for(int i=0;i<count;++i)
      {
          ui->customPlot->graph(i)->data().data()->clear();
      }

}






































