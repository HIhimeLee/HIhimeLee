#include "filemanager.h"
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTreeWidget>

#include <QDateTime>
#include <QStandardPaths>
#include <QFile>
#include <QFileInfo>

FileManager:: FileManager(QWidget * parent)
    :QWidget(parent)
{
    qDebug()<<"-------------->"<<__func__;

}

//QList<float>* FileManager::loadFile() //로딩버튼으로 로딩시
QList<quint16>* FileManager::loadFile() //로딩버튼으로 로딩시
{

    qDebug() << "-------------->" << __func__;
    floatList.clear();
    QString desktopPath = QDir::homePath()+"/Desktop";
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), desktopPath+"/cvsFolder", tr("CSV files (*.csv);;All Files (*)"));
    if (fileName.isEmpty()) {
        return nullptr;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file: " << fileName;
        return nullptr;
    }
    emit updateLabel(fileName);

    while (!file.atEnd()) {
        QString line = file.readLine();
        QStringList fields = line.split(",");
        foreach(QString field, fields) {
            floatList.append(field.toFloat());
        }
    }
    qDebug()<<"loaded"<<floatList.size();
    return &floatList;
}


//QList<float>* FileManager::loadFileBy(const QString fileName) //로딩위젯에서 로딩시
QList<quint16>* FileManager::loadFileBy(const QString fileName) //로딩위젯에서 로딩시
{

    floatList.clear();
    QFile file(fileName);
    emit updateLabel(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file: " << fileName;
        return nullptr;
    }

    while (!file.atEnd()) {
        QString line = file.readLine();
        QStringList fields = line.split(",");
        foreach(QString field, fields) {
            floatList.append(field.toFloat());
        }
    }
    qDebug()<<"loaded"<<floatList.size();
    return &floatList;
}


QList<double>* FileManager::loadFileFFT() //로딩버튼으로 로딩시
{

    qDebug() << "-------------->" << __func__;
    doubleList.clear();
    QString desktopPath = QDir::homePath()+"/Desktop";
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), desktopPath+"/cvsFolder", tr("CSV files (*.csv);;All Files (*)"));
    if (fileName.isEmpty()) {
        return nullptr;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file: " << fileName;
        return nullptr;
    }
    emit updateLabel(fileName);

    while (!file.atEnd()) {
        QString line = file.readLine();
        QStringList fields = line.split(",");
        foreach(QString field, fields) {
            doubleList.append(field.toDouble());
        }
    }
    qDebug()<<"loaded"<<doubleList.size();
    return &doubleList;
}


QList<double>* FileManager::loadFileFFTBy(const QString fileName)//로딩위젯에서 로딩시
{
    doubleList.clear();
    QFile file(fileName);
    emit updateLabel(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file: " << fileName;
        return nullptr;
    }
    while (!file.atEnd()) {
        QString line = file.readLine();
        QStringList fields = line.split(",");
        foreach(QString field, fields) {
            doubleList.append(field.toDouble());
        }
    }
    qDebug()<<"loaded"<<doubleList.size();
    return &doubleList;
}



//void FileManager::saveCsvFile(QList<float> *elecList, QList<qint16> *dataList,QString deviceNum) //cur
void FileManager::saveCsvFile(QList<quint16> *elecList, QList<qint32> *dataList,QString deviceNum) //cur
{
    qDebug() << "-------------->" << __func__;
    QString defaultDir = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString defaultFileName =defaultDir+"/"+
            QString("Dev%1_Cur_%2%3").arg(deviceNum).arg(QDateTime::currentDateTime().toString("yymmdd_hhmmss")).arg(".csv");
    QString fileName = QFileDialog::getSaveFileName(nullptr, tr("Save File"), defaultFileName, tr("Excel files (*.csv)"));

    if (fileName.isEmpty()) {
        return;
    }
    qDebug()<<fileName;
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(nullptr, tr("Error"), tr("Could not create file"));
        return;
    }

    QTextStream out(&file);
    //    for (float data : *elecList) { //사이즈없는상태에 foreach 이렇게 접근하면 오류
    //        out << data << "\n";
    //    }
    //    for (qint16 data : *dataList) {
    //        out << data << "\n";
    //    }

    int loadingSize = elecList->size()+dataList->size();
    const int unit =4007;
    int loadingInd = loadingSize/unit;
    int start1=0;
    int start2=0;
    for(int k = 0;k<loadingInd;k++){
        for(int i = start1;i<start1+7;i++){
            out<<elecList->at(i)<< "\n";;
        }
        for(int i = start2;i<start2+4000;i++){
            out<<dataList->at(i)<< "\n";;
        }
        start1+=7;
        start2+=4000;
    }
    file.setFileName(QString("Dev%1_Cur_%2.c").arg(deviceNum).arg(QDateTime::currentDateTime().toString()));
    file.close();
}




void FileManager::saveCsvFile(QList<double> *dataList,QString deviceNum) //fft
{
    qDebug() << "-------------->" << __func__<<"fft";
    QString defaultDir = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString defaultFileName =defaultDir+"/"+
            QString("Dev%1_FFT_%2%3").arg(deviceNum).arg(QDateTime::currentDateTime().toString("yymmdd_hhmmss")).arg(".csv");
    QString fileName = QFileDialog::getSaveFileName(nullptr, tr("Save File"), defaultFileName, tr("Excel files (*.csv)"));

    if (fileName.isEmpty()) {
        return;
    }
    qDebug()<<fileName;
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(nullptr, tr("Error"), tr("Could not create file"));
        return;
    }

    QTextStream out(&file);
    for (double data : *dataList) {
        out << data << "\n";
    }

    file.close();
}

void FileManager::saveImgFile(QString path,QString deviceNum) //해당경로 이미지를 저장
{
    QImage image(path);

      QString defaultDir = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
      QString defaultFileName = defaultDir + QString("/Dev%1_WT_%2.png").arg(deviceNum).arg(QDateTime::currentDateTime().toString("yyMMdd_hhmmss"));

      QString fileName = QFileDialog::getSaveFileName(nullptr, tr("file save"), defaultFileName, tr("img file (*.png)"));

      if (fileName.isEmpty()) {
          return;
      }

      if (image.save(fileName)) {
          qDebug() << "이미지가 성공적으로 저장되었습니다: " << fileName;
      } else {
          QMessageBox::critical(nullptr, tr("오류"), tr("이미지를 저장할 수 없습니다"));
      }
}




//void FileManager::saveAcqFile(QList<QList<float>> tempHistory)
//{

//    QString defaultDir = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
//    QString defaultFileName =defaultDir+"/"+"sample"+QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".csv";
//    QString fileName = QFileDialog::getSaveFileName(nullptr, tr("Save File"), defaultFileName, tr("Excel files (*.csv)"));
//    if (fileName.isEmpty()) {
//        return;
//    }
//    qDebug()<<fileName;
//    QFile file(fileName);
//    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
//        QMessageBox::critical(nullptr, tr("Error"), tr("Could not create file"));
//        return;
//    }
//    QTextStream out(&file);

//    for(int k=0;k<tempHistory.size();k++){
//        for (int i=0; i<tempHistory[k].size(); i++) {
//            out << tempHistory[k][i];
//            //            qDebug()<<tempHistory[k][i];
//            if (i < tempHistory[k].size()-1) { //데이터리스트에서 마지막일때
//                out << ",";
//            }
//        }
//        out << "\n";
//    }



//    file.close();
//    qDebug()<<"tempHistorySize: " <<tempHistory.size();

//}

//void FileManager::changeFilePermissions(QString filePath)
//{
//    QFile file(filePath);
//    QFileInfo fileInfo(file);
//    file.setPermissions(QFile::WriteOwner | QFile::ReadOwner | QFile::ReadGroup | QFile::ReadOther);
//    qDebug() << "File permissions after change:" << fileInfo.permissions();
//}


