#include <QtGui/QApplication>
#include "mainwindow.h"
#include "dbconnect.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    createConnection();
    MainWindow w;
    w.show();

    return a.exec();
}
