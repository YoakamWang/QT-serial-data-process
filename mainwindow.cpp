#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDateTime>
#include <QDebug>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    m_settings(new SettingsDialog),
    m_status(new QLabel),
    m_serial(new QSerialPort(this))
{
    ui->setupUi(this);
    setWindowTitle("New Solution Verification");
    m_file_save.setFileName("wavedata_save.csv");
    if(!m_file_save.open(QIODevice::WriteOnly|QIODevice::Text)){
        QMessageBox::critical(this,tr("Error"),"Failed to open the file.");
        return;
    }
    ui->disconnectButton->setEnabled(false);
    Chart* chart1 = new Chart();
    ui->chartView->addWidget(chart1);
    ui->statusBar->addWidget(m_status);
    connect(ui->configButton, &QPushButton::clicked, m_settings, &SettingsDialog::show);
    connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::openSerialPort);
    connect(ui->disconnectButton, &QPushButton::clicked, this, &MainWindow::closeSerialPort);
    connect(m_serial, &QSerialPort::readyRead, this, &MainWindow::readData);
}

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
//        m_console->setEnabled(true);
//        m_console->setLocalEchoEnabled(p.localEchoEnabled);
        ui->configButton->setEnabled(false);
        ui->disconnectButton->setEnabled(true);
        //ui->actionConfigure->setEnabled(false);
        showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                         .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                         .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
    } else {
        QMessageBox::critical(this, tr("Error"), m_serial->errorString());

        //showStatusMessage(tr("Open error"));
    }
}
void MainWindow::closeSerialPort()
{
    if (m_serial->isOpen())
        m_serial->close();
    ui->connectButton->setEnabled(true);
    ui->disconnectButton->setEnabled(false);
    ui->configButton->setEnabled(true);
    showStatusMessage(tr("Disconnected"));
}
void MainWindow::readData()
{
    const QByteArray data = m_serial->readAll();
    //process the serial data here
    //process the serial data here
    //const char *mm=data.data();
    //QString str1=mm;
    //int heartnum=str1.mid(4,6).toInt();
    //QFile file("saveData.csv");
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz");
    QTextStream out(&m_file_save);
    QThread::msleep(15);                                       //delay 15ms
    out<<current_date<<","<<1<<","<<2<<","<<3<<"\n";
    qDebug()<<data;
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

