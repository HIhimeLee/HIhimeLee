
//추후 로딩과 수신시 그리는 공통부분 추출하여 함수화
// TODO: 실제모터 구동시 확인 필요
// 회전수(분당) = 120*60Hz/극수
// 회전수(초당) = 120*60Hz/극수/60 :

#include <QtConcurrent/QtConcurrent>

#include "mainwindow.h"
#include "ui_mainwindow.h"
// #include "fileManager.h"
#include "qdebug.h"
#include <cmath>
// #include <include/fftw3.h>
// #include <loadinglist.h>
#include <QtNetwork/QNetworkInterface>
#include <QNetworkConfigurationManager>
#include <QNetworkConfiguration>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QTimer *Timer = new QTimer(this);
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(on_pushButton_clicked()));
    connect(Timer, SIGNAL(timeout()), this, SLOT(ms_timer_out()));

    Timer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    qDebug() << Q_FUNC_INFO <<"Hello World";
}
void MainWindow::ms_timer_out()
{
    qDebug() << Q_FUNC_INFO <<"Hello Timer Out World";
}

