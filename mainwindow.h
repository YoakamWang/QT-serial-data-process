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
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private slots:
    void openSerialPort();
    void closeSerialPort();
    void readData();

private:
    void showStatusMessage(const QString &message);
private:
    Ui::MainWindow *ui;
    SettingsDialog *m_settings = nullptr;
    QSerialPort *m_serial = nullptr;
    QLabel *m_status = nullptr;
    QFile m_file_save;
};
#endif // MAINWINDOW_H
