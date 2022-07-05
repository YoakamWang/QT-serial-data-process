#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDateTime>
#include <QDebug>
#include <QThread>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    m_settings(new SettingsDialog),
    m_serial(new QSerialPort(this)),
    m_status(new QLabel)
{
    ui->setupUi(this);
    setWindowTitle("New Solution Verification");
    m_file_save.setFileName("wavedata_save.csv");
    if(!m_file_save.open(QIODevice::WriteOnly|QIODevice::Text)){
        QMessageBox::critical(this,tr("Error"),"Failed to open the file.");
        return;
    }
    ui->disconnectButton->setEnabled(false);
    ui->writeButton->setEnabled(false);
    Chart* chart1 = new Chart();
    ui->chartView->addWidget(chart1);
    ui->statusBar->addWidget(m_status);
    connect(ui->configButton, &QPushButton::clicked, m_settings, &SettingsDialog::show);
    connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::openSerialPort);
    connect(ui->disconnectButton, &QPushButton::clicked, this, &MainWindow::closeSerialPort);
    connect(ui->writeButton,&QPushButton::clicked,this,&MainWindow::writeData);
    connect(m_serial, &QSerialPort::readyRead, this, &MainWindow::readData);
    //connect(this,SIGNAL(signal_data(QString)),class::getInstance(),SLOT(slot_data(QString)));
}
//MainWindow::SerialData MainWindow::serialdata() const{
//    return m_serialData;
//}

void MainWindow::openSerialPort()
{
    const SettingsDialog::Settings p = m_settings->settings();
    m_serial->setPortName(p.name);
    m_serial->setBaudRate(p.baudRate);
    m_serial->setDataBits(p.dataBits);
    m_serial->setParity(p.parity);
    m_serial->setStopBits(p.stopBits);
    m_serial->setFlowControl(p.flowControl);
    if (m_serial->open(QIODevice::ReadWrite)) {
        //qDebug()<<1;
//        m_console->setEnabled(true);
//        m_console->setLocalEchoEnabled(p.localEchoEnabled);
        ui->configButton->setEnabled(false);
        ui->disconnectButton->setEnabled(true);
        ui->writeButton->setEnabled(true);
        //ui->actionConfigure->setEnabled(false);
        showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                         .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                         .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
/*       connect(&wriDataTimer, SIGNAL(timeout()), this, SLOT(writeData()));                    //Timer for loop function
       wriDataTimer.start(400); */                                                              //Set the interal as 100 ms.
    } else {
        QMessageBox::critical(this, tr("Error"), m_serial->errorString());

        showStatusMessage(tr("Serial Open error"));
    }
}
void MainWindow::closeSerialPort()
{
    if (m_serial->isOpen())
        m_serial->close();
    ui->connectButton->setEnabled(true);
    ui->disconnectButton->setEnabled(false);
    ui->configButton->setEnabled(true);
    ui->writeButton->setEnabled(false);
//    m_serialData.enround=0;
//    m_serialData.motoround=0;
//    m_serialData.biground=0;
    encoderround1=0;
    motoround1=0;
    biground1=0;
    showStatusMessage(tr("Disconnected"));
}

void MainWindow::showStatusMessage(const QString &message)
{
    m_status->setText(message);
}
MainWindow::~MainWindow()
{
    m_file_save.close();
    delete ui;
}
void MainWindow::writeData()
{
    const QByteArray str1="#0140";
    //qDebug()<<1;
      m_serial->write(str1);
 }
void MainWindow::readData()
{
 //    if(m_serial->waitForReadyRead()){
      const QByteArray str1="#0164";
    //qDebug()<<1;
      m_serial->write(str1);
        const QByteArray data = m_serial->readAll();
        const char *mm=data.data();
        QString sss=mm;
        encoderround1=sss.mid(1).toInt();
       //qDebug()<<encoderround1;
    //QThread::msleep(50);


     if(m_serial->waitForReadyRead()){
         const QByteArray str2="#0141";
          m_serial->write(str2);
         const QByteArray data1 = m_serial->readLine();
         const char *mm1=data1.data();
         QString sss1=mm1;
         double xishu=4.003;
         motoround1=sss1.mid(1).toDouble()*xishu;
        // qDebug()<<motoround1;
 }
    //QThread::msleep(50);
     if(m_serial->waitForReadyRead()){
                QByteArray str3="#0140";
                m_serial->write(str3);
            const QByteArray data2= m_serial->readLine();

           const char *mm2=data2.data();
             QString sss2=mm2;
            biground1=sss2.mid(1).toInt();
            // qDebug()<<biground1;
    }
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz");
    QTextStream out(&m_file_save);
     // qDebug()<<encoderround1<<","<<motoround1<<","<<biground1;
    out<<current_date<<","<<encoderround1<<","<<motoround1<<","<<biground1<<"\n";
    QThread::msleep(150);
}

//void MainWindow::writeData()
//{
//    const QByteArray str1="#0140";
//    //qDebug()<<1;
//      m_serial->write(str1);
//      if(m_serial->waitForBytesWritten()){
//       const QByteArray data = m_serial->readAll();
//          const char *mm=data.data();
//          QString sss=mm;
//          encoderround1=sss.mid(2).toFloat();
//         qDebug()<<encoderround1;}


//         const QByteArray str2="#0164";
//         m_serial->write(str2);
//         if(m_serial->waitForBytesWritten()){
//              if(m_serial->waitForReadyRead()){
//        const QByteArray data1 = m_serial->readAll();
//              const char *mm1=data1.data();
//                QString sss1=mm1;
//               motoround1=sss1.mid(2).toFloat();
//             qDebug()<<motoround1;}
// }


//                QByteArray str3="#0141";
//                m_serial->write(str3);
//                if(m_serial->waitForBytesWritten()){
//                    if(m_serial->waitForReadyRead()){
//            const QByteArray data2= m_serial->readAll();

//           const char *mm2=data2.data();
//             QString sss2=mm2;
//            biground1=sss2.mid(2).toInt();
//             qDebug()<<biground1;}
//    }

//}
//void MainWindow::readData(){

//}





//void MainWindow::readData()
//{
//    const QByteArray data = m_serial->readAll();
//    //process the serial data here
//    //const char *mm=data.data();
//    //QString str1=mm;
//    //int heartnum=str1.mid(4,6).toInt();
//    //QFile file("saveData.csv");
//    QDateTime current_date_time =QDateTime::currentDateTime();
//    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz");
//    QTextStream out(&m_file_save);
//        //  QThread::msleep(100);     //delay 15ms
//        //    m_serialData.enround=200;
//        //    m_serialData.motoround=800;
//        //    m_serialData.biground=300;
//    QString sss;
//    for (int i=2;i<data.size()-1;i++){
//          sss.append(data[i]);
//    }
//    int enround=sss.toInt();
//   // encoderround1=enround;
//    //motoround1=800;
//    //biground1=300;
//    //emit sendDataChart(encoderround1,biground1,motoround1);
//    out<<current_date<<","<<encoderround1<<","<<motoround1<<","<<biground1<<"\n";
//    qDebug()<<encoderround1;
//    //qDebug()<<data[3];
//    //qDebug()<<data[4];
//}

