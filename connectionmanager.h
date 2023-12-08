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
    void sendSettingPacket(const PDMInfo& pdmInfo);
signals:
    void dataReady(QList<float>*);
    void sig_dataOccured(QList<qint16>*,QList<float>*);

//private slots:
    void socketReadyRead();
    void packetParsingBegin(char *buf, int size);
    void packetParsingMid(char *buf, int size);
public:
    void start();
    void stop();

private:
    QUdpSocket *socket;
    QList<float> *freqList=nullptr;
    QList<qint16> *curList=nullptr;
    qint64 preTime1;
    qint64 preTime2;



};

#endif // CONNECTIONMANAGER_H
