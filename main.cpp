#include <QtGui/QApplication>
#include "mainwindow.h"
#include "dbconnect.h"
#include "test.h"
#include "currentuser.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    createConnection();
    //reset_currentuser();
    test();
    MainWindow w;
    w.show();

    return a.exec();
}
