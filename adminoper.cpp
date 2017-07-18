#include "adminoper.h"
#include "ui_adminoper.h"
#include "dboperation.h"
#include "currentuser.h"
#include <QString>
#include <string>
#include <QBitmap>
#include <QPainter>
#include <QDebug>
#include <QPropertyAnimation>
#include <QLineEdit>
#include <QMessageBox>

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

    ui->LEPswd->setEchoMode(QLineEdit::Password);
    ui->LEPswd2->setEchoMode(QLineEdit::Password);

    //圆角
    QBitmap objBitmap(size());
    QPainter painter(&objBitmap);
    painter.fillRect(rect(),Qt::white);
    painter.setBrush(QColor(0,0,0));
    painter.drawRoundedRect(this->rect(),11,11);
    setMask(objBitmap);

    //连接关闭按钮和关闭动画
    connect(ui->PBClose, SIGNAL(clicked()), this, SLOT(closeWidget()));

    ui->tabWidget->setCurrentIndex(0);
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

void AdminOper::on_PBDelete_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}

void AdminOper::on_PBMessage_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}

void AdminOper::on_PBSend1_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}

void AdminOper::on_PBSend2_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}

void AdminOper::on_PBSend3_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}

void AdminOper::on_PBMy1_clicked()
{
    ui->tabWidget->setCurrentIndex(3);
}

void AdminOper::on_PBMy2_clicked()
{
    ui->tabWidget->setCurrentIndex(3);
}

void AdminOper::on_PBMy3_clicked()
{
    ui->tabWidget->setCurrentIndex(3);
}

void AdminOper::on_PBGet1_clicked()
{
    ui->tabWidget->setCurrentIndex(4);
}

void AdminOper::on_PBGet2_clicked()
{
    ui->tabWidget->setCurrentIndex(4);
}

void AdminOper::on_PBGet3_clicked()
{
    ui->tabWidget->setCurrentIndex(4);
}


/*
#define ADD_SUCCESS 0
#define ADD_ERROR_SAME_USERNAME -1
#define ADD_ERROR_DIFF_PASSWORD -2
#define ADD_ERROR_PASSWORD_NULL -3
*/
void AdminOper::on_PBLogin_clicked()
{
    QString str_user = ui->LEUser->text();
    QString str_pswd1 = ui->LEPswd->text();
    QString str_pswd2 = ui->LEPswd2->text();
    int index = ui->comboBox->currentIndex();
    int role;
    if(index==0)
        role=3;
    else role=2;
    User newuser(str_user,str_pswd1,str_pswd2,role);
    int message = newuser.add();
    if(message==ADD_ERROR_SAME_USERNAME)
    {
        QMessageBox::critical(0, "ERROR",
                    "USERNAME HAS BEED USED!", QMessageBox::Cancel);
        ui->LEUser->clear();
    }
    if(message==ADD_ERROR_DIFF_PASSWORD)
    {
        QMessageBox::critical(0, "ERROR",
                    "PASSWORD NOT SAME!", QMessageBox::Cancel);
        ui->LEPswd->clear();
        ui->LEPswd2->clear();
    }
    if(message==ADD_ERROR_PASSWORD_NULL)
        QMessageBox::critical(0, "ERROR",
                    "PASSWORD SHOULD NOT BE EMPTY!", QMessageBox::Cancel);
    if(message==ADD_SUCCESS)
    {
        QMessageBox::information(0, "SUCCESS",
                    "NEW USER HAS BEEN REGISTERED!", QMessageBox::Ok);
        ui->LEUser->clear();
        ui->LEPswd->clear();
        ui->LEPswd2->clear();
    }
}

/*
#define DELETE_SUCCESS 0
#define DELETE_ERROR_NO_USER -1
#define DELETE_ERROR_ADMIN -2
*/

void AdminOper::on_PBDele_clicked()
{
    QString delstr1 = ui->LEDelete->text();
    QString delstr2 = ui->LEDelete2->text();
    if(delstr1!=delstr2)
        QMessageBox::critical(0, "ERROR",
                    "UERNAME IS DIFFERENT!", QMessageBox::Cancel);
    else
    {
        User u(delstr1);
        int message = u.del();
        if(message==DELETE_ERROR_NO_USER)
            QMessageBox::critical(0, "ERROR",
                        "NO SUCH USER!", QMessageBox::Cancel);
        if(message==DELETE_ERROR_ADMIN)
            QMessageBox::critical(0, "ERROR",
                        "ADMIN CANNOT DELETE!", QMessageBox::Cancel);
        if(message==DELETE_SUCCESS)
            QMessageBox::information(0, "SUCCESS",
                        "SUCCESS TO DELETE A USER!", QMessageBox::Ok);
    }
    ui->LEDelete->clear();
    ui->LEDelete2->clear();
}


void AdminOper::on_PBSend_clicked()
{
    QString strsend = ui->LESend->text();
    QString contentsend = ui->TESend->toPlainText();
    User u(strsend);
    int message = u.query();
    if(message==QUERY_NO_USER&&strsend!=NULL)
        QMessageBox::critical(0, "ERROR",
                    "NO SUCH USER!", QMessageBox::Cancel);
    else if(strsend!=NULL)
    {
        Info i(get_currentuser(),message,contentsend);
        i.add();
        QMessageBox::information(0, "SUCCESS",
                    QString("A MESSAGE HAS BEEN SENT TO %1!").arg(strsend), QMessageBox::Ok);
    }
    else
    {
        Info i(get_currentuser(),-1,contentsend);
        i.add();
        QMessageBox::information(0, "SUCCESS",
                    QString("A MESSAGE HAS BEEN BROADCAST!").arg(strsend), QMessageBox::Ok);
    }
}
