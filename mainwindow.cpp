#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dbconnect.h"
#include "adminoper.h"
#include "selleroper.h"
#include "manageroper.h"
#include "currentuser.h"
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
    this->setFixedSize(720,445); //���ù̶���С
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint); //���ر���������С���ɼ�

    ui->LEUser->setFocus(); //���ص��û�����
    ui->LEPswd->setEchoMode(QLineEdit::Password); //��������ʱ��ʾ*

    //Բ��
    QBitmap objBitmap(size());
    QPainter painter(&objBitmap);
    painter.fillRect(rect(),Qt::white);
    painter.setBrush(QColor(0,0,0));
    painter.drawRoundedRect(this->rect(),11,11);
    setMask(objBitmap);

    //���涯�����ı�͸���ȵķ�ʽ����0 - 1����
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(550);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();

    //���ӹرհ�ť�͹رն���
    connect(ui->PBClose, SIGNAL(clicked()), this, SLOT(closeWidget()));
}

void MainWindow::mousePressEvent(QMouseEvent *e) //���������
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
void MainWindow::mouseMoveEvent(QMouseEvent *e) //�����������ƶ�
{
    if(flag == 0) return;
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    last = e->globalPos();
    //qDebug() << last.rx() << ".." << last.ry();
    move(x()+dx, y()+dy);
}
void MainWindow::mouseReleaseEvent(QMouseEvent *e) //����ͷ�
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
    if(Username==NULL)
    {
        QMessageBox::critical(0, "ERROR",
                    "USERNAME IS EMPTY!", QMessageBox::Cancel);
        return;
    }
    QSqlDatabase db = QSqlDatabase::database("connection");
    QSqlQuery query(db);
    query.exec("select * from UserList where Name='"+Username+"'");
    if(!query.first())
    {
        QMessageBox::critical(0, "ERROR",
                    "No such user!", QMessageBox::Cancel);
        ui->LEUser->clear();
        ui->LEPswd->clear();
        ui->LEUser->setFocus();
    }
    else
    {
        QString qstr = query.value(2).toString();
        if(qstr!=Password)
        {
            QMessageBox::critical(0, "ERROR",
                        "Wrong Password!", QMessageBox::Cancel);
            ui->LEPswd->clear();
            ui->LEPswd->setFocus();
        }
        else
        {
//            QMessageBox::information(0,"GoodJob",
//                        "Login is OK!",QMessageBox::Ok);
            ui->LEUser->clear();
            ui->LEPswd->clear();
            ui->LEUser->setFocus();
            this->hide();
            set_currentuser(query.value(0).toInt());
            if(query.value(3)==1)
            {
                AdminOper adminOper;
                adminOper.exec();
                //qDebug() << get_currentuser();
            }
            else if(query.value(3)==2)
            {
//                QMessageBox::information(0,"GoodJob",
//                                        "ManagerLogin is OK!",QMessageBox::Ok);
                ManagerOper managerOper;
                managerOper.exec();
            }
            else if(query.value(3)==3)
            {
//                QMessageBox::information(0,"GoodJob",
//                                       "SellerLogin is OK!",QMessageBox::Ok);
                SellerOper sellerOper;
                sellerOper.exec();
            }
            //qDebug() << "abcd";
            this->show();
        }
    }
}

bool MainWindow::closeWidget()
{
   //���涯�����ı�͸���ȵķ�ʽ��ʧ1 - 0����
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
    //�˳�ϵͳ
    QApplication::quit();
}
