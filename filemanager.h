#ifndef FILEMANAGE_H
#define FILEMANAGE_H
#include <QWidget>

class QTreeWidget;
class FileManager : public QWidget
{
    Q_OBJECT
signals:
    void updateLabel(QString);
public:
//    QList<float>* loadFile();
    QList<quint16>* loadFile();
    QList<double>* loadFileFFT();
   // void saveAcqFile(QList<QList<float>>);
//    void saveCsvFile(QList<float>* ,QList<qint16> *,QString deviceNum);//cur
    void saveCsvFile(QList<quint16>* ,QList<qint32> *,QString deviceNum);//cur
    void saveCsvFile(QList<double> *,QString deviceNum); //fft
    void saveImgFile(QString,QString);
   // void changeFilePermissions(QString);
//    QList<float>* loadFileBy(const QString fileName);
    QList<quint16>* loadFileBy(const QString fileName);
    QList<double>* loadFileFFTBy(const QString fileName);


public:
    FileManager(QWidget * parent =nullptr);
//    QList<float>* dataList;
//    QList<float> floatList; //불러오기용
    QList<quint16>* dataList;
    QList<quint16> floatList; //불러오기용
    QList<double> doubleList; //불러오기용
};
#endif // FILEMANAGE_H
