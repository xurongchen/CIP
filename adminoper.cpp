#include "adminoper.h"
#include "ui_adminoper.h"
#include "dboperation.h"
#include "currentuser.h"
#include "information.h"
#include "pop.h"
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


    infosend=NULL;
    inforeci=NULL;
    //infosend
    send_renew(infosend);
    ui->TVSend->setModel(infosend);

    //设置列宽不可变动，即不能通过鼠标拖动增加列宽
    ui->TVSend->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
    ui->TVSend->horizontalHeader()->setResizeMode(1, QHeaderView::Fixed);
    //ui->TVSend->horizontalHeader()->setResizeMode(2, QHeaderView::Fixed);
    ui->TVSend->horizontalHeader()->setResizeMode(2, QHeaderView::ResizeToContents);
    //ui->TVSend->horizontalHeader()->setResizeMode(3, QHeaderView::Fixed);
    //设置表格的各列的宽度值
    ui->TVSend->setColumnWidth(0,70);
    ui->TVSend->setColumnWidth(1,80);
    ui->TVSend->setColumnWidth(2,190);
    ui->TVSend->setColumnWidth(3,0);

    //默认显示行头，如果你觉得不美观的话，我们可以将隐藏
    ui->TVSend->verticalHeader()->hide();
    //设置选中时为整行选中
    ui->TVSend->setSelectionBehavior(QAbstractItemView::SelectRows);

    //设置表格的单元为只读属性，即不能编辑
    ui->TVSend->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //如果你用在QTableView中使用右键菜单，需启用该属性
    ui->TVSend->setContextMenuPolicy(Qt::CustomContextMenu);


    //inforeci
    reci_renew(inforeci);
    ui->TVReci->setModel(inforeci);

    //设置列宽不可变动，即不能通过鼠标拖动增加列宽
    ui->TVReci->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
    ui->TVReci->horizontalHeader()->setResizeMode(1, QHeaderView::Fixed);
    //ui->TVReci->horizontalHeader()->setResizeMode(2, QHeaderView::Fixed);
    ui->TVReci->horizontalHeader()->setResizeMode(2, QHeaderView::ResizeToContents);
    //ui->TVReci->horizontalHeader()->setResizeMode(3, QHeaderView::Fixed);
    //设置表格的各列的宽度值
    ui->TVReci->setColumnWidth(0,70);
    ui->TVReci->setColumnWidth(1,80);
    ui->TVReci->setColumnWidth(2,190);
    ui->TVReci->setColumnWidth(3,0);

    //默认显示行头，如果你觉得不美观的话，我们可以将隐藏
    ui->TVReci->verticalHeader()->hide();
    //设置选中时为整行选中
    ui->TVReci->setSelectionBehavior(QAbstractItemView::SelectRows);

    //设置表格的单元为只读属性，即不能编辑
    ui->TVReci->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //如果你用在QTableView中使用右键菜单，需启用该属性
    ui->TVReci->setContextMenuPolicy(Qt::CustomContextMenu);
}

AdminOper::~AdminOper()
{
    delete ui;
    send_delete(infosend);
    reci_delete(inforeci);
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
    if(message==ADD_ERROR_USERNAME_NULL)
    {
//        QMessageBox::critical(0, "ERROR",
//                    "USERNAME SHOULD NOT BE EMPTY!", QMessageBox::Cancel);
        show_word(QString::fromLocal8Bit("错误！"), QString::fromLocal8Bit("账号不能为空！"));
        ui->LEUser->clear();
        ui->LEUser->setFocus();
    }
    if(message==ADD_ERROR_SAME_USERNAME)
    {
//        QMessageBox::critical(0, "ERROR",
//                    "USERNAME HAS BEED USED!", QMessageBox::Cancel);
        show_word(QString::fromLocal8Bit("错误！"), QString::fromLocal8Bit("该账号已被注册！"));
        ui->LEUser->clear();
        ui->LEPswd->clear();
        ui->LEPswd2->clear();
        ui->LEUser->setFocus();
    }
    if(message==ADD_ERROR_DIFF_PASSWORD)
    {
//        QMessageBox::critical(0, "ERROR",
//                    "PASSWORD NOT SAME!", QMessageBox::Cancel);
        show_word(QString::fromLocal8Bit("错误！"), QString::fromLocal8Bit("两次输入的密码不一致！"));
        ui->LEPswd->clear();
        ui->LEPswd2->clear();
        ui->LEPswd->setFocus();
    }
    if(message==ADD_ERROR_PASSWORD_NULL)
    {
//        QMessageBox::critical(0, "ERROR",
//                    "PASSWORD SHOULD NOT BE EMPTY!", QMessageBox::Cancel);
        show_word(QString::fromLocal8Bit("错误！"), QString::fromLocal8Bit("密码不能为空！"));
    }
    if(message==ADD_SUCCESS)
    {
//        QMessageBox::information(0, "SUCCESS",
//                    "NEW USER HAS BEEN REGISTERED!", QMessageBox::Ok);
        show_word(QString::fromLocal8Bit("成功！"), QString::fromLocal8Bit("账号添加成功！"));
        ui->LEUser->clear();
        ui->LEPswd->clear();
        ui->LEPswd2->clear();
        ui->LEUser->setFocus();
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
    {
//        QMessageBox::critical(0, "ERROR",
//                    "UERNAME IS DIFFERENT!", QMessageBox::Cancel);
        show_word(QString::fromLocal8Bit("错误！"), QString::fromLocal8Bit("两次输入的账号不一致！"));
    }
    else
    {
        User u(delstr1);
        int message = u.del();
        if(message==DELETE_ERROR_NO_USER)
        {
//            QMessageBox::critical(0, "ERROR",
//                        "NO SUCH USER!", QMessageBox::Cancel);
            show_word(QString::fromLocal8Bit("错误！"), QString::fromLocal8Bit("该账号不存在！"));
        }
        if(message==DELETE_ERROR_ADMIN)
        {
//            QMessageBox::critical(0, "ERROR",
//                        "ADMIN CANNOT DELETE!", QMessageBox::Cancel);
            show_word(QString::fromLocal8Bit("错误！"), QString::fromLocal8Bit("管理员账号不可删除！"));
        }
        if(message==DELETE_SUCCESS)
        {
            QMessageBox::information(0, "SUCCESS",
                        "SUCCESS TO DELETE A USER!", QMessageBox::Ok);
            show_word(QString::fromLocal8Bit("成功！"), QString::fromLocal8Bit("账号删除成功！"));
        }
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
                    QString("A MESSAGE HAS BEEN BROADCAST!"), QMessageBox::Ok);
    }
    send_renew(infosend);
    ui->TVSend->setModel(infosend);
    ui->TVSend->setColumnWidth(3,0);
    reci_renew(inforeci);
    ui->TVReci->setModel(inforeci);
    ui->TVReci->setColumnWidth(3,0);

}

void AdminOper::on_PBSendDel_clicked()
{
    int curRow=ui->TVSend->currentIndex().row(); //选中行
    QAbstractItemModel *modessl = ui->TVSend->model();
    QModelIndex indextemp = modessl->index(curRow,3);//遍历第一行的所有列,i是你要取值的列的下标
    //这个是一个单元格的值。tostring()----ok
    QVariant datatemp = modessl->data(indextemp);
    Info i(datatemp.toInt());
    i.del(SENDDEL);
    send_renew(infosend);
    ui->TVSend->setModel(infosend);
    ui->TVSend->setColumnWidth(3,0);
    reci_renew(inforeci);
    ui->TVReci->setModel(inforeci);
    ui->TVReci->setColumnWidth(3,0);
}

void AdminOper::on_PBSendClear_clicked()
{
    Current_user_send_Info_clear();
    send_renew(infosend);
    ui->TVSend->setModel(infosend);
    ui->TVSend->setColumnWidth(3,0);
    reci_renew(inforeci);
    ui->TVReci->setModel(inforeci);
    ui->TVReci->setColumnWidth(3,0);
}

void AdminOper::on_PBReciDel_clicked()
{
    int curRow=ui->TVReci->currentIndex().row(); //选中行
    QAbstractItemModel *modessl = ui->TVReci->model();
    QModelIndex indextemp = modessl->index(curRow,3);//遍历第一行的所有列,i是你要取值的列的下标
    //这个是一个单元格的值。tostring()----ok
    QVariant datatemp = modessl->data(indextemp);
    Info i(datatemp.toInt());
    int message = i.del(RECIDEL);
    if(message==INFO_DEL_FAILED_NO_PERMISSION)
        QMessageBox::critical(0, "ERROR",
                    QString("THIS IS A BROADCAST,ONLY SENDER CAN DELETE IN SEND TABLE!"), QMessageBox::Cancel);
    send_renew(infosend);
    ui->TVSend->setModel(infosend);
    ui->TVSend->setColumnWidth(3,0);
    reci_renew(inforeci);
    ui->TVReci->setModel(inforeci);
    ui->TVReci->setColumnWidth(3,0);
}


void AdminOper::on_PBReciClear_clicked()
{
    Current_user_reci_Info_clear();
    send_renew(infosend);
    ui->TVSend->setModel(infosend);
    ui->TVSend->setColumnWidth(3,0);
    reci_renew(inforeci);
    ui->TVReci->setModel(inforeci);
    ui->TVReci->setColumnWidth(3,0);
}
