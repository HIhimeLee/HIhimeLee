#include "connectionmanager.h"
#include "settingdisp.h"
#include <QUdpSocket>


#include <QDateTime>

ConnectionManager::ConnectionManager(QObject* parent) :QObject(parent)
{
    qDebug()<<"-------------->"<<__func__;
    socket = new QUdpSocket;
    preTime1 = QDateTime::currentMSecsSinceEpoch();
    preTime2 = QDateTime::currentMSecsSinceEpoch();
    timeCounter = 0;
    qDebug()<<"-------------->"<<__func__ << " Timer :  " << preTime1;
    qDebug()<<"-------------->"<<__func__ << " Timer :  " << preTime2;
}
void ConnectionManager::sendSettingPacket_t(const PDMInfo& pdmInfo){
;
    ;

}
quint8 ConnectionManager::timeCountFunc(void){
    qDebug() << Q_FUNC_INFO <<" Timer Count : " << timeCounter;
    if (9 > timeCounter ){
        timeCounter++;
    } else {
        timeCounter = 0;
    }
    return timeCounter;
}

void ConnectionManager::sendSettingPacket1(QList<qint32>* bufflist) {
    qDebug()<<"-------------->"<<__func__;
    const int packetSize = 2026; // Size of the packet based on the provided structure
    quint32 time1;
    qint32 tempData;
    QByteArray packet;
    packet.resize(packetSize);
// 1.장치ID(2byte), 2.Phase(1byte), 3.패킷ID(1byte), 4.Trans1, 5.Trans2, 6.전압,전류, 7.주파수, 8.역률, 9.유효전력, 10.무효전력(여기까지2byte), 11.타임스템프(4byte)총 25byte
    // Fill the packet according to the provided structure
    char* buf = packet.data();
    buf[0] = STX;
    buf[1] = 0x00;
    buf[2] = 0x01;
    buf[3] = 0x00; // R =0x00, S = 0x01, T = 0x02
    buf[4] = 0x01; // 패킷 ID 1 첫 패킷
    buf[5] = static_cast<char>((0x000A >> 8) & 0xFF); // High byte of trans1
    buf[6] = static_cast<char>(0x000A & 0xFF);        // Low byte of trans1
    buf[7] = static_cast<char>((0x0005 >> 8) & 0xFF); // High byte of trans2
    buf[8] = static_cast<char>(0x0005 & 0xFF);        // Low byte of trans2
    buf[9] = 0;
    buf[10] = 220; //전압 220V
    buf[11] = 0;
    buf[12] = 100; //전류 100 x 10mA = 1A
    buf[13] = 0; //
    buf[14] = 60; // 주파수 60Hz
    buf[15] = 0; //
    buf[16] = 200; // 역률 100 x 0.1% = 20%
    buf[17] = 0; //
    buf[18] = 100; // 유효전력 100W
    buf[19] = 0; //
    buf[20] = 10; // 무효전력 10W
    preTime1 = QDateTime::currentMSecsSinceEpoch();
    time1 = (quint32) preTime1 ;
    buf[21] = (quint8 )(time1>>24) & 0xFF;
    buf[22] = (quint8 )(time1>>16) & 0xFF; //전압 220V
    buf[23] = (quint8 )(time1>>8) & 0xFF;
    buf[24] = (quint8 )time1 & 0xFF;
    int k= 25;
    for(int i = 0;i<bufflist[0].count();i++) {
//        qDebug() << bufflist[0].at(i);
        tempData = bufflist[0].at(i);
        for (int j = 0; j < 4; j++) {
            if (j == 0) {
                 buf[k+j] = static_cast<char>((tempData >> 24) & 0xFF);
            }
             else if ( j == 1){
                 buf[k+j] = static_cast<char>((tempData >> 16) & 0xFF);
             }
             else if (j == 2 ) {
                  buf[k+j] = static_cast<char>((tempData >> 8) & 0xFF);
             }
             else if( j==3) {
                  buf[k+j] = static_cast<char>(tempData & 0xFF);
             }

 //          if(i==2) qDebug() <<"buf = " << buf[k+j] <<"k = " << k << "j =  "<<j <<"  ::: " << tempData ;
        }
        k = k + 4;
    }
    qDebug() << "----test  " << k;
    buf[k] = ETX;

    QUdpSocket *socket= new QUdpSocket;
    socket->writeDatagram(packet, QHostAddress("172.22.7.144"), 7000); // Adjust as needed
    delete socket;
}
void ConnectionManager::sendSettingPacket2(QList<qint32>* bufflist) {
    qDebug()<<"-------------->"<<__func__;
    const int packetSize = 2010; // Size of the packet based on the provided structure
    quint32 time1;
    qint32 tempData;
    QByteArray packet;
    packet.resize(packetSize);
    // Fill the packet according to the provided structure
// 1.장치ID(2byte), 2.Phase(1byte), 3.패킷ID(1byte), 4.타임스템프(4byte)총 8byte
    char* buf = packet.data();
    buf[0] = STX;
    buf[1] = 0x00;
    buf[2] = 0x01;
    buf[3] = 0x00; // R =0x00, S = 0x01, T = 0x02
    buf[4] = 0x02; // 패킷 ID =2
    /*핵심*/
    preTime1 = QDateTime::currentMSecsSinceEpoch();
    time1 = (quint32) preTime1 ;
    buf[5] = (quint8 )(time1>>24) & 0xFF;
    buf[6] = (quint8 )(time1>>16) & 0xFF; //전압 220V
    buf[7] = (quint8 )(time1>>8) & 0xFF;
    buf[8] = (quint8 )time1 & 0xFF;
    int k= 9;
    for(int i = 0;i<bufflist[0].count();i++) {
//        qDebug() << bufflist[0].at(i);
        tempData = bufflist[0].at(i);
        for (int j = 0; j < 4; j++) {
            if (j == 0) {
                 buf[k+j] = static_cast<char>((tempData >> 24) & 0xFF);
            }
             else if ( j == 1){
                 buf[k+j] = static_cast<char>((tempData >> 16) & 0xFF);
             }
             else if (j == 2 ) {
                  buf[k+j] = static_cast<char>((tempData >> 8) & 0xFF);
             }
             else if( j==3) {
                  buf[k+j] = static_cast<char>(tempData & 0xFF);
             }

 //          if(i==2) qDebug() <<"buf = " << buf[k+j] <<"k = " << k << "j =  "<<j <<"  ::: " << tempData ;
        }
        k = k + 4;
    }
    qDebug() << "----test k:  " << k;
    buf[k] = ETX;
    QUdpSocket *socket= new QUdpSocket;
    socket->writeDatagram(packet, QHostAddress("172.22.7.144"), 7000); // Adjust as needed
   delete socket;
}
void ConnectionManager::sendSettingPacketAck(PDMInfo& pdmInfo) {
    qDebug()<<"-------------->"<<__func__;
    const int packetSize = 7; // Size of the packet based on the provided structure
    quint32 time1;
    qint32 tempData;
    QByteArray packet;
    packet.resize(packetSize);
    char* buf = packet.data();
    buf[0] = STX;
    buf[1] = 0x00;  // 장치ID 상위
    buf[2] = 0x01;  // 장치ID 하위
    buf[3] = 0x00; // R =0x00, S = 0x01, T = 0x02
    buf[4] = pdmInfo.packetID; // 패킷 ID =2
    buf[5] = pdmInfo.sts; // 패킷 ID =2
    /*핵심*/


    qDebug() << "----test k:  " ;
    buf[6] = ETX;
    QUdpSocket *socket= new QUdpSocket;
    socket->writeDatagram(packet, QHostAddress(pdmInfo.ip), pdmInfo.port); // Adjust as needed
    delete socket;
}
void ConnectionManager::sendSettingPacketAck12(PDMInfo& pdmInfo) {
    qDebug()<<"-------------->"<<__func__;
    const int packetSize = 9; // Size of the packet based on the provided structure
    quint32 time1;
    qint32 tempData;
    QByteArray packet;
    packet.resize(packetSize);
    char* buf = packet.data();
    buf[0] = STX;
    buf[1] = 0x00;  // 장치ID 상위
    buf[2] = 0x01;  // 장치ID 하위
    buf[3] = 0x00; // R =0x00, S = 0x01, T = 0x02
    buf[4] = pdmInfo.packetID; // 패킷 ID =2
    buf[5] = pdmInfo.sts; // 패킷 ID =2
    buf[6] = 0x00;  // 변경 장치ID 상위
    buf[7] = 0x10;  // 변경 장치ID 상위

    /*핵심*/


    qDebug() << "----test k:  " ;
    buf[8] = ETX;
    QUdpSocket *socket= new QUdpSocket;
    socket->writeDatagram(packet, QHostAddress(pdmInfo.ip), pdmInfo.port); // Adjust as needed

    delete socket;
}
void ConnectionManager::sendSettingPacketAck13(PDMInfo& pdmInfo) {
    qDebug()<<"-------------->"<<__func__;
    const int packetSize = 9; // Size of the packet based on the provided structure
    quint32 time1;
    qint32 tempData;
    QByteArray packet;
    packet.resize(packetSize);
    char* buf = packet.data();
    buf[0] = STX;
    buf[1] = 0x00;  // 장치ID 상위
    buf[2] = 0x01;  // 장치ID 하위
    buf[3] = 0x00; // R =0x00, S = 0x01, T = 0x02
    buf[4] = pdmInfo.packetID; // 패킷 ID =2
    buf[5] = pdmInfo.sts; // 패킷 ID =2
    buf[6] = 192;  // 변경 장치ID 상위
    buf[7] = 168;  // 변경 장치ID 상위
    buf[8] = 10;  // 변경 장치ID 상위
    buf[9] = 10;  // 변경 장치ID 상위

    /*핵심*/


    qDebug() << "----test k:  " ;
    buf[10] = ETX;
    QUdpSocket *socket= new QUdpSocket;
    socket->writeDatagram(packet, QHostAddress(pdmInfo.ip), pdmInfo.port); // Adjust as needed
    delete socket;
}
void ConnectionManager::start()
{
    qDebug()<<"-------------->"<<__func__;
    if(socket == nullptr){
        socket = new QUdpSocket(this);
    }

    if(!socket->bind(QHostAddress::Any, 7000)) {
        qDebug() << "binding error" << socket->errorString();
    } else {
        connect(socket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
        qDebug() << "binding good";
    }

}


void ConnectionManager::socketReadyRead()
{
    qDebug()<<"-------------->"<<__func__;
    QByteArray datagram;
    PDMInfo pdmInfo;
    datagram.resize(socket->pendingDatagramSize());
    socket->readDatagram(datagram.data(), datagram.size());
    QByteArray packet = datagram;
// qDebug() << "datagram: " <<datagram;
    char *buf = packet.data();
    int bufSize = packet.size();
    if (buf[0] != STX) return;
    if (buf[bufSize-1] != ETX) return;
    pdmInfo.ip = "172.22.7.144";
    pdmInfo.port = 7000;
    if(buf[4] == PACKET1){
        packetParsingBegin(buf,bufSize);
    }else if( PACKET8 > buf[4] ){
        packetParsingMid(buf, bufSize);
    }else{
        switch(buf[4]){
                    case CMDIPCHG  :
                        pdmInfo.packetID = STSDAQIPCHGACK;
                        pdmInfo.sts = STS_ACK_OK;
                        sendSettingPacketAck(pdmInfo);
                       break;
                    case CMDCURCAL:
                        pdmInfo.packetID = STSCURCALACK;
                        pdmInfo.sts = STS_ACK_OK;
                        sendSettingPacketAck(pdmInfo);
                       break;
                    case CMDVOLCAL  :
                        pdmInfo.packetID = STSVOLCALACK;
                        pdmInfo.sts = STS_ACK_OK;
                        sendSettingPacketAck(pdmInfo);
                       break;
                    case CMDDAQIDCHG :
                        pdmInfo.packetID =  STSDAQIDCHGACK;
                        pdmInfo.sts = STS_ACK_OK;
                        sendSettingPacketAck12(pdmInfo);
                       break;
                    case CMDDAQAPMODE  :
                        pdmInfo.packetID = STSDAQAPMODACK;
                        pdmInfo.sts = STS_ACK_OK;
                        sendSettingPacketAck13(pdmInfo);
                      break;
                    case CMDDAQCALMOD  :
                        pdmInfo.packetID = STSDAQCALMODACK;
                        pdmInfo.sts = STS_ACK_OK;
                        sendSettingPacketAck(pdmInfo);
                      break;
                    case CMDDAQCTCHG  :
                        pdmInfo.packetID = STSDAQCTCHGACK;
                        pdmInfo.sts = STS_ACK_OK;
                        sendSettingPacketAck(pdmInfo);
                      break;
                    default:

                        qDebug() <<"-------------->"<<__func__ << " : switch NG =  " << buf[3] ;
                        break;

                    }

        }


}


void ConnectionManager::packetParsingBegin(char *buf, int size)
{
        qDebug()<<"-------------->"<<__func__;

    if (buf[0] != STX | buf[size - 1] != ETX) return;
    QList<float> elecList;
    QList<qint16> curList;

    qint8 pDevice = buf[1];
    elecList.append(pDevice); // 장치번호 추가
    qint16 pPhase = buf[2];
    elecList.append(pPhase); // 위상을 전기특성리스트로 이동.

    uint16_t trans1 = (buf[4] << 8) | (buf[5] & 0xFF);
    uint16_t trans2 = (buf[6] << 8) | (buf[7] & 0xFF);
    qDebug()<<"trans ratio "<<trans1<<trans2;
    char* p = buf+8; //first address 8바이트 점프
    QStringList ele ={"volt[V]", "current[A]", "freq[Hz]", "PF", "AcP[W]", "ApP[VA]" };
    for (int i = 0; i < 6; ++i) { //출력용
        qint32 value = ((p[0]&0xFF) << 24) | ((p[1] & 0xFF) << 16) | ((p[2] & 0xFF) << 8) | (p[3] & 0xFF); //(p[0] << 24)에서 수정
        float f = *reinterpret_cast<float*>(&value);
        elecList.append(f);
        p += 4;
    }

    char* pp = buf + 32; //first address
    int curSize=(size-32-1) /2;
    for (int i = 0; i < curSize; ++i) {
        unsigned char byte0 = pp[0];
        unsigned char byte1 = pp[1];
        qint16 value0 = ((static_cast<quint16>(byte0) & 0xFF) << 8) | (byte1 & 0xFF);
        curList.append(value0);
        pp += 2;
    }
    qDebug()<<"-------------->"<<__func__ << " size : " << size << " cursize " << curSize;
    emit sig_dataOccured(&curList,&elecList);
}


void ConnectionManager::packetParsingMid(char *buf, int size)
{
        qDebug()<<"-------------->"<<__func__;
    if (buf[0] != STX | buf[size - 1] != ETX) return;
    QList<float> elecList;
    QList<qint16> curList;

    qint8 pDevice = buf[1];
    elecList.append(pDevice); // 장치번호 추가
    qint16 pPhase = buf[2];
    elecList.append(pPhase); // 위상을 전기특성리스트로 이동.
//    qDebug()<<"Mid phase"<<pPhase;

    char* p = buf + 4; //first address
    int curSize=(size-4-1) /2;
    for (int i = 0; i < curSize; ++i) {
        unsigned char byte0 = p[0];
        unsigned char byte1 = p[1];
        qint16 value0 = ((static_cast<quint16>(byte0) & 0xFF) << 8) | (byte1 & 0xFF);
        curList.append(value0);
        p += 2;
    }

    emit sig_dataOccured(&curList,&elecList); // null보내면 어떻게 될까
}


void ConnectionManager::stop()
{

    qDebug()<<"-------------->"<<__func__;
    if(socket){
        socket->abort();
        disconnect(socket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
    }

}



ConnectionManager::~ConnectionManager()
{
    qDebug()<<"-------------->"<<__func__;
    delete socket;
}
