#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QObject>
#include <QUdpSocket>

struct PDMInfo;
class ConnectionManager :public QObject
{
    Q_OBJECT
public:
    ConnectionManager(QObject *parent = nullptr);
    ~ConnectionManager();
    void sendSettingPacket_t(const PDMInfo& pdmInfo);
    quint8 timeCountFunc(void);
signals:
    void dataReady(QList<float>*);
    void sig_dataOccured(QList<qint16>*,QList<float>*);

public slots:
    void socketReadyRead();
    void packetParsingBegin(char *buf, int size);
    void packetParsingMid(char *buf, int size);
    void sendSettingPacket1(QList<qint32>* bufflist);
    void sendSettingPacket2(QList<qint32>* bufflist);
    void sendSettingPacketAck(PDMInfo& pdmInfo);
    void sendSettingPacketAck12(PDMInfo& pdmInfo);
    void sendSettingPacketAck13(PDMInfo& pdmInfo);
public:
    void start();
    void stop();

private:
    quint8 timeCounter;
    QUdpSocket *socket;
    QList<float> *freqList=nullptr;
    QList<qint16> *curList=nullptr;
    qint64 preTime1;
    qint64 preTime2;



};

#endif // CONNECTIONMANAGER_H
