#include "selleroper.h"
#include "ui_selleroper.h"
#include <QString>
#include <string>
#include <QBitmap>
#include <QPainter>
#include <QDebug>
#include <QPropertyAnimation>



SellerOper::SellerOper(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SellerOper)
{
    ui->setupUi(this);
    ui->tabWidget->setStyleSheet("QTabWidget:pane {border-top:0px solid #e8f3f9;background:  transparent; }"); //设置tab背景透明
    ui->tabWidget->findChildren<QTabBar*>().at(0)->hide(); //tab标题栏隐藏
    flag = 0;
    flag2 = 0;
    this->setFixedSize(720,445); //设置固定大小
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint); //隐藏标题栏及最小化可见

    //圆角
    QBitmap objBitmap(size());
    QPainter painter(&objBitmap);
    painter.fillRect(rect(),Qt::white);
    painter.setBrush(QColor(0,0,0));
    painter.drawRoundedRect(this->rect(),11,11);
    setMask(objBitmap);

    //连接关闭按钮和关闭动画
    connect(ui->PBClose, SIGNAL(clicked()), this, SLOT(closeWidget()));

    ui->dateEdit->setMinimumDate(QDate::currentDate().addDays(0));
    ui->dateEdit->setMaximumDate(QDate::currentDate().addDays(365));  // +365天
    ui->dateEdit->setCalendarPopup(true);  // 日历弹出
    //ui->dateEdit->setCalendarWidget();

    ui->tabWidget->setCurrentIndex(0);
}

SellerOper::~SellerOper()
{
    delete ui;
}

void SellerOper::mousePressEvent(QMouseEvent *e) //鼠标点击界面
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
void SellerOper::mouseMoveEvent(QMouseEvent *e) //界面跟随鼠标移动
{
    if(flag == 0) return;
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    last = e->globalPos();
    //qDebug() << last.rx() << ".." << last.ry();
    move(x()+dx, y()+dy);
}
void SellerOper::mouseReleaseEvent(QMouseEvent *e) //鼠标释放
{
    judge.setX(0), judge.setY(0);
    flag = 0;
}

bool SellerOper::closeWidget()
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

void SellerOper::closeEvent(QCloseEvent *)
{
    //退出系统
    if(flag2 != 1)
    {
        qDebug() << "exit";
        QApplication::quit();
    }
}

void SellerOper::on_PBMin_clicked()
{
    this->showMinimized();
}

void SellerOper::on_PBReturn_clicked()
{
    flag2 = 1;
    this->close();
}

void SellerOper::on_PBWork_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
}

void SellerOper::on_PBMessage_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}

void SellerOper::on_PBSend1_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}

void SellerOper::on_PBSend1_2_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}

void SellerOper::on_PBSend1_3_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}

void SellerOper::on_PBMy1_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}

void SellerOper::on_PBMy1_2_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}

void SellerOper::on_PBMy1_3_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}

void SellerOper::on_PBGet1_clicked()
{
    ui->tabWidget->setCurrentIndex(3);
}

void SellerOper::on_PBGet1_2_clicked()
{
    ui->tabWidget->setCurrentIndex(3);
}

void SellerOper::on_PBGet1_3_clicked()
{
    ui->tabWidget->setCurrentIndex(3);
}

void SellerOper::on_PBA1_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
}

void SellerOper::on_PBB1_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
}

void SellerOper::on_PBC1_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
}

void SellerOper::on_PBA2_clicked()
{
    ui->tabWidget->setCurrentIndex(4);
}

void SellerOper::on_PBB2_clicked()
{
    ui->tabWidget->setCurrentIndex(4);
}

void SellerOper::on_PBC2_clicked()
{
    ui->tabWidget->setCurrentIndex(4);
}

void SellerOper::on_PBA3_clicked()
{
    ui->tabWidget->setCurrentIndex(5);
}

void SellerOper::on_PBB3_clicked()
{
    ui->tabWidget->setCurrentIndex(5);
}

void SellerOper::on_PBC3_clicked()
{
    ui->tabWidget->setCurrentIndex(5);
}

void SellerOper::on_PBWork2_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
}

void SellerOper::on_PBMessage2_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}
