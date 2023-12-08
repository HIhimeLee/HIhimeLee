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

#define STX 0x02
#define ETX 0x03

#define CMDIPREQ     0x80
#define CMDIPEND     0x81
#define CMDIPCHG     0x82
#define CMDCURCAL    0x83
#define CMDVOLCAL    0x84
#define CMDDAQIDCHG  0x85

#define STSDATA1 0x01
#define STSDATA2 0x02
#define STSDATA3 0x03
#define STSDATA4 0x04
#define STSDATA5 0x05
#define STSDATA6 0x06
#define STSDATA7 0x07
#define STSDATA8 0x08
#define STSCURCALACK     0x10
#define STSVOLCALACK     0x11
#define STSDAQICHGACK    0x12

class ConnectionManager;
 struct PDMInfo{
// struct DAQData_Info1 {
   QString ip; //Destination
   int port; //Destination
   uint8_t firstChar;
   uint16_t deviceNum; //전송시 hex변환
   uint8_t phase;
   uint8_t packetID;
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
signals:
   void selectDeviceSignal(int num);

private slots:
   void btnTransFunc();
private:

   QLineEdit *ipLineEdit ;
   QLineEdit *portLineEdit;
   QLineEdit *ipLineEdit2 ;
   QLineEdit *portLineEdit2;
   QLineEdit *device;
   QLineEdit *op;
   QLineEdit *ratio;



};
#endif // SETTINGDISP_H
