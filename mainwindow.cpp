
//추후 로딩과 수신시 그리는 공통부분 추출하여 함수화
// TODO: 실제모터 구동시 확인 필요
// 회전수(분당) = 120*60Hz/극수
// 회전수(초당) = 120*60Hz/극수/60 :

#include <QtConcurrent/QtConcurrent>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fileManager.h"
#include "qdebug.h"
#include <cmath>
// #include <include/fftw3.h>
// #include <loadinglist.h>
#include <QtNetwork/QNetworkInterface>
#include <QNetworkConfigurationManager>
#include <QNetworkConfiguration>

// ConnectionManager _conMng; //추후 포인터로 바꿔버리자!!!
#define LIMIT_FREQ 120 //이전에는 120
#define REFRESH_TIME  100//200    pop주기 ms
#define DATA_PUSH_TIME 120//* Test data push 주기 ms*/
#define LIMIT_SIZE 20000//수신차단 사이즈
#define TOTAL_DATA 4000// 8*500

#define DATA_PUSH_SIZE 500 //push
#define DATA_POP_SIZE  500 //
#define PACKET_CNT 8
#define MAX_CUR_LIST TOTAL_DATA+DATA_POP_SIZE
#define MAX_CUR_LIST2 TOTAL_DATA
#define HALF TOTAL_DATA/2+1
#define THRESHOLD 150 //const int THRESHOLD =100;
#define INDEX_CUR 0
#define INDEX_FFT 1
#define INDEX_WT 2

const double SAMPLING_RATE =2142;//7725: 56Hz도근방; 1833: 51Hz fs바꾸면 피크가 전혀 이상한 값에 찍힌다.
const int FFT_ITEM_SIZE = LIMIT_FREQ/(SAMPLING_RATE/TOTAL_DATA); //180/0.535
const int FFT_LOAD_UNIT =FFT_ITEM_SIZE*2+1; //449(FREQ_ITEM*2열+위상)
int CHART_POP_SIZE;
bool _generalMode=0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qDebug()<<"-------------->"<<__func__;

 Timer2 = new QTimer(this);

    ui->setupUi(this);
    initMainLayout();
    initMainToDAQ();
    initDaq();
    signalGen();
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_2->setText("STOP");
    connect(ui->pushButton_1, SIGNAL(clicked()), this, SLOT(on_pushButton_clicked()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(on_stopButton_clicked()));
    connect(Timer2, SIGNAL(timeout()), this, SLOT(ms_timer_out()));
//    connect(ui->pushButton_1, SIGNAL(clicked()), this, SLOT(sendSettingPacket1()));
//    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(sendSettingPacket2()));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    qDebug() << Q_FUNC_INFO <<"Hello World pushButton";
     _conMng.start();
    _conMng.sendSettingPacket1(_SignalTestData[0]);
    on_pushButton_clicked2();
}
void MainWindow::on_pushButton_clicked2()
{
    qDebug() << Q_FUNC_INFO <<"Hello World pushButton22";

    Timer = new QTimer(this);
//     _conMng.start(); on_stopButton_clicked
    _conMng.sendSettingPacket2(_SignalTestData[0]);
    connect(Timer, SIGNAL(timeout()), this, SLOT(ms_timer_out()));
    Timer->start(100);
}
void MainWindow::on_stopButton_clicked()
{
    qDebug() << Q_FUNC_INFO <<"Timer Stop";

    Timer->stop()  ;
}
void MainWindow::initMainLayout()
{
   // _conMng =  new ConnectionManager();
   // ConnectionManager * p = this->_conMng;
    qDebug() << Q_FUNC_INFO <<"Hello World Layout \n";
//    ui->pushButton_1->clicked();
//    connect(ui->pushButton_1, SIGNAL(clicked()), &this->_conMng, SLOT(sendSettingPacket1()));
//    connect(ui->pushButton_2, SIGNAL(clicked()), &this->_conMng, SLOT(sendSettingPacket2()));

}

void MainWindow::initMainToDAQ()
{
    qDebug() << Q_FUNC_INFO <<"Hello World Main To DAQ \n";
}
void MainWindow::initDaq()
{
    qDebug()<<"-------------->"<<__func__;
//    _fileManager = new FileManager;
    qint32 time1,time2 ;
    m_preTime1 = QDateTime::currentMSecsSinceEpoch();
    m_preTime2 = QDateTime::currentMSecsSinceEpoch();
    time1 = (qint32)(m_preTime1 & 0xFFFFFFFF);
    time2 = (qint32)(m_preTime2 & 0xFFFFFFFF);
    qDebug()<<"-------------->"<<__func__ << " Timer :  " << m_preTime1;
    qDebug()<<"-------------->"<<__func__ << " Timer :  " << time2;
    //_conMng이거는 인스턴스 생성안해도 되나..
//    connect(&_conMng, &ConnectionManager::sig_dataOccured, this, &MainWindow::slot_daq_dataOccured);
}
void MainWindow::signalGen()
{
    qDebug()<<"-------------->"<<__func__;
    /*signalTest*/
    const double TARGET_FREQUENCY = 60.0;
    const double AMPLITUDE = 500.0;
    const double PHASE_DELAY = 30.0 * M_PI / 180.0;
    for(int k = 0 ;k<3;k++) {
        _SignalTestData[k] = new QList<qint32>;
        for (int i = 0; i < DATA_PUSH_SIZE; ++i) {
            double time = i / static_cast<double>(SAMPLING_RATE);
//            double value = 2*time; 확인용
            double value = std::sin(2 * M_PI * TARGET_FREQUENCY * time + k * PHASE_DELAY);
            _SignalTestData[k]->append(qint32(value * AMPLITUDE));
        }
    }
    qDebug()<<"-------------->"<<__func__ <<"Data  : " << _SignalTestData[0]->count();

//    for(int i = 0;i<3;i++) {
//        for(int j = 0;j<_SignalTestData[i]->count();j++) {
//            qDebug() << _SignalTestData[i]->at(j);
//        }
//    }
}

void MainWindow::sendsignal(QList<qint32>* buff)
{
    _conMng.sendSettingPacket1(_SignalTestData[0]);
}

void MainWindow::ms_timer_out()
{
    qDebug() << Q_FUNC_INFO <<"Hello Timer1";
    quint8 i,k = 0;
    i = _conMng.timeCountFunc();
    if (9 > i){
          k = 1;
        _conMng.sendSettingPacket2(_SignalTestData[0]);
    }else {
          k =2;
      Timer->stop()  ;
      on_pushButton_clicked();
    }
    qDebug() << Q_FUNC_INFO <<"Timer i : " << i << "k = :"<<k;
}
void MainWindow::ms_timer_out2()
{
    qDebug() << Q_FUNC_INFO <<"Hello Timer2";
    quint8 i,k = 0;
    i = _conMng.timeCountFunc();
    if (9 > i){
          k = 1;
        _conMng.sendSettingPacket2(_SignalTestData[0]);
    }else {
          k =2;
      Timer->stop()  ;
    }
    qDebug() << Q_FUNC_INFO <<"Timer i : " << i << "k = :"<<k;
}



