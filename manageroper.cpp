#include "manageroper.h"
#include "ui_manageroper.h"
#include "dboperation.h"
#include "currentuser.h"
#include "information.h"
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

ManagerOper::~ManagerOper()
{
    delete ui;
}

void ManagerOper::mousePressEvent(QMouseEvent *e) //鼠标点击界面
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
void ManagerOper::mouseMoveEvent(QMouseEvent *e) //界面跟随鼠标移动
{
    if(flag == 0) return;
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    last = e->globalPos();
    //qDebug() << last.rx() << ".." << last.ry();
    move(x()+dx, y()+dy);
}
void ManagerOper::mouseReleaseEvent(QMouseEvent *e) //鼠标释放
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
   //界面动画，改变透明度的方式消失1 - 0渐变
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
    //退出系统
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

void ManagerOper::on_PBMessage2_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}

void ManagerOper::on_PBAdd2_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}

void ManagerOper::on_PBView2_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
}

void ManagerOper::on_PBSend_clicked()
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

void ManagerOper::on_PBSendDel_clicked()
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

void ManagerOper::on_PBSendClear_clicked()
{
    Current_user_send_Info_clear();
    send_renew(infosend);
    ui->TVSend->setModel(infosend);
    ui->TVSend->setColumnWidth(3,0);
    reci_renew(inforeci);
    ui->TVReci->setModel(inforeci);
    ui->TVReci->setColumnWidth(3,0);
}

void ManagerOper::on_PBReciDel_clicked()
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

void ManagerOper::on_PBReciClear_clicked()
{
    Current_user_reci_Info_clear();
    send_renew(infosend);
    ui->TVSend->setModel(infosend);
    ui->TVSend->setColumnWidth(3,0);
    reci_renew(inforeci);
    ui->TVReci->setModel(inforeci);
    ui->TVReci->setColumnWidth(3,0);
}
