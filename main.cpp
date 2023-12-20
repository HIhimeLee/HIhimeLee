#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); // 자원관리, 디스플레이,
    MainWindow w;
//    public ConnectionManager conMgr;
    w.show();

    return a.exec();
}
