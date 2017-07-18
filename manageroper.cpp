#include "manageroper.h"
#include "ui_manageroper.h"
#include <QString>
#include <string>
#include <QBitmap>
#include <QPainter>
#include <QDebug>
#include <QPropertyAnimation>
#include <QLineEdit>
#include <QMessageBox>

ManagerOper::ManagerOper(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ManagerOper)
{
    ui->setupUi(this);
    ui->tabWidget->setStyleSheet("QTabWidget:pane {border-top:0px solid #e8f3f9;background:  transparent; }"); //����tab����͸��
    ui->tabWidget->findChildren<QTabBar*>().at(0)->hide(); //tab����������
    flag = 0;
    flag2 = 0;
    this->setFixedSize(720,445); //���ù̶���С
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint); //���ر���������С���ɼ�

    //Բ��
    QBitmap objBitmap(size());
    QPainter painter(&objBitmap);
    painter.fillRect(rect(),Qt::white);
    painter.setBrush(QColor(0,0,0));
    painter.drawRoundedRect(this->rect(),11,11);
    setMask(objBitmap);

    //���ӹرհ�ť�͹رն���
    connect(ui->PBClose, SIGNAL(clicked()), this, SLOT(closeWidget()));

    ui->tabWidget->setCurrentIndex(0);
}

ManagerOper::~ManagerOper()
{
    delete ui;
}

void ManagerOper::mousePressEvent(QMouseEvent *e) //���������
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
void ManagerOper::mouseMoveEvent(QMouseEvent *e) //�����������ƶ�
{
    if(flag == 0) return;
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    last = e->globalPos();
    //qDebug() << last.rx() << ".." << last.ry();
    move(x()+dx, y()+dy);
}
void ManagerOper::mouseReleaseEvent(QMouseEvent *e) //����ͷ�
{
    judge.setX(0), judge.setY(0);
    flag = 0;
}

void ManagerOper::on_PBReturn_clicked()
{
    flag2 = 1;
    this->close();
}

void ManagerOper::on_PBMin_clicked()
{
    this->showMinimized();
}
bool ManagerOper::closeWidget()
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

void ManagerOper::closeEvent(QCloseEvent *)
{
    //�˳�ϵͳ
    if(flag2 != 1)
    {
        qDebug() << "exit";
        QApplication::quit();
    }
}

void ManagerOper::on_PBView_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
}

void ManagerOper::on_PBAdd_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}

void ManagerOper::on_PBMessage_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}

void ManagerOper::on_PBSend1_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}

void ManagerOper::on_PBSend2_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}

void ManagerOper::on_PBSend3_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}

void ManagerOper::on_PBMy1_clicked()
{
    ui->tabWidget->setCurrentIndex(3);
}

void ManagerOper::on_PBMy2_clicked()
{
    ui->tabWidget->setCurrentIndex(3);
}

void ManagerOper::on_PBMy3_clicked()
{
    ui->tabWidget->setCurrentIndex(3);
}

void ManagerOper::on_PBGet1_clicked()
{
    ui->tabWidget->setCurrentIndex(4);
}

void ManagerOper::on_PBGet2_clicked()
{
    ui->tabWidget->setCurrentIndex(4);
}

void ManagerOper::on_PBGet3_clicked()
{
    ui->tabWidget->setCurrentIndex(4);
}
