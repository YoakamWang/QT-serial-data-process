/***************************************************************************
**                                                                        **
**  QCustomPlot, an easy to use, modern plotting widget for Qt            **
**  Copyright (C) 2011-2021 Emanuel Eichhammer                            **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU General Public License as published by  **
**  the Free Software Foundation, either version 3 of the License, or     **
**  (at your option) any later version.                                   **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  along with this program.  If not, see http://www.gnu.org/licenses/.   **
**                                                                        **
****************************************************************************
**           Author: Emanuel Eichhammer                                   **
**  Website/Contact: http://www.qcustomplot.com/                          **
**             Date: 29.03.21                                             **
**          Version: 2.1.0                                                **
****************************************************************************/

/************************************************************************************************************
**                                                                                                         **
**  This is the example code for QCustomPlot.                                                              **
**                                                                                                         **
**  It demonstrates basic and some advanced capabilities of the widget. The interesting code is inside     **
**  the "setup(...)Demo" functions of Chart.                                                          **
**                                                                                                         **
**  In order to see a demo in action, call the respective "setup(...)Demo" function inside the             **
**  Chart constructor. Alternatively you may call setupDemo(i) where i is the index of the demo       **
**  you want (for those, see Chart constructor comments). All other functions here are merely a       **
**  way to easily create screenshots of all demos for the website. I.e. a timer is set to successively     **
**  setup all the demos and make a screenshot of the window area and save it in the ./screenshots          **
**  directory.                                                                                             **
**                                                                                                         **
*************************************************************************************************************/

#include "Chart.h"
#include "ui_chart.h"
#include <QDebug>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#  include <QDesktopWidget>
#endif
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>

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
  connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
  customPlot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom|QCP::iSelectPlottables);//can drag and zoom

  dataTimer.start(10); // Interval 0 means to refresh as fast as possible
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
    // add data to lines:
    //ui->customPlot->graph(0)->addData(key, qSin(key)+std::rand()/(double)RAND_MAX*1*qSin(key/0.3843));
    //ui->customPlot->graph(1)->addData(key, qCos(key)+std::rand()/(double)RAND_MAX*0.5*qSin(key/0.4364));
       ui->customPlot->graph(0)->addData(key, rand()%700);
       ui->customPlot->graph(1)->addData(key, rand()%700);
       ui-> customPlot->graph(2)->addData(key, rand()%500);
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






































