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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void ms_timer_out();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
