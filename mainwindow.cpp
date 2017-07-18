#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dbconnect.h"
#include "adminoper.h"
#include <QString>
#include <string>
#include <QBitmap>
#include <QPainter>
#include <QDebug>
#include <QPropertyAnimation>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    flag = 0;
    this->setFixedSize(720,445); //设置固定大小
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint); //隐藏标题栏及最小化可见

    //圆角
    QBitmap objBitmap(size());
    QPainter painter(&objBitmap);
    painter.fillRect(rect(),Qt::white);
    painter.setBrush(QColor(0,0,0));
    painter.drawRoundedRect(this->rect(),11,11);
    setMask(objBitmap);

    //界面动画，改变透明度的方式出现0 - 1渐变
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(550);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();

    //连接关闭按钮和关闭动画
    connect(ui->PBClose, SIGNAL(clicked()), this, SLOT(closeWidget()));
}

void MainWindow::mousePressEvent(QMouseEvent *e) //鼠标点击界面
{
    judge.setX(0), judge.setY(0);
    judge = e->pos();
    if(judge.y() > 45 || (judge.x() == 0 && judge.y() == 0))
    {
        flag = 0;
        return ;
    }
    flag = 1;
    last = e->globalPos();
    //qDebug() << judge.rx() << ".." << judge.ry();
}
void MainWindow::mouseMoveEvent(QMouseEvent *e) //界面跟随鼠标移动
{
    if(flag == 0) return;
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    last = e->globalPos();
    //qDebug() << last.rx() << ".." << last.ry();
    move(x()+dx, y()+dy);
}
void MainWindow::mouseReleaseEvent(QMouseEvent *e) //鼠标释放
{
    judge.setX(0), judge.setY(0);
    flag = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_PBMin_clicked()
{
    this->showMinimized();
}

void MainWindow::on_PBLogin_clicked()
{
    QString Username = ui->LEUser->text();
    QString Password = ui->LEPswd->text();
    QSqlDatabase db = QSqlDatabase::database("connection");
    QSqlQuery query(db);
    query.exec("select * from UserList where Name='"+Username+"'");
    if(!query.first())
        QMessageBox::critical(0, "ERROR",
                    "No such user!", QMessageBox::Cancel);
    else
    {
        QString qstr = query.value(2).toString();
        if(qstr!=Password)
            QMessageBox::critical(0, "ERROR",
                        "Wrong Password!", QMessageBox::Cancel);
        else
        {
//            QMessageBox::information(0,"GoodJob",
//                        "Login is OK!",QMessageBox::Ok);
            this->hide();
            AdminOper adminOper;
            adminOper.exec();
            qDebug() << "abcd";
            this->show();
        }
    }
}

bool MainWindow::closeWidget()
{
   //界面动画，改变透明度的方式消失1 - 0渐变
   QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
   animation->setDuration(550);
   animation->setStartValue(1);
   animation->setEndValue(0);
   animation->start();
   connect(animation, SIGNAL(finished()), this, SLOT(close()));

   return true;
}

void MainWindow::closeEvent(QCloseEvent *)
{
    //退出系统
    QApplication::quit();
}
