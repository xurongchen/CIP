#include "adminoper.h"
#include "ui_adminoper.h"
#include <QString>
#include <string>
#include <QBitmap>
#include <QPainter>
#include <QDebug>
#include <QPropertyAnimation>

AdminOper::AdminOper(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdminOper)
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
}

AdminOper::~AdminOper()
{
    delete ui;
}

void AdminOper::mousePressEvent(QMouseEvent *e) //鼠标点击界面
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
void AdminOper::mouseMoveEvent(QMouseEvent *e) //界面跟随鼠标移动
{
    if(flag == 0) return;
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    last = e->globalPos();
    //qDebug() << last.rx() << ".." << last.ry();
    move(x()+dx, y()+dy);
}
void AdminOper::mouseReleaseEvent(QMouseEvent *e) //鼠标释放
{
    judge.setX(0), judge.setY(0);
    flag = 0;
}

void AdminOper::on_PBMin_clicked()
{
    this->showMinimized();
}

bool AdminOper::closeWidget()
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

void AdminOper::closeEvent(QCloseEvent *)
{
    //退出系统
    if(flag2 != 1)
    {
        qDebug() << "exit";
        QApplication::quit();
    }
}

void AdminOper::on_PBReturn_clicked()
{
    flag2 = 1;
    this->close();
}

void AdminOper::on_PBAdd_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
}

void AdminOper::on_pushButton_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}
