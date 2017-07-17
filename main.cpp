#include <QtGui/QApplication>
#include "mainwindow.h"
#include "dbconnect.h"
#include "test.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    createConnection();
    test();
    MainWindow w;
    w.show();

    return a.exec();
}
