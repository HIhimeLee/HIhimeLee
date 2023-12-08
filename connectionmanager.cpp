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
}


void ConnectionManager::sendSettingPacket(const PDMInfo& pdmInfo) {
    qDebug()<<"-------------->"<<__func__;
    const int packetSize = 8; // Size of the packet based on the provided structure
    QByteArray packet;
    packet.resize(packetSize);

    // Fill the packet according to the provided structure
    char* buf = packet.data();
    buf[0] = STX;
    buf[1] = pdmInfo.deviceNum;
    buf[2] = pdmInfo.packetID;
    /*핵심*/
    buf[3] = static_cast<char>((pdmInfo.trans1 >> 8) & 0xFF); // High byte of trans1
    buf[4] = static_cast<char>(pdmInfo.trans1 & 0xFF);        // Low byte of trans1
    buf[5] = static_cast<char>((pdmInfo.trans2 >> 8) & 0xFF); // High byte of trans2
    buf[6] = static_cast<char>(pdmInfo.trans2 & 0xFF);        // Low byte of trans2
    buf[7] = ETX;

    uint16_t trans1 = (buf[3] << 8) | (buf[4] & 0xFF);
    uint16_t trans2 = (buf[5] << 8) | (buf[6] & 0xFF);
    qDebug()<<"trans1:trans2"<<trans1<<trans2; // 15:5로 잘 출력됨.


    QUdpSocket *socket= new QUdpSocket;
    socket->writeDatagram(packet, QHostAddress(pdmInfo.ip), pdmInfo.port); // Adjust as needed
    // socket->writeDatagram(reinterpret_cast<const char*>(&pdmInfo), sizeof(PDMInfo), QHostAddress(pdmInfo.ip), pdmInfo.port);
    //qDebug() << "Sent PDM data to " << pdmInfo.ip << ":" << pdmInfo.port;
    //qDebug() << "device,OP,tran1:2 " <<pdmInfo.deviceNum<< pdmInfo.OP<<pdmInfo.trans1<<pdmInfo.trans2;
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
    datagram.resize(socket->pendingDatagramSize());
    socket->readDatagram(datagram.data(), datagram.size());
    QByteArray packet = datagram;
// qDebug() << "datagram: " <<datagram;
    char *buf = packet.data();
    int bufSize = packet.size();
    if (buf[0] != STX) return;
    if (buf[bufSize-1] != ETX) return;

//            QString strPacket;
//            for (int i=0; i<packet.length(); i++) {
//                QString s;
//                s.sprintf("%02X", (uchar)packet.at(i));
//                strPacket += s + " ";
//            }
//            qDebug() << "packet:" << strPacket;

    if(buf[3] ==STSDATA1){
        packetParsingBegin(buf,bufSize);
    }else{
        packetParsingMid(buf, bufSize);
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
