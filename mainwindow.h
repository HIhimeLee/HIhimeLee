#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
// #include <QtCharts>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include "connectionmanager.h"
// #include <loadinglist.h>
#include <settingdisp.h>
#include <windows.h>
#include <wlanapi.h>
#include <objbase.h>
#include <wtypes.h>
#include <cwchar>
#include <cstdio>

namespace Ui {
class MainWindow;
}
class FileManager;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
// signals:
//    void dataReady(QList<float>*);
//    void sig_dataOccured(QList<qint16>*,QList<float>*);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_clicked2();
    void ms_timer_out();
    void ms_timer_out2();
    void on_stopButton_clicked();
    //void on_pushButton_1_clicked();

private:
    void initMainLayout();
    void initMainToDAQ();
    void initDaq();
    void signalGen();
    void sendsignal(QList<qint32>*);
public :
    QTimer *Timer;
    QTimer *Timer2;
private:
    Ui::MainWindow *ui;
    /* initDaq() */
    ConnectionManager _conMng; //추후 포인터로 바꿔버리자!!!
    FileManager* _fileManager;
    /* signalGen() */
    QList<qint32>* _SignalTestData[3]; //current data
    qint64 m_preTime1;
    qint64 m_preTime2;

};

#endif // MAINWINDOW_H
