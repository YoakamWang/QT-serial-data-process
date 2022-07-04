#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include "Chart.h"
#include "settingsdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static int encoderround1; //Transfer the serial data from mainwindow to chart.
    static float motoround1;
    static int biground1;
//    struct SerialData{
//        int enround;
//        int motoround;
//        int biground;
//    };
//    SerialData serialdata() const;
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private slots:
    void openSerialPort();
    void closeSerialPort();
    void readData();
    void writeData();

//signals:
//    void sendDataChart(int encoderround,int motoround,int biground);



private:
    void showStatusMessage(const QString &message);
private:
    Ui::MainWindow *ui;
    SettingsDialog *m_settings = nullptr;
    QSerialPort *m_serial = nullptr;
    QLabel *m_status = nullptr;
    QFile m_file_save;
    QTimer wriDataTimer;
    //SerialData m_serialData;

};
#endif // MAINWINDOW_H
