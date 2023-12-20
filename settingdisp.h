#ifndef SETTINGDISP_H
#define SETTINGDISP_H

#include <QWidget>
#include <QLabel>
#include <QDebug>
#include<QLineEdit>
#include<QPushButton>
#include<QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QUdpSocket>
#include <QHostAddress>

#define STX 2//65//0x02
#define ETX 0x03

#define CMDIPREQ     0x80
#define CMDIPEND     0x81
#define CMDIPCHG     0x82
#define CMDCURCAL    0x83
#define CMDVOLCAL    0x84
#define CMDDAQIDCHG  0x85
#define CMDDAQAPMODE 0x86
#define CMDDAQCALMOD 0x87
#define CMDDAQCTCHG 0x88

#define PACKET1 0x01
#define PACKET2 0x02
#define PACKET3 0x03
#define PACKET4 0x04
#define PACKET5 0x05
#define PACKET6 0x06
#define PACKET7 0x07
#define PACKET8 0x08
#define STSCURCALACK     0x10
#define STSVOLCALACK     0x11
#define STSDAQIDCHGACK    0x12
#define STSDAQAPMODACK    0x13
#define STSDAQCALMODACK    0x14
#define STSDAQIPCHGACK    0x15
#define STSDAQCTCHGACK    0x16

#define STS_ACK_OK 0x00
#define STS_ACK_NG1 0x01
#define STS_ACK_NG2 0x02

class ConnectionManager;
struct PDMInfo{
// struct DAQData_Info1 {
   QString ip; //Destination
   int port; //Destination
   uint8_t firstChar;
   uint16_t deviceNum; //전송시 hex변환
   uint8_t phase;
   uint8_t packetID;  // CMD, STS ID
   uint8_t cmd;
   uint8_t sts;  // CMD, STS ID
   uint16_t trans1;
   uint16_t trans2;
   uint16_t volt;
   uint16_t current;
   uint16_t hz;
   uint16_t powerFactor;
   uint16_t activePower;
   uint16_t reactivePower;
   int32_t *Data1Arry;
};
struct DAQData2_Info1 {
   QString ip; //Destination
   int port; //Destination
   uint8_t firstChar;
   uint16_t deviceNum; //전송시 hex변환
   uint8_t phase;
   uint8_t packetID;
   int32_t *Data1Arry;
};
class SettingDisp: public QWidget
{
        Q_OBJECT

public:
   // SettingDisp();
   SettingDisp(QWidget *parent = nullptr);
   PDMInfo * getInfo(void);

   QLineEdit *ipLineEdit ;
   QLineEdit *portLineEdit;
   QLineEdit *ipLineEdit2 ;
   QLineEdit *portLineEdit2;
   QLineEdit *device;
   QLineEdit *op;
   QLineEdit *ratio;
   PDMInfo *pPDMinfo;

signals:
   void selectDeviceSignal(int num);

private slots:
   void btnTransFunc();
//private:



};
// extern PDMInfo pdmInfo ;
#endif // SETTINGDISP_H

