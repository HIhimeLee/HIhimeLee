#include "settingdisp.h"
#include <connectionmanager.h>


int32_t DataArrayBuf[100];

SettingDisp::SettingDisp(QWidget *parent)
    :QWidget(parent)
{
    //        QLabel *label = new QLabel("Hello, Qt!", this);
    //       label->setAlignment(Qt::AlignCenter);

    QLabel *titleLabel = new QLabel("환경 설정", this);
    QFont titleFont;
    QVBoxLayout *vlayout = new QVBoxLayout;
    QHBoxLayout *hlayout = new QHBoxLayout;
    QHBoxLayout *hlayout1 = new QHBoxLayout;
    QHBoxLayout *hlayout2 = new QHBoxLayout;
    QHBoxLayout *hlayout3 = new QHBoxLayout();
    QHBoxLayout *btnlay = new QHBoxLayout();
    pPDMinfo = new  PDMInfo;
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);


    QLabel *ipLabel = new QLabel("DAQ IP:", this);
    ipLineEdit = new QLineEdit(this);
    ipLineEdit->setText("192.168.10.140");


    QLabel *portLabel = new QLabel("포트:", this);
    portLineEdit = new QLineEdit(this);
    portLineEdit->setText("7000");


    QLabel *ipLabel2 = new QLabel("MY IP:", this);
    ipLineEdit2 = new QLineEdit(this);
    ipLineEdit2->setText("192.168.10.7");

    QLabel *portLabel2 = new QLabel("포트:", this);
    portLineEdit2 = new QLineEdit(this);
    portLineEdit2->setText("7000");

    QLabel *deviceLabel = new QLabel("장치번호:", this);
    device = new QLineEdit(this);
    device->setPlaceholderText("1~255"); //전류출력은 무조건 5A이다.
    device->setText("1");
    QLabel *opLabel = new QLabel("캘리브레이션:", this);
    op = new QLineEdit(this);
    op->setText("0"); //전류를 캘리브레이션한다면?..



    QLabel *comboLabel = new QLabel("세팅값:", this);
    ratio = new QLineEdit;
    ratio->setPlaceholderText("정수:정수");
    ratio->setText("15:5");


    // 버튼
    QPushButton *btnTrans = new QPushButton("OK", this);
    btnTrans->setFixedSize(50, 30);
    btnlay->addStretch();
    btnlay->addWidget(btnTrans);
    btnlay->addStretch();

    hlayout->addWidget(ipLabel);
    hlayout->addWidget(ipLineEdit,3);
    hlayout->addWidget(portLabel);
    hlayout->addWidget(portLineEdit,1);

    hlayout1->addWidget(ipLabel2);
    hlayout1->addWidget(ipLineEdit2,3);
    hlayout1->addWidget(portLabel2);
    hlayout1->addWidget(portLineEdit2,1);


    hlayout2->addWidget(deviceLabel);
    hlayout2->addWidget(device);
    hlayout2->addWidget(opLabel);
    hlayout2->addWidget(op);
    hlayout3->addWidget(comboLabel,1);
    hlayout3->addWidget(ratio,1);
    vlayout->addWidget(titleLabel);
    vlayout->addLayout(hlayout);
    vlayout->addLayout(hlayout1);
    vlayout->addLayout(hlayout2);
    vlayout->addLayout(hlayout3);
    vlayout->addSpacing(20);
    vlayout->addLayout(btnlay);
//    vlayout->addWidget(btnTrans);

    // 수신 IP, 포트, 데이터 입력칸 및 버튼을 위젯에 설정
    setLayout(vlayout);
    resize(350,200);
    this->setTabOrder(op, ratio);
    this->setTabOrder(ratio, btnTrans);

//    connect(btnTrans,SIGNAL(clicked()),this,SLOT(btnTransFunc()));
}

#if 1
PDMInfo * SettingDisp::getInfo(void){
    return pPDMinfo;
}
void SettingDisp::btnTransFunc()
{
    qDebug()<<"-------------->"<<__func__;
    emit selectDeviceSignal(device->text().toInt());

    (*pPDMinfo).ip = ipLineEdit->text();
    (*pPDMinfo).port = portLineEdit->text().toInt();
 //   pdmInfo.deviceNum = device->text().toInt();  // Assuming deviceNum is an integer
    //pdmInfo = op->text().toFloat();  // Assuming OP is a float

    QString comboText = ratio->text();//->currentText();
    QStringList transValues = comboText.split(":");
//    if (transValues.size() == 2) {
//        pdmInfo.trans1 = transValues[0].toInt();
//        pdmInfo.trans2 = transValues[1].toInt();
//    }
   // sendData(pdmInfo);
    close();
}
#endif



